#include <iostream>
#include <windows.h>
#define IDS_LASTNAME   101
#define IDS_FACULTY    102
#define IDS_GROUP      103
#define IDS_DISCIPLINE 104

int main() {
    setlocale(LC_ALL, "Ukrainian");

    int choice;
    std::cout << "========================================" << std::endl;
    std::cout << "Оберiть мову / Choose language:" << std::endl;
    std::cout << "1 - Українська (Ukrainian)" << std::endl;
    std::cout << "2 - English" << std::endl;
    std::cout << "Ваш вибiр / Your choice: ";
    std::cin >> choice;
    std::cout << "========================================" << std::endl << std::endl;

    HMODULE hLangLib = NULL;

    // Динамiчне завантаження вiдповiдної бiблiотеки ресурсiв
    if (choice == 1) {
        hLangLib = LoadLibrary(TEXT("LangUKR.dll"));
    }
    else if (choice == 2) {
        hLangLib = LoadLibrary(TEXT("LangENG.dll"));
    }
    else {
        std::cout << "Невiрний вибiр / Invalid choice!" << std::endl;
        return 1;
    }

    // Перевiрка успiшностi завантаження ресурсної DLL
    if (hLangLib == NULL) {
        std::cerr << "Помилка завантаження бiблiотеки мови! Код: " << GetLastError() << std::endl;
        return 1;
    }

    char buffer[256];

    std::cout << "--- Завантаженi данi з DLL мови ---" << std::endl;

    // Почергове завантаження рядкiв за їх iдентифiкаторами через LoadStringA
    if (LoadStringA(hLangLib, IDS_LASTNAME, buffer, sizeof(buffer))) {
        std::cout << buffer << std::endl;
    }
    if (LoadStringA(hLangLib, IDS_FACULTY, buffer, sizeof(buffer))) {
        std::cout << buffer << std::endl;
    }
    if (LoadStringA(hLangLib, IDS_GROUP, buffer, sizeof(buffer))) {
        std::cout << buffer << std::endl;
    }
    if (LoadStringA(hLangLib, IDS_DISCIPLINE, buffer, sizeof(buffer))) {
        std::cout << buffer << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;

    // Звiльнення бiблiотеки з пам'ятi
    FreeLibrary(hLangLib);
    return 0;
}