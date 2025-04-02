// lab4.cpp
#include <iostream>
#include "locale.h"
#include "Lexeme.h"
#include "Scanner.h"
#include "Diagram.h"
#include <stdio.h>

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    int mod = 1;

    if (mod == 0) {
        Scanner* sc;
        int type;
        TypeLex l;
        const char* fileName = "input.txt";
        sc = new Scanner(fileName);
        do {
            type = sc->scanner(l);
            printf("%s - Тип %d \n", l, type);
        } while (type != typeEnd);
    }
    else if (mod == 1) {
        int type;
        TypeLex l;

        FILE* file = fopen("input.txt", "r");
        if (file == nullptr) {
            std::cerr << "Ошибка открытия файла" << std::endl;
            return 1;
        }

        Scanner* scanner = new Scanner(file);
        Diagram* diagram = new Diagram(scanner);

        diagram->parseProgram();

        type = scanner->scanner(l);


        if (type == typeEnd)
            printf(GREEN "Синтаксических ошибок не обнаружено.\n" RESET);
        else
            scanner->printError(RED "Error:" RESET " Лишний текст в конце программы.", "");

        delete scanner;
        delete diagram;
    }
    
    return 0;
}
