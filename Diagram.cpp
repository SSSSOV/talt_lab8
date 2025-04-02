// Diagram.cpp
#include "Diagram.h"
// Конструктор класса Diagram
Diagram::Diagram(Scanner* sc) : scanner(sc), currentToken(0), previousToken(0) {
    this->Root = new Tree();
    this->Root->SetCur(this->Root);
    Start = this->Root->GetCur();
}

void Diagram::pushHistory(int pos) {
    for (int i = 1; i < 5; i++) history[i - 1] = history[i];
    history[4] = pos;
}

int Diagram::popHistory() {
    int v = history[4];
    for (int i = 0; i < 4; i++) history[4 - i] = history[3 - i];
    history[0] = -1;
    return v;
}

// Обновление текущего токена
void Diagram::updateToken() {
    previousToken = currentToken;
    currentToken = scanner->scanner(currentLexeme);
    
    pushHistory(scanner->getUK());
}

// Восстановление предыдущего токена
void Diagram::recoverToken() {
    popHistory();
    scanner->setUK(popHistory());
    updateToken();
}

// Вывод сообщения об ошибке
void Diagram::printError(const std::string& message) {
    scanner->printError(message, currentLexeme);
}

// Разбор программы
void Diagram::parseProgram() {
    updateToken();
    while (currentToken != typeEnd) {
        if (currentToken == typeClass) {
            parseClass();
        }
        else if (currentToken == typeVoid) {
            updateToken();
            parseFunction();
        }
        else if (isDataType(currentToken)) {

            TypeLex lex;
            memcpy(lex, currentLexeme, strlen(currentLexeme) + 1);

            DATA_TYPE dt = parseType();
            if (previousToken == typeInt && currentToken == typeMain) {
                parseFunction();
                Root->SemSetType(Root->Cur, TYPE_INT);
            }
            else if (currentToken == typeID) {
                parseData(dt, lex);
            }
            else {
                printError("Ожидался 'main' или идентификатор переменной после типа данных.");
            }
        }
        else {
            printError("Ожидалось объявление класса, функции или переменной в начале программы.");
        }
    }
    Root->SetCur(Start);
    //printf("\n");
    Root->Print();
}

// Проверка, является ли токен типом данных
bool Diagram::isDataType(int token) {
    return token == typeInt || token == typeShort || token == typeLong ||
        token == typeBool || token == typeID;
}

// Разбор типа данных
DATA_TYPE Diagram::parseType() {
    if (!isDataType(currentToken)) {
        printError("Ожидался тип данных (int, short, long, bool) или идентификатор перед объявлением переменной или функции.");
    }

    if (currentToken == typeInt) {
        updateToken(); return TYPE_INT;
    }
    if (currentToken == typeLong) {
        updateToken(); return TYPE_LONG;
    }
    if (currentToken == typeShort) {
        updateToken(); return TYPE_SHORT;
    }
    if (currentToken == typeBool) {
        updateToken(); return TYPE_BOOL;
    }
    if (currentToken == typeID) {
        updateToken(); return TYPE_CLASS;
    }
    return TYPE_UNKNOWN;
}

// Разбор функции
void Diagram::parseFunction() {
    if (currentToken == typeID || currentToken == typeMain) {
        Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_FUNCT, currentLexeme);
        int i = 0;
        updateToken();
        if (currentToken != typeLeftBracket) {
            printError("Ожидался символ '(' после имени функции.");
        }
        updateToken();
        if (isDataType(currentToken)) {
            i = parseArguments();
        }
        if (currentToken != typeRightBracket) {
            printError("Ожидался символ ')' для завершения списка аргументов функции.");
        }
        updateToken();
        Root->SemSetParam(v, i);
        parseCompoundStatement();
        Root->SetCur(v);
    }
    else {
        printError("Ожидалось имя функции или ключевое слово 'main'.");
    }
}

// Разбор класса
void Diagram::parseClass() {
    updateToken();
    if (currentToken != typeID) {
        printError("Ожидалось имя класса после ключевого слова 'class'.");
    }
    Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_CLASS, currentLexeme);
    updateToken();
    if (currentToken != typeLeftBrace) {
        printError("Ожидался символ '{' для начала тела класса после имени класса.");
    }
    updateToken();
    while (currentToken != typeRightBrace) {
        if (currentToken == typeVoid) {
            updateToken();
            parseFunction();
        }
        else {
            printError("Функции в классе должны быть объявлены с типом 'void'.");
        }
    }
    updateToken();
    if (currentToken != typeSemicolon) {
        printError("Ожидался символ ';' для завершения объявления класса.");
    }
    updateToken();
    if(v!=nullptr) Root->SetCur(v);
}

// Разбор аргументов функции
int Diagram::parseArguments() {
    int i = 0;
    DATA_TYPE type = parseType();
    if (currentToken != typeID) {
        printError("Ожидалось имя аргумента после указания его типа.");
    }
    Root->SemInclude(type, OBJ_VAR, currentLexeme);
    Root->SemSetInit(Root->Cur);
    i++;
    updateToken();

    while (currentToken == typeComma) {
        updateToken();
        type = parseType();
        if (currentToken != typeID) {
            printError("Ожидалось имя аргумента после указания его типа.");
        }
        Root->SemInclude(type, OBJ_VAR, currentLexeme);
        Root->SemSetInit(Root->Cur);
        i++;
        updateToken();
    }
    return i;
}
int Diagram::parseParametrs(Tree* func) {

    int count = 0;
    DATA_TYPE* par_types = new DATA_TYPE[100];

    while (true) {

        *(par_types+count) = parseExpression();
        count++;

        if (currentToken == typeComma)
            updateToken();
        else {
            Root->SemCheckParTypes(func, par_types);
            return count;
        }
    }
}

// Разбор данных
void Diagram::parseData(DATA_TYPE dt, TypeLex lex) {
    do {
        if (currentToken != typeID) {
            printError("Ожидалось имя переменной после указания типа данных.");
        }
        Root->SemInclude(dt, OBJ_VAR, currentLexeme, lex);
        updateToken();
        if (currentToken == typeEval) {
            updateToken();
            DATA_TYPE dt2 = parseExpression(); // Тип правой части выражения
            DATA_TYPE dt1 = Root->SemGetType(Root->Cur); // Тип левой части (переменной)

            Root->SemGetTypeExpr(dt1, dt2, typeEval);
            Root->SemSetInit(Root->Cur);
        }
        if (currentToken == typeComma) {
            updateToken();
        }
        else if (currentToken == typeSemicolon) {
            updateToken();
            return;
        }
        else {
            printError("Ожидался ',' для перечисления переменных или ';' для завершения объявления.");
        }
    } while (true);
}

// Разбор составного оператора
void Diagram::parseCompoundStatement() {
    if (currentToken != typeLeftBrace) {
        printError("Ожидался символ '{' для начала тела составного оператора.");
    }
    updateToken();
    while (currentToken != typeRightBrace) {

        if (isDataType(currentToken)) {
            updateToken();
            if (currentToken != typeID) {
                recoverToken(); recoverToken(); recoverToken();
                updateToken();

                parseOperator();
            }
            else {
                recoverToken(); recoverToken(); recoverToken();
                updateToken();

                TypeLex lex;
                memcpy(lex, currentLexeme, strlen(currentLexeme) + 1);

                DATA_TYPE dt = parseType();

                if (currentToken == typeID) {
                    parseData(dt, lex);
                }
                else {
                    printError("Ожидался идентификатор переменной после типа данных.");
                }
            }
        }
        else parseOperator();
    }
    updateToken();
}

// Разбор оператора
void Diagram::parseOperator(bool skipNode) {
    if (currentToken == typeSemicolon) {
        updateToken();
    }
    else if (currentToken == typeID) {
        parseAssignmentOrFunctionCall();
    }
    else if (currentToken == typeFor) {
        parseFor();
    }
    else if (currentToken == typeLeftBrace) {
        if (skipNode == false) {
            Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_BLOCK);
            parseCompoundStatement();
            Root->SetCur(v);
        }
        else parseCompoundStatement();
    }
    else {
        printError("Ожидался оператор (присваивание, вызов функции или составной оператор).");
    }
}

// Разбор выражения
DATA_TYPE Diagram::parseExpression() {
    DATA_TYPE dt1 = parseAnd();
    while (currentToken == typeOr) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseAnd();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор логического И
DATA_TYPE Diagram::parseAnd() {
    DATA_TYPE dt1 = parseEquality();
    while (currentToken == typeAnd) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseEquality();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор операторов равенства
DATA_TYPE Diagram::parseEquality() {
    DATA_TYPE dt1 = parseRelational();
    while (currentToken == typeEq || currentToken == typeUnEq) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseRelational();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор реляционных операторов
DATA_TYPE Diagram::parseRelational() {
    DATA_TYPE dt1 = parseAdditive();
    while (currentToken == typeLess || currentToken == typeLessOrEq ||
        currentToken == typeMore || currentToken == typeMoreOrEq) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseAdditive();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор сложения и вычитания
DATA_TYPE Diagram::parseAdditive() {
    DATA_TYPE dt1 = parseMultiplicative();
    while (currentToken == typePlus || currentToken == typeMinus) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseMultiplicative();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор умножения, деления и остатка от деления
DATA_TYPE Diagram::parseMultiplicative() {
    DATA_TYPE dt1 = parseUnary();
    while (currentToken == typeMul || currentToken == typeDiv || currentToken == typeMod) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt2 = parseUnary();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // Определяем тип результата
    }
    return dt1;
}

// Разбор унарных операторов
DATA_TYPE Diagram::parseUnary() {
    if (currentToken == typePlus || currentToken == typeMinus || currentToken == typeNot) {
        int op = currentToken; // Сохраняем текущий оператор
        updateToken();
        DATA_TYPE dt = parseElementary();
        return Root->SemGetTypeExpr(dt, TYPE_UNKNOWN, op); // Определяем тип результата для унарной операции
    }
    return parseElementary();
}

// Разбор элементарных выражений
DATA_TYPE Diagram::parseElementary() {

    DATA_TYPE dt = TYPE_UNKNOWN;

    if (currentToken == typeID) {

        Tree* v = Root->SemGetVar(currentLexeme);
        Root->SemIsInit(v);
        dt = Root->SemGetType(v);
        updateToken();
    }
    else if (currentToken == constInt) {
        // Обработка целочисленной константы (10-ричной)
        dt = TYPE_INT; // Целочисленные константы по умолчанию имеют тип TYPE_INT
        updateToken();
    }
    else if (currentToken == constHex) {
        // Обработка целочисленной константы (16-ричной)
        dt = TYPE_INT; // Шестнадцатеричные константы также имеют тип TYPE_INT
        updateToken();
    }
    else if (currentToken == typeTrue || currentToken == typeFalse) {
        // Обработка логических констант (true/false)
        dt = TYPE_BOOL; // Логические константы имеют тип TYPE_BOOL
        updateToken();
    }
    else if (currentToken == typeLeftBracket) {
        // Обработка вложенного выражения в скобках
        updateToken();
        dt = parseExpression(); // Рекурсивный разбор выражения в скобках
        if (currentToken != typeRightBracket) {
            printError("Ожидался символ ')' для завершения вложенного выражения.");
        }
        updateToken();
    }
    else {
        printError("Ожидалось элементарное выражение, например, идентификатор, константа или логическое значение.");
    }
    return dt;
}

// Разбор вызова функции или присваивания
void Diagram::parseAssignmentOrFunctionCall() {

    int idCount = parseName();

    if (currentToken == typeLeftBracket) {

        // Разбор вызова функции
        recoverToken();
        recoverToken();
        Tree* v = Root->SemGetFunct(currentLexeme);
        updateToken();

        int i = 0;
        updateToken();
        if (currentToken != typeRightBracket)
            i = parseParametrs(v);

        Root->SemControlParam(v, i);

        if (currentToken != typeRightBracket)
            printError("Ожидался символ ')' для завершения вызова функции.");
        updateToken();

        if (currentToken != typeSemicolon)
            printError("Ожидался символ ';' после вызова функции.");
        updateToken();
    }
    else if (currentToken == typeEval) {

        // Разбор присваивания
        recoverToken();
        recoverToken();
        Tree* v = Root->SemGetVar(currentLexeme);
        DATA_TYPE dt1;
        updateToken();

        updateToken();
        DATA_TYPE dt2 = parseExpression(); // Тип правой части выражения
        if (v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) dt1 = Root->SemGetType(v); // Тип левой части (переменной)

        if(v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) Root->SemGetTypeExpr(dt1, dt2, typeEval);

        if (currentToken != typeSemicolon) {
            printError("Ожидался символ ';' после выражения присваивания.");
        }
        if (v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) Root->SemSetInit(v);
        updateToken();
    }
    else {
        printError("Ожидалось '=' для присваивания или '(' для вызова функции.");
    }
}

int Diagram::parseName() {

    int i = 0;
    if (currentToken != typeID)
        scanner->printError("Ожидалось имя.", currentLexeme);
    updateToken();
    i++;
    while (currentToken == typeDot) {

        updateToken();
        if (currentToken != typeID)
            scanner->printError("Ожидался индентификатор.", currentLexeme);
        updateToken();
        i++;
    }
    return i;
}

void Diagram::parseFor() {

    if (currentToken != typeFor)
        scanner->printError("Ожидался оператор 'for'.", currentLexeme);

    Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_FOR);

    updateToken();
    if (currentToken != typeLeftBracket)
        scanner->printError("Ожидался символ '(' начала условия цикла.", currentLexeme);

    updateToken();

    if (isDataType(currentToken)) {
        updateToken();

        if (currentToken == typeEval) {
            recoverToken(); recoverToken(); recoverToken();
            updateToken();

            parseAssignment();

            if (currentToken != typeSemicolon)
                scanner->printError("Ожидался символ ';' в условии цикла.", currentLexeme);

            updateToken();
        }
        else if (currentToken == typeID) {
            recoverToken(); recoverToken(); recoverToken();
            updateToken();

            TypeLex lex;
            memcpy(lex, currentLexeme, strlen(currentLexeme) + 1);

            DATA_TYPE dt = parseType();

            if (currentToken == typeID) {
                parseData(dt, lex);
            }
            else printError("Ожидался идентификатор переменной после типа данных.");
        }
        else printError("Ожидалось '=' или название переменной для for.");
    } 
    else printError("Ожидалась инициализация или присваивание для for.");

    parseExpression();
    if (currentToken != typeSemicolon)
        scanner->printError("Ожидался символ ';' в условии цикла.", currentLexeme);

    updateToken();
    parseAssignment();
    if (currentToken != typeRightBracket)
        scanner->printError("Ожидался символ ')' конца условия цикла.", currentLexeme);

    updateToken();
    parseOperator(true);
    Root->SetCur(v);
}

void Diagram::parseAssignment() {

    if (currentToken != typeID) {
        scanner->printError("Ожидалось название переменной.", currentLexeme);
    }
    parseName();

    if (currentToken != typeEval) {
        scanner->printError("Ожидался символ присваивания '='.", currentLexeme);
    }

    updateToken();
    parseExpression();
}