#include <iostream>
#include <vector>
#include <iomanip>
#include "rsa_library.h"

int main() {
    // Встановлення української локалi для коректного вiдображення символiв у консолi
    setlocale(LC_ALL, "Ukrainian");

    std::cout << "--- Запуск головної програми першим режимом використання dll ---" << std::endl << std::endl;

    // Визначаємо великi простi числа для генерацiї 64-бiтних ключiв
    unsigned long long p0 = 65521;
    unsigned long long q0 = 65519;
    unsigned long long e0, d0, n0;

    unsigned long long p1 = 99991;
    unsigned long long q1 = 99989;
    unsigned long long e1, d1, n1;

    // Формуємо 2 пари ключiв (d0, e0, n0, d1, e1, n1) за допомогою нашої dll
    GenerateRSAKeys(p0, q0, e0, d0, n0);
    GenerateRSAKeys(p1, q1, e1, d1, n1);

    std::cout << "Згенеровано ключi для користувача 0:" << std::endl;
    std::cout << "  Вiдкритий {e0, n0}: {" << e0 << ", " << n0 << "}" << std::endl;
    std::cout << "  Таємний    {d0, n0}: {" << d0 << ", " << n0 << "}" << std::endl << std::endl;

    std::cout << "Згенеровано ключi для користувача 1:" << std::endl;
    std::cout << "  Вiдкритий {e1, n1}: {" << e1 << ", " << n1 << "}" << std::endl;
    std::cout << "  Таємний    {d1, n1}: {" << d1 << ", " << n1 << "}" << std::endl << std::endl;

    // Масив вiдкритих тестових даних t[i]
    std::vector<unsigned long long> t = { 12345, 67890, 444555, 999999, 11223344 };

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << std::setw(12) << "Вхiднi t[i]"
        << std::setw(15) << "Зашифр. e1t"
        << std::setw(15) << "Розшифр. d1"
        << std::setw(15) << "Зашифр. e0"
        << std::setw(15) << "Фiнал d0" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;

    // 2. Виконання циклу для рiзних даних
    for (size_t i = 0; i < t.size(); i++) {
        unsigned long long current_t = t[i];

        //Зашифрування вiдкритого даного t[i] за допомогою ключа {e1, n1}
        unsigned long long e1t = EncryptRSA(current_t, e1, n1);

        //Розшифрування даного e1t за допомогою ключа {d1, n1}
        unsigned long long d1e1t = DecryptRSA(e1t, d1, n1);

        // Перевiрка першого етапу d1e1t = t[i]
        if (d1e1t != current_t) {
            std::cerr << "\nНевiдповiднiсть на першому етапi для значення " << current_t << std::endl;
            return 1;
        }

        // Зашифрування отриманого пiсля розшифрування d1e1t за допомогою ключа {e0, n0}
        unsigned long long e0d1e1t = EncryptRSA(d1e1t, e0, n0);

        //Розшифрування даного за допомогою ключа {d0, n0}
        unsigned long long d0e0d1e1t = DecryptRSA(e0d1e1t, d0, n0);

        // Перевiрка другого етапу d0e0d1e1t = t[i] 
        if (d0e0d1e1t != current_t) {
            std::cerr << "\nНевiдповiднiсть на другому етапi для значення " << current_t << std::endl;
            return 1;
        }

        // Виведення результатiв поточної iтерацiї у форматизовану таблицю
        std::cout << std::setw(12) << current_t
            << std::setw(15) << e1t
            << std::setw(15) << d1e1t
            << std::setw(15) << e0d1e1t
            << std::setw(15) << d0e0d1e1t << std::endl;
    }

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "\nТестування успiшно завершено" << std::endl;

    return 0;
}