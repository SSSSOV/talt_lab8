// Scanner.cpp
#include "Scanner.h"
#include <cstdio>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
TypeLex keyWord[MAX_NUM_KEY_WORD] = { "class", "void", "int", "short", "long", "bool", "for", "main", "true", "false" };
int indexKeyWord[MAX_NUM_KEY_WORD] = { typeClass, typeVoid, typeInt,typeShort, typeLong,  typeBool, typeFor, typeMain, typeTrue, typeFalse, constHex, constInt };

Scanner::Scanner(FILE* in) {
    getData(in);
    putPointer(0);
    pos = 0;
    line = 1;
}

Scanner::Scanner(const char* fileName) {
    getData(fileName);
    uk = 0;
    pos = 1;
    line = 1;
};

bool Scanner::isLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}
bool Scanner::isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}
bool Scanner::isDigit16(char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

int Scanner::PPP() {
    pos = pos + 1;
    return pos;
}

void Scanner::getData(const char* FileName) {
    char aa;
    // Открываем входной файл для чтения
    FILE* in = fopen(FileName, "r");
    if (in == nullptr) {
        // Если файл не найден, выводим ошибку и завершаем выполнение программы
        printError("Отсутствует входной файл");
        exit(1);
    }

    int i = 0;
    // Читаем файл до конца
    while (!feof(in)) {
        fscanf(in, "%c", &aa); // Читаем очередной символ
        if (!feof(in))
            t[i++] = aa; // Сохраняем символ в буфер t
        // Проверяем, не превышает ли размер буфера допустимый предел
        if (i >= MAX_TEXT - 1) {
            printError("Слишком большой размер исходного модуля");
            break;
        }
    }
    t[i] = '\0'; // Завершаем строку нулевым символом
    fclose(in); // Закрываем файл
}
void Scanner::getData(FILE* in)
{
    if (in == NULL)
    {
        printError("Отсутствует входной файл", "");
        exit(1);
    }
    int i = 0;
    char tmp;
    while (!feof(in))
    {
        fscanf(in, "%c", &tmp);
        if (!feof(in))
            t[i++] = tmp;
        if (i > MAX_TEXT)
        {
            printError("Слишком большой размер исходного модуля", "");
            break;
        }
    }
    t[i] = '\0';
    fclose(in);
}

void Scanner::printError(string error, TypeLex lex) {
    cerr << "Ошибка! Строка " << line << ", позиция " << pos << ": " << error << " ( " << lex << " )\n";
    exit(1);
}
void Scanner::printError(string errorMessage, string lexeme)
{
    if (lexeme[0] == 0)
        cout << "Ошибка: " << errorMessage << endl;
    else
        cout << "Строка " << line << ", Позиция " << pos << " Ошибка: " << errorMessage << ". Неверный символ: " << lexeme << endl;
    exit(0);
}
void Scanner::printError(const string& error) {
    cout << "Ошибка! " << error << "\n";
    exit(1);
}

void Scanner::printWarning(int typeErrors) {
    printf("Предупреждение! Строка %d, позиция %d. ", line, pos);
    switch (typeErrors) {
    case WLongId:
        printf("Слишком длинный идентификатор. Его длина будет ограничена %d символами\n", MAX_LEX);
        break;
    default:
        printf("Неизвестная ошибка %d\n", typeErrors);
        break;
    }
}

void Scanner::printError(int typeErrors, TypeLex lex) {
    printf("Ошибка! Строка %d, позиция %d. ", line, pos);
    switch (typeErrors) {
    case EWrongChar:
        printf("Недопустимый символ: '%s'\n", lex);
        break;
    case EConst10StartFrom0:
        printf("Константа из нескольких цифр начинается с нуля.\n");
        break;
    case ELongIntConst:
        printf("Константа слишком длинная.\n");
        break;
    case EbreakAfter0x:
        printf("Пробел после 0x!\n");
        break;
    default:
        printf("Неизвестная ошибка %d '%s'\n", typeErrors, lex);
        break;
    }
    exit(1);
}

int Scanner::scanner(TypeLex lex) {
    memset(lex, '\0', MAX_LEX); // Очищаем массив lex, чтобы записать новую лексему
    int i = 0;

    // Пропускаем пробелы, табуляции, переводы строк и однострочные комментарии
    while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t' || t[uk] == '/') {
        // Обрабатываем пробелы, табуляции и переводы строк
        while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t') {
            pos++; // Увеличиваем позицию
            if (t[uk] == '\n')
                newLine(); // Если это перевод строки, обновляем номер строки
            uk++; // Переходим к следующему символу
        }
        // Если это начало однострочного комментария, пропускаем его
        if ((t[uk] == '/') && (t[uk + 1] == '/')) {
            uk += 2; // Пропускаем "//"
            while (t[uk] != '\n' && t[uk] != '\0')
                uk++; // Пропускаем символы до конца строки
        }
        else {
            break; // Если это не комментарий, выходим из цикла
        }
    }

    // Проверяем конец текста
    if (t[uk] == '\0')
        return typeEnd; // Возвращаем специальный тип для обозначения конца текста

    // Обрабатываем идентификаторы и ключевые слова
    if (isLetter(t[uk])) {
        lex[i++] = t[uk++]; // Первый символ идентификатора
        pos++;
        // Считываем остальные символы идентификатора (буквы и цифры)
        while (isLetter(t[uk]) || isDigit(t[uk])) {
            if (i < MAX_LEX - 1) {
                lex[i++] = t[uk];
            }
            else if (i == MAX_LEX - 1) {
                i++;
                printWarning(WLongId); // Предупреждаем о слишком длинном идентификаторе
            }
            uk++; pos++;
        }
        // Сравниваем идентификатор с ключевыми словами
        for (int j = 0; j < MAX_NUM_KEY_WORD; j++) {
            if (strcmp(lex, keyWord[j]) == 0)
                return indexKeyWord[j]; // Возвращаем тип ключевого слова
        }
        return typeID; // Если это не ключевое слово, то это идентификатор
    }

    // Обрабатываем числовые константы
    else if (isDigit(t[uk])) {
        if (t[uk] == '0') {
            // Проверяем, является ли это шестнадцатеричной константой
            if (t[uk + 1] == 'x' || t[uk + 1] == 'X') {
                uk += 2; // Пропускаем "0x"
                // Считываем символы шестнадцатеричной константы
                if (isDigit16(t[uk])) {
                    while (isDigit16(t[uk])) {
                        lex[i++] = t[uk++];
                        pos++;
                        if (i == MAX_LEX) {
                            printError(ELongIntConst, lex); // Ошибка: слишком длинная константа
                            return typeError;
                        }
                    }
                }
                else if (t[uk] == ' ') {
                    printError(EbreakAfter0x, lex);
                }
                else { 
                    lex[i++] = t[uk++]; 
                    printError(EWrongChar, lex); // Ошибка: недопустимый символ
                    return typeError; 
                }

                if (i == 0) {
                    printError(EWrongChar, lex);
                    return typeError;
                }
                //printf("0x");
                return constHex; // Возвращаем тип шестнадцатеричной константы
            }

            // Проверяем недопустимые десятичные константы, начинающиеся с нуля
            if (isDigit(t[uk + 1])) {
                printError(EConst10StartFrom0, lex);
                return typeError;
            }
            lex[i++] = t[uk++];
            pos++;
            return constInt; // Возвращаем тип десятичной константы
        }
        else {
            // Считываем цифры десятичной константы
            while (isDigit(t[uk])) {
                lex[i++] = t[uk++];
                pos++;
                if (i == MAX_LEX) {
                    printError(ELongIntConst, lex); // Ошибка: слишком длинная константа
                    return typeError;
                }
            }
            return constInt; // Возвращаем тип десятичной константы
        }
    }

    // Специальные знаки
    else if (t[uk] == '.') {
        lex[i++] = t[uk++];
        pos++;
        return typeDot; // Возвращаем тип .
    }
    else if (t[uk] == ',') {
        lex[i++] = t[uk++];
        pos++;
        return typeComma; // Возвращаем тип ,
    }
    else if (t[uk] == ';') {
        lex[i++] = t[uk++];
        pos++;
        return typeSemicolon; // Возвращаем тип ;
    }
    else if (t[uk] == '(') {
        lex[i++] = t[uk++];
        pos++;
        return typeLeftBracket; // Возвращаем тип (

    }
    else if (t[uk] == ')') {
        lex[i++] = t[uk++];
        pos++;
        return typeRightBracket; // Возвращаем тип )

    }
    else if (t[uk] == '{') {
        lex[i++] = t[uk++];
        pos++;
        return typeLeftBrace; // Возвращаем тип {

    }
    else if (t[uk] == '}') {
        lex[i++] = t[uk++];
        pos++;
        return typeRightBrace; // Возвращаем тип }
    }
    
    // Знаки операций
    else if (t[uk] == '&') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '&') {
            lex[i++] = t[uk++];
            pos++;
            return typeAnd; // Возвращаем тип &&
        }
        else {
            printError(EWrongChar, lex);
            return typeError; // Ошибка: недопустимый символ
        }
        }
    else if (t[uk] == '|') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '|') {
            lex[i++] = t[uk++];
            pos++;
            return typeOr; // Возвращаем тип ||
        }
        else {
            printError(EWrongChar, lex);
            return typeError; // Ошибка: недопустимый символ
        }
    }

    else if (t[uk] == '-') {
        lex[i++] = t[uk++];
        pos++;
        return typeMinus; // Возвращаем тип -
    }
    else if (t[uk] == '+') {
        lex[i++] = t[uk++];
        pos++;
        return typePlus; // Возвращаем тип +
    }
    else if (t[uk] == '/') {
        lex[i++] = t[uk++];
        pos++;
        return typeDiv; // Возвращаем тип /
    }
    else if (t[uk] == '*') {
        lex[i++] = t[uk++];
        pos++;
        return typeMul; // Возвращаем тип *
    }
    else if (t[uk] == '%') {
        lex[i++] = t[uk++];
        pos++;
        return typeMod; // Возвращаем тип %
    }

    else if (t[uk] == '=') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeEq; // Возвращаем тип ==
        }
        else
            return typeEval; // Возвращаем тип =
    }
    else if (t[uk] == '!') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeUnEq; // Возвращаем тип !=
        }
        else
            return typeNot; // Возвращаем тип !
    }
    else if (t[uk] == '<') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeLessOrEq; // Возвращаем тип <=
        }
        else
            return typeLess; // Возвращаем тип <
    }
    else if (t[uk] == '>') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeMoreOrEq; // Возвращаем тип >=
        }
        else
            return typeMore; // Возвращаем тип >
    }
    
    // Если символ не распознан, выводим ошибку
    lex[i++] = t[uk++];
    pos++;
    printError(EWrongChar, lex);
    return typeError;
}

void Scanner::newLine() {
    line++;
    pos = 0;
}

void Scanner::setUK(int i) {
    uk = i;
    line = tmpLine;
    pos = tmpPos;
}

int Scanner::getUK() {
    tmpLine = line;
    tmpPos = pos;
    return uk;
}

int Scanner::getPos() {
    return pos;
}

void Scanner::setPos(int _pos) {
    pos = _pos;
}
int Scanner::getLine() {
    return line;
}

void Scanner::setLine(int _line) {
    line = _line;
}

void Scanner::putPointer(int i) {
    pointer = i;
}

int Scanner::getPointer() {
    return pointer;
}