#pragma once
#ifdef RSALIBRARY_EXPORTS
#define RSA_API __declspec(dllexport)
#else
#define RSA_API __declspec(dllimport)
#endif

extern "C" {

    
    //Функція для генерації ключів RSA на основі двох простих чисел p та q.
    RSA_API void GenerateRSAKeys(unsigned long long p, unsigned long long q,
        unsigned long long& e, unsigned long long& d, unsigned long long& n);

    //Функція шифрування повідомлення за допомогою алгоритму RSA.
    RSA_API unsigned long long EncryptRSA(unsigned long long message, unsigned long long e, unsigned long long n);

    //Функція розшифрування повідомлення за допомогою алгоритму RSA.
    RSA_API unsigned long long DecryptRSA(unsigned long long ciphertext, unsigned long long d, unsigned long long n);

}