#pragma once

extern "C" {

    // Функція для генерації ключів RSA на основі двох простих чисел p та q.
    void GenerateRSAKeys(unsigned long long p, unsigned long long q,
        unsigned long long& e, unsigned long long& d, unsigned long long& n);

    // Функція шифрування повідомлення за допомогою алгоритму RSA.
    unsigned long long EncryptRSA(unsigned long long message, unsigned long long e, unsigned long long n);

    // Функція розшифрування повідомлення за допомогою алгоритму RSA.
    unsigned long long DecryptRSA(unsigned long long ciphertext, unsigned long long d, unsigned long long n);

}