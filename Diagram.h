// Diagram.h
#ifndef DIAGRAM_H
#define DIAGRAM_H

#include "Scanner.h"
#include "Lexeme.h"
#include "Semant.h"
#include <iostream>

class Diagram {
private:
    Scanner* scanner;
    Tree* Root;
    Tree* Start;
    TypeLex currentLexeme;
    int currentToken;
    int previousToken;

    int history[5] = {-1, -1, -1, -1, -1};

    void pushHistory(int pos);
    int popHistory();

    void updateToken();
    void recoverToken();
    void printError(const std::string& message);

public:
    explicit Diagram(Scanner* sc);
    ~Diagram() = default;

    bool isDataType(int token);
    void parseProgram();
    int parseArguments();
    int parseParametrs(Tree* func);
    DATA_TYPE parseType();
    void parseFunction();
    void parseClass();
    void parseData(DATA_TYPE dt, TypeLex lex = nullptr);
    void parseCompoundStatement();
    void parseAssignment();
    DATA_TYPE parseExpression();
    int parseName();
    void parseFor();
    void parseOperator(bool skipNode = false);
    DATA_TYPE parseAnd();
    DATA_TYPE parseEquality();
    DATA_TYPE parseRelational();
    DATA_TYPE parseAdditive();
    DATA_TYPE parseMultiplicative();
    DATA_TYPE parseUnary();
    DATA_TYPE parseElementary();
    void parseAssignmentOrFunctionCall();
};

#endif DIAGRAM_H