#include "rsa_library.h"
#include <fstream>
#include <vector>
#include <windows.h>
#include <iostream>

// Внутрішня функція для безпечного множення 64-бітних чисел за модулем 
unsigned long long MulMod(unsigned long long a, unsigned long long b, unsigned long long mod) {
    unsigned long long res = 0;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b /= 2;
    }
    return res;
}

// Внутрішня функція для двійкового алгоритму піднесення до степеня за модулем
unsigned long long PowerMod(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = MulMod(res, base, mod);
        }
        base = MulMod(base, base, mod);
        exp /= 2;
    }
    return res;
}

// Внутрішня функція для пошуку Найбільшого спільного дільника 
unsigned long long GCD(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Розширений алгоритм Евкліда для пошуку оберненого елемента, тобто обчислення ключа D
long long ExtendedGCD(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = ExtendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return gcd;
}

// Експортована функція генерації ключів
extern "C" void GenerateRSAKeys(unsigned long long p, unsigned long long q,
    unsigned long long& e, unsigned long long& d, unsigned long long& n) {

    n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);

    e = 65537;
    if (e >= phi || GCD(e, phi) != 1) {
        e = 3;
        while (GCD(e, phi) != 1) {
            e += 2;
        }
    }

    long long x, y;
    ExtendedGCD((long long)e, (long long)phi, x, y);
    d = (x % (long long)phi + (long long)phi) % (long long)phi;
}

// Експортована функція шифрування
extern "C" unsigned long long EncryptRSA(unsigned long long message, unsigned long long e, unsigned long long n) {
    return PowerMod(message, e, n);
}

// Експортована функція розшифрування
extern "C" unsigned long long DecryptRSA(unsigned long long ciphertext, unsigned long long d, unsigned long long n) {
    return PowerMod(ciphertext, d, n);
}


// Функцiя контролю цiлiсностi DLL ---
BOOL CheckIntegrity(HMODULE hModule) {
    char path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return FALSE;

    std::streamsize totalSize = file.tellg();
    if (totalSize <= sizeof(DWORD)) return FALSE;

    std::streamsize originalSize = totalSize - sizeof(DWORD);
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(originalSize);
    file.read(buffer.data(), originalSize);

    DWORD storedChecksum = 0;
    file.read(reinterpret_cast<char*>(&storedChecksum), sizeof(DWORD));
    file.close();

    DWORD calculatedSum = 0;
    for (size_t i = 0; i < originalSize; ++i) {
        calculatedSum += static_cast<unsigned char>(buffer[i]);
    }

    return (calculatedSum == storedChecksum);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::cout << "\n[DllMain] DLL_PROCESS_ATTACH | lpReserved = " << lpReserved << std::endl;

        if (!CheckIntegrity(hModule)) {
            std::cout << "[DllMain] Порушено цiлiснiсть DLL! Завантаження скасовано." << std::endl;
            return FALSE; 
        }
        std::cout << "[DllMain] Перевiрка цiлiсностi пройдена успiшно!" << std::endl;
        break;

    case DLL_PROCESS_DETACH:
        std::cout << "[DllMain] DLL_PROCESS_DETACH | lpReserved = " << lpReserved << std::endl;
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}