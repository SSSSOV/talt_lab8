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
    // ��������� ������� ���� ��� ������
    FILE* in = fopen(FileName, "r");
    if (in == nullptr) {
        // ���� ���� �� ������, ������� ������ � ��������� ���������� ���������
        printError("����������� ������� ����");
        exit(1);
    }

    int i = 0;
    // ������ ���� �� �����
    while (!feof(in)) {
        fscanf(in, "%c", &aa); // ������ ��������� ������
        if (!feof(in))
            t[i++] = aa; // ��������� ������ � ����� t
        // ���������, �� ��������� �� ������ ������ ���������� ������
        if (i >= MAX_TEXT - 1) {
            printError("������� ������� ������ ��������� ������");
            break;
        }
    }
    t[i] = '\0'; // ��������� ������ ������� ��������
    fclose(in); // ��������� ����
}
void Scanner::getData(FILE* in)
{
    if (in == NULL)
    {
        printError("����������� ������� ����", "");
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
            printError("������� ������� ������ ��������� ������", "");
            break;
        }
    }
    t[i] = '\0';
    fclose(in);
}

void Scanner::printError(string error, TypeLex lex) {
    cerr << "������! ������ " << line << ", ������� " << pos << ": " << error << " ( " << lex << " )\n";
    exit(1);
}
void Scanner::printError(string errorMessage, string lexeme)
{
    if (lexeme[0] == 0)
        cout << "������: " << errorMessage << endl;
    else
        cout << "������ " << line << ", ������� " << pos << " ������: " << errorMessage << ". �������� ������: " << lexeme << endl;
    exit(0);
}
void Scanner::printError(const string& error) {
    cout << "������! " << error << "\n";
    exit(1);
}

void Scanner::printWarning(int typeErrors) {
    printf("��������������! ������ %d, ������� %d. ", line, pos);
    switch (typeErrors) {
    case WLongId:
        printf("������� ������� �������������. ��� ����� ����� ���������� %d ���������\n", MAX_LEX);
        break;
    default:
        printf("����������� ������ %d\n", typeErrors);
        break;
    }
}

void Scanner::printError(int typeErrors, TypeLex lex) {
    printf("������! ������ %d, ������� %d. ", line, pos);
    switch (typeErrors) {
    case EWrongChar:
        printf("������������ ������: '%s'\n", lex);
        break;
    case EConst10StartFrom0:
        printf("��������� �� ���������� ���� ���������� � ����.\n");
        break;
    case ELongIntConst:
        printf("��������� ������� �������.\n");
        break;
    case EbreakAfter0x:
        printf("������ ����� 0x!\n");
        break;
    default:
        printf("����������� ������ %d '%s'\n", typeErrors, lex);
        break;
    }
    exit(1);
}

int Scanner::scanner(TypeLex lex) {
    memset(lex, '\0', MAX_LEX); // ������� ������ lex, ����� �������� ����� �������
    int i = 0;

    // ���������� �������, ���������, �������� ����� � ������������ �����������
    while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t' || t[uk] == '/') {
        // ������������ �������, ��������� � �������� �����
        while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t') {
            pos++; // ����������� �������
            if (t[uk] == '\n')
                newLine(); // ���� ��� ������� ������, ��������� ����� ������
            uk++; // ��������� � ���������� �������
        }
        // ���� ��� ������ ������������� �����������, ���������� ���
        if ((t[uk] == '/') && (t[uk + 1] == '/')) {
            uk += 2; // ���������� "//"
            while (t[uk] != '\n' && t[uk] != '\0')
                uk++; // ���������� ������� �� ����� ������
        }
        else {
            break; // ���� ��� �� �����������, ������� �� �����
        }
    }

    // ��������� ����� ������
    if (t[uk] == '\0')
        return typeEnd; // ���������� ����������� ��� ��� ����������� ����� ������

    // ������������ �������������� � �������� �����
    if (isLetter(t[uk])) {
        lex[i++] = t[uk++]; // ������ ������ ��������������
        pos++;
        // ��������� ��������� ������� �������������� (����� � �����)
        while (isLetter(t[uk]) || isDigit(t[uk])) {
            if (i < MAX_LEX - 1) {
                lex[i++] = t[uk];
            }
            else if (i == MAX_LEX - 1) {
                i++;
                printWarning(WLongId); // ������������� � ������� ������� ��������������
            }
            uk++; pos++;
        }
        // ���������� ������������� � ��������� �������
        for (int j = 0; j < MAX_NUM_KEY_WORD; j++) {
            if (strcmp(lex, keyWord[j]) == 0)
                return indexKeyWord[j]; // ���������� ��� ��������� �����
        }
        return typeID; // ���� ��� �� �������� �����, �� ��� �������������
    }

    // ������������ �������� ���������
    else if (isDigit(t[uk])) {
        if (t[uk] == '0') {
            // ���������, �������� �� ��� ����������������� ����������
            if (t[uk + 1] == 'x' || t[uk + 1] == 'X') {
                uk += 2; // ���������� "0x"
                // ��������� ������� ����������������� ���������
                if (isDigit16(t[uk])) {
                    while (isDigit16(t[uk])) {
                        lex[i++] = t[uk++];
                        pos++;
                        if (i == MAX_LEX) {
                            printError(ELongIntConst, lex); // ������: ������� ������� ���������
                            return typeError;
                        }
                    }
                }
                else if (t[uk] == ' ') {
                    printError(EbreakAfter0x, lex);
                }
                else { 
                    lex[i++] = t[uk++]; 
                    printError(EWrongChar, lex); // ������: ������������ ������
                    return typeError; 
                }

                if (i == 0) {
                    printError(EWrongChar, lex);
                    return typeError;
                }
                //printf("0x");
                return constHex; // ���������� ��� ����������������� ���������
            }

            // ��������� ������������ ���������� ���������, ������������ � ����
            if (isDigit(t[uk + 1])) {
                printError(EConst10StartFrom0, lex);
                return typeError;
            }
            lex[i++] = t[uk++];
            pos++;
            return constInt; // ���������� ��� ���������� ���������
        }
        else {
            // ��������� ����� ���������� ���������
            while (isDigit(t[uk])) {
                lex[i++] = t[uk++];
                pos++;
                if (i == MAX_LEX) {
                    printError(ELongIntConst, lex); // ������: ������� ������� ���������
                    return typeError;
                }
            }
            return constInt; // ���������� ��� ���������� ���������
        }
    }

    // ����������� �����
    else if (t[uk] == '.') {
        lex[i++] = t[uk++];
        pos++;
        return typeDot; // ���������� ��� .
    }
    else if (t[uk] == ',') {
        lex[i++] = t[uk++];
        pos++;
        return typeComma; // ���������� ��� ,
    }
    else if (t[uk] == ';') {
        lex[i++] = t[uk++];
        pos++;
        return typeSemicolon; // ���������� ��� ;
    }
    else if (t[uk] == '(') {
        lex[i++] = t[uk++];
        pos++;
        return typeLeftBracket; // ���������� ��� (

    }
    else if (t[uk] == ')') {
        lex[i++] = t[uk++];
        pos++;
        return typeRightBracket; // ���������� ��� )

    }
    else if (t[uk] == '{') {
        lex[i++] = t[uk++];
        pos++;
        return typeLeftBrace; // ���������� ��� {

    }
    else if (t[uk] == '}') {
        lex[i++] = t[uk++];
        pos++;
        return typeRightBrace; // ���������� ��� }
    }
    
    // ����� ��������
    else if (t[uk] == '&') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '&') {
            lex[i++] = t[uk++];
            pos++;
            return typeAnd; // ���������� ��� &&
        }
        else {
            printError(EWrongChar, lex);
            return typeError; // ������: ������������ ������
        }
        }
    else if (t[uk] == '|') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '|') {
            lex[i++] = t[uk++];
            pos++;
            return typeOr; // ���������� ��� ||
        }
        else {
            printError(EWrongChar, lex);
            return typeError; // ������: ������������ ������
        }
    }

    else if (t[uk] == '-') {
        lex[i++] = t[uk++];
        pos++;
        return typeMinus; // ���������� ��� -
    }
    else if (t[uk] == '+') {
        lex[i++] = t[uk++];
        pos++;
        return typePlus; // ���������� ��� +
    }
    else if (t[uk] == '/') {
        lex[i++] = t[uk++];
        pos++;
        return typeDiv; // ���������� ��� /
    }
    else if (t[uk] == '*') {
        lex[i++] = t[uk++];
        pos++;
        return typeMul; // ���������� ��� *
    }
    else if (t[uk] == '%') {
        lex[i++] = t[uk++];
        pos++;
        return typeMod; // ���������� ��� %
    }

    else if (t[uk] == '=') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeEq; // ���������� ��� ==
        }
        else
            return typeEval; // ���������� ��� =
    }
    else if (t[uk] == '!') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeUnEq; // ���������� ��� !=
        }
        else
            return typeNot; // ���������� ��� !
    }
    else if (t[uk] == '<') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeLessOrEq; // ���������� ��� <=
        }
        else
            return typeLess; // ���������� ��� <
    }
    else if (t[uk] == '>') {
        lex[i++] = t[uk++];
        pos++;
        if (t[uk] == '=') {
            lex[i++] = t[uk++];
            pos++;
            return typeMoreOrEq; // ���������� ��� >=
        }
        else
            return typeMore; // ���������� ��� >
    }
    
    // ���� ������ �� ���������, ������� ������
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