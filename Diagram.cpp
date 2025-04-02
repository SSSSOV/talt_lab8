// Diagram.cpp
#include "Diagram.h"
// ����������� ������ Diagram
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

// ���������� �������� ������
void Diagram::updateToken() {
    previousToken = currentToken;
    currentToken = scanner->scanner(currentLexeme);
    
    pushHistory(scanner->getUK());
}

// �������������� ����������� ������
void Diagram::recoverToken() {
    popHistory();
    scanner->setUK(popHistory());
    updateToken();
}

// ����� ��������� �� ������
void Diagram::printError(const std::string& message) {
    scanner->printError(message, currentLexeme);
}

// ������ ���������
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
                printError("�������� 'main' ��� ������������� ���������� ����� ���� ������.");
            }
        }
        else {
            printError("��������� ���������� ������, ������� ��� ���������� � ������ ���������.");
        }
    }
    Root->SetCur(Start);
    //printf("\n");
    Root->Print();
}

// ��������, �������� �� ����� ����� ������
bool Diagram::isDataType(int token) {
    return token == typeInt || token == typeShort || token == typeLong ||
        token == typeBool || token == typeID;
}

// ������ ���� ������
DATA_TYPE Diagram::parseType() {
    if (!isDataType(currentToken)) {
        printError("�������� ��� ������ (int, short, long, bool) ��� ������������� ����� ����������� ���������� ��� �������.");
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

// ������ �������
void Diagram::parseFunction() {
    if (currentToken == typeID || currentToken == typeMain) {
        Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_FUNCT, currentLexeme);
        int i = 0;
        updateToken();
        if (currentToken != typeLeftBracket) {
            printError("�������� ������ '(' ����� ����� �������.");
        }
        updateToken();
        if (isDataType(currentToken)) {
            i = parseArguments();
        }
        if (currentToken != typeRightBracket) {
            printError("�������� ������ ')' ��� ���������� ������ ���������� �������.");
        }
        updateToken();
        Root->SemSetParam(v, i);
        parseCompoundStatement();
        Root->SetCur(v);
    }
    else {
        printError("��������� ��� ������� ��� �������� ����� 'main'.");
    }
}

// ������ ������
void Diagram::parseClass() {
    updateToken();
    if (currentToken != typeID) {
        printError("��������� ��� ������ ����� ��������� ����� 'class'.");
    }
    Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_CLASS, currentLexeme);
    updateToken();
    if (currentToken != typeLeftBrace) {
        printError("�������� ������ '{' ��� ������ ���� ������ ����� ����� ������.");
    }
    updateToken();
    while (currentToken != typeRightBrace) {
        if (currentToken == typeVoid) {
            updateToken();
            parseFunction();
        }
        else {
            printError("������� � ������ ������ ���� ��������� � ����� 'void'.");
        }
    }
    updateToken();
    if (currentToken != typeSemicolon) {
        printError("�������� ������ ';' ��� ���������� ���������� ������.");
    }
    updateToken();
    if(v!=nullptr) Root->SetCur(v);
}

// ������ ���������� �������
int Diagram::parseArguments() {
    int i = 0;
    DATA_TYPE type = parseType();
    if (currentToken != typeID) {
        printError("��������� ��� ��������� ����� �������� ��� ����.");
    }
    Root->SemInclude(type, OBJ_VAR, currentLexeme);
    Root->SemSetInit(Root->Cur);
    i++;
    updateToken();

    while (currentToken == typeComma) {
        updateToken();
        type = parseType();
        if (currentToken != typeID) {
            printError("��������� ��� ��������� ����� �������� ��� ����.");
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

// ������ ������
void Diagram::parseData(DATA_TYPE dt, TypeLex lex) {
    do {
        if (currentToken != typeID) {
            printError("��������� ��� ���������� ����� �������� ���� ������.");
        }
        Root->SemInclude(dt, OBJ_VAR, currentLexeme, lex);
        updateToken();
        if (currentToken == typeEval) {
            updateToken();
            DATA_TYPE dt2 = parseExpression(); // ��� ������ ����� ���������
            DATA_TYPE dt1 = Root->SemGetType(Root->Cur); // ��� ����� ����� (����������)

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
            printError("�������� ',' ��� ������������ ���������� ��� ';' ��� ���������� ����������.");
        }
    } while (true);
}

// ������ ���������� ���������
void Diagram::parseCompoundStatement() {
    if (currentToken != typeLeftBrace) {
        printError("�������� ������ '{' ��� ������ ���� ���������� ���������.");
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
                    printError("�������� ������������� ���������� ����� ���� ������.");
                }
            }
        }
        else parseOperator();
    }
    updateToken();
}

// ������ ���������
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
        printError("�������� �������� (������������, ����� ������� ��� ��������� ��������).");
    }
}

// ������ ���������
DATA_TYPE Diagram::parseExpression() {
    DATA_TYPE dt1 = parseAnd();
    while (currentToken == typeOr) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseAnd();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ ����������� �
DATA_TYPE Diagram::parseAnd() {
    DATA_TYPE dt1 = parseEquality();
    while (currentToken == typeAnd) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseEquality();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ ���������� ���������
DATA_TYPE Diagram::parseEquality() {
    DATA_TYPE dt1 = parseRelational();
    while (currentToken == typeEq || currentToken == typeUnEq) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseRelational();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ ����������� ����������
DATA_TYPE Diagram::parseRelational() {
    DATA_TYPE dt1 = parseAdditive();
    while (currentToken == typeLess || currentToken == typeLessOrEq ||
        currentToken == typeMore || currentToken == typeMoreOrEq) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseAdditive();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ �������� � ���������
DATA_TYPE Diagram::parseAdditive() {
    DATA_TYPE dt1 = parseMultiplicative();
    while (currentToken == typePlus || currentToken == typeMinus) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseMultiplicative();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ ���������, ������� � ������� �� �������
DATA_TYPE Diagram::parseMultiplicative() {
    DATA_TYPE dt1 = parseUnary();
    while (currentToken == typeMul || currentToken == typeDiv || currentToken == typeMod) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt2 = parseUnary();
        dt1 = Root->SemGetTypeExpr(dt1, dt2, op); // ���������� ��� ����������
    }
    return dt1;
}

// ������ ������� ����������
DATA_TYPE Diagram::parseUnary() {
    if (currentToken == typePlus || currentToken == typeMinus || currentToken == typeNot) {
        int op = currentToken; // ��������� ������� ��������
        updateToken();
        DATA_TYPE dt = parseElementary();
        return Root->SemGetTypeExpr(dt, TYPE_UNKNOWN, op); // ���������� ��� ���������� ��� ������� ��������
    }
    return parseElementary();
}

// ������ ������������ ���������
DATA_TYPE Diagram::parseElementary() {

    DATA_TYPE dt = TYPE_UNKNOWN;

    if (currentToken == typeID) {

        Tree* v = Root->SemGetVar(currentLexeme);
        Root->SemIsInit(v);
        dt = Root->SemGetType(v);
        updateToken();
    }
    else if (currentToken == constInt) {
        // ��������� ������������� ��������� (10-������)
        dt = TYPE_INT; // ������������� ��������� �� ��������� ����� ��� TYPE_INT
        updateToken();
    }
    else if (currentToken == constHex) {
        // ��������� ������������� ��������� (16-������)
        dt = TYPE_INT; // ����������������� ��������� ����� ����� ��� TYPE_INT
        updateToken();
    }
    else if (currentToken == typeTrue || currentToken == typeFalse) {
        // ��������� ���������� �������� (true/false)
        dt = TYPE_BOOL; // ���������� ��������� ����� ��� TYPE_BOOL
        updateToken();
    }
    else if (currentToken == typeLeftBracket) {
        // ��������� ���������� ��������� � �������
        updateToken();
        dt = parseExpression(); // ����������� ������ ��������� � �������
        if (currentToken != typeRightBracket) {
            printError("�������� ������ ')' ��� ���������� ���������� ���������.");
        }
        updateToken();
    }
    else {
        printError("��������� ������������ ���������, ��������, �������������, ��������� ��� ���������� ��������.");
    }
    return dt;
}

// ������ ������ ������� ��� ������������
void Diagram::parseAssignmentOrFunctionCall() {

    int idCount = parseName();

    if (currentToken == typeLeftBracket) {

        // ������ ������ �������
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
            printError("�������� ������ ')' ��� ���������� ������ �������.");
        updateToken();

        if (currentToken != typeSemicolon)
            printError("�������� ������ ';' ����� ������ �������.");
        updateToken();
    }
    else if (currentToken == typeEval) {

        // ������ ������������
        recoverToken();
        recoverToken();
        Tree* v = Root->SemGetVar(currentLexeme);
        DATA_TYPE dt1;
        updateToken();

        updateToken();
        DATA_TYPE dt2 = parseExpression(); // ��� ������ ����� ���������
        if (v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) dt1 = Root->SemGetType(v); // ��� ����� ����� (����������)

        if(v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) Root->SemGetTypeExpr(dt1, dt2, typeEval);

        if (currentToken != typeSemicolon) {
            printError("�������� ������ ';' ����� ��������� ������������.");
        }
        if (v != nullptr && Root->SemGetObjType(v) == OBJ_VAR) Root->SemSetInit(v);
        updateToken();
    }
    else {
        printError("��������� '=' ��� ������������ ��� '(' ��� ������ �������.");
    }
}

int Diagram::parseName() {

    int i = 0;
    if (currentToken != typeID)
        scanner->printError("��������� ���.", currentLexeme);
    updateToken();
    i++;
    while (currentToken == typeDot) {

        updateToken();
        if (currentToken != typeID)
            scanner->printError("�������� ��������������.", currentLexeme);
        updateToken();
        i++;
    }
    return i;
}

void Diagram::parseFor() {

    if (currentToken != typeFor)
        scanner->printError("�������� �������� 'for'.", currentLexeme);

    Tree* v = Root->SemInclude(TYPE_UNKNOWN, OBJ_FOR);

    updateToken();
    if (currentToken != typeLeftBracket)
        scanner->printError("�������� ������ '(' ������ ������� �����.", currentLexeme);

    updateToken();

    if (isDataType(currentToken)) {
        updateToken();

        if (currentToken == typeEval) {
            recoverToken(); recoverToken(); recoverToken();
            updateToken();

            parseAssignment();

            if (currentToken != typeSemicolon)
                scanner->printError("�������� ������ ';' � ������� �����.", currentLexeme);

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
            else printError("�������� ������������� ���������� ����� ���� ������.");
        }
        else printError("��������� '=' ��� �������� ���������� ��� for.");
    } 
    else printError("��������� ������������� ��� ������������ ��� for.");

    parseExpression();
    if (currentToken != typeSemicolon)
        scanner->printError("�������� ������ ';' � ������� �����.", currentLexeme);

    updateToken();
    parseAssignment();
    if (currentToken != typeRightBracket)
        scanner->printError("�������� ������ ')' ����� ������� �����.", currentLexeme);

    updateToken();
    parseOperator(true);
    Root->SetCur(v);
}

void Diagram::parseAssignment() {

    if (currentToken != typeID) {
        scanner->printError("��������� �������� ����������.", currentLexeme);
    }
    parseName();

    if (currentToken != typeEval) {
        scanner->printError("�������� ������ ������������ '='.", currentLexeme);
    }

    updateToken();
    parseExpression();
}