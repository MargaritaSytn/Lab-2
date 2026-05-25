#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

// Функцiя для пiдрахунку контрольної суми 
DWORD CalculateChecksum(const std::vector<char>& buffer, size_t size) {
    DWORD sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += static_cast<unsigned char>(buffer[i]);
    }
    return sum;
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "Ukrainian");

    // Перевiрка, що параметри заданi
    if (argc < 2) {
        std::cerr << "Помилка! Передайте шлях до DLL у параметрах командного рядка." << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::cout << "--- Програма розрахунку контрольної суми ---" << std::endl;
    std::cout << "Обробка файлу: " << filePath << std::endl;

    // 1. Вiдкриваємо DLL як двiйковий файл для читання
    std::ifstream fileIn(filePath, std::ios::binary | std::ios::ate);
    if (!fileIn) {
        std::cerr << "Не вдалося вiдкрити файл DLL!" << std::endl;
        return 1;
    }

    std::streamsize size = fileIn.tellg();
    fileIn.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!fileIn.read(buffer.data(), size)) {
        std::cerr << "Помилка читання файлу!" << std::endl;
        return 1;
    }
    fileIn.close();

    DWORD checksum = CalculateChecksum(buffer, size);
    std::cout << "Обчислена контрольна сума: " << checksum << std::endl;

    std::ofstream fileOut(filePath, std::ios::binary | std::ios::app);
    if (!fileOut) {
        std::cerr << "Не вдалося вiдкрити файл для запису!" << std::endl;
        return 1;
    }
    fileOut.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    fileOut.close();
    std::cout << "Контрольну суму успiшно записано в кiнець DLL!" << std::endl;

    // 4. Контроль обчисленого значення
    std::ifstream fileVerify(filePath, std::ios::binary | std::ios::ate);
    std::streamsize newSize = fileVerify.tellg();
    fileVerify.seekg(newSize - sizeof(DWORD), std::ios::beg);

    DWORD readChecksum = 0;
    fileVerify.read(reinterpret_cast<char*>(&readChecksum), sizeof(readChecksum));
    fileVerify.close();

    if (readChecksum == checksum) {
        std::cout << "Перевiрка успiшна, формування контрольного елементу правильне." << std::endl;
    }
    else {
        std::cout << "Значення не збiгаються!" << std::endl;
    }

    return 0;
}