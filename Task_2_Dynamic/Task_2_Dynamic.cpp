#include <iostream>
#include <vector>
#include <iomanip>
#include <windows.h> 

// Завдання 3. Визначення типiв функцiй за допомогою вказiвникiв
typedef void (*pGenerateRSAKeys)(unsigned long long, unsigned long long, unsigned long long&, unsigned long long&, unsigned long long&);
typedef unsigned long long (*pEncryptRSA)(unsigned long long, unsigned long long, unsigned long long);
typedef unsigned long long (*pDecryptRSA)(unsigned long long, unsigned long long, unsigned long long);

int main() {
    setlocale(LC_ALL, "Ukrainian");
    std::cout << "--- Запуск головної програми в ДИНАМiЧНОМУ режимi використання DLL ---" << std::endl << std::endl;

    // Завдання 4. Завантаження бiблiотеки та перевiрка успiшностi
    HMODULE hLib = LoadLibrary(TEXT("Task 1.dll"));

    if (hLib == NULL) {
        std::cerr << "Помилка! Не вдалося завантажити бiблiотеку Task 1.dll. Код помилки: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Бiблiотеку успiшно завантажено в пам'ять!" << std::endl;

    // Завдання 5. Визначення адрес функцiй
    pGenerateRSAKeys GenerateRSAKeys = (pGenerateRSAKeys)GetProcAddress(hLib, "GenerateRSAKeys");
    pEncryptRSA EncryptRSA = (pEncryptRSA)GetProcAddress(hLib, "EncryptRSA");
    pDecryptRSA DecryptRSA = (pDecryptRSA)GetProcAddress(hLib, "DecryptRSA");

    // Перевiрка, чи успiшно знайдено всi функцiї
    if (!GenerateRSAKeys || !EncryptRSA || !DecryptRSA) {
        std::cerr << "Помилка! Не вдалося знайти адреси експортованих функцiй." << std::endl;
        FreeLibrary(hLib);
        return 1;
    }
    std::cout << "Адреси всiх функцiй успiшно отримано!" << std::endl << std::endl;

    // Завдання 6. Перевiрка роботи функцiй
    unsigned long long p0 = 65521, q0 = 65519;
    unsigned long long e0, d0, n0;

    unsigned long long p1 = 99991, q1 = 99989;
    unsigned long long e1, d1, n1;

    GenerateRSAKeys(p0, q0, e0, d0, n0);
    GenerateRSAKeys(p1, q1, e1, d1, n1);

    std::cout << "Згенеровано ключi для користувача 0:" << std::endl;
    std::cout << "  Вiдкритий {e0, n0}: {" << e0 << ", " << n0 << "}" << std::endl;
    std::cout << "  Таємний   {d0, n0}: {" << d0 << ", " << n0 << "}" << std::endl << std::endl;

    std::cout << "Згенеровано ключi для користувача 1:" << std::endl;
    std::cout << "  Вiдкритий {e1, n1}: {" << e1 << ", " << n1 << "}" << std::endl;
    std::cout << "  Таємний   {d1, n1}: {" << d1 << ", " << n1 << "}" << std::endl << std::endl;

    std::vector<unsigned long long> t = { 12345, 67890, 444555, 999999, 11223344 };

    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    std::cout << std::setw(12) << "Вхiднi t[i]"
        << std::setw(15) << "Зашифр. e1t"
        << std::setw(15) << "Розшифр. d1"
        << std::setw(15) << "Зашифр. e0"
        << std::setw(15) << "Фiнал d0" << std::endl;
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < t.size(); i++) {
        unsigned long long current_t = t[i];

        unsigned long long e1t = EncryptRSA(current_t, e1, n1);
        unsigned long long d1e1t = DecryptRSA(e1t, d1, n1);

        if (d1e1t != current_t) {
            std::cerr << "\nНевiдповiднiсть на першому етапi!" << std::endl;
            FreeLibrary(hLib);
            return 1;
        }

        unsigned long long e0d1e1t = EncryptRSA(d1e1t, e0, n0);
        unsigned long long d0e0d1e1t = DecryptRSA(e0d1e1t, d0, n0);

        if (d0e0d1e1t != current_t) {
            std::cerr << "\nНевiдповiднiсть на другому етапi!" << std::endl;
            FreeLibrary(hLib);
            return 1;
        }

        std::cout << std::setw(12) << current_t
            << std::setw(15) << e1t
            << std::setw(15) << d1e1t
            << std::setw(15) << e0d1e1t
            << std::setw(15) << d0e0d1e1t << std::endl;
    }

    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    std::cout << "\nТестування в динамiчному режимi успiшно завершено!" << std::endl;

    // Звiльнення бiблiотеки з пам'ятi
    FreeLibrary(hLib);
    return 0;
}