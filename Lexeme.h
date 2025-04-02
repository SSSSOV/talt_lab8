// Lexeme.h
#ifndef COMPC___DEFS_H
#define COMPC___DEFS_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

// Максимальная длина исходного модуля
#define MAX_TEXT 10000
// Максимальная длина лексемы
#define MAX_LEX 20
// Максимальное количество ключевых слов
#define MAX_NUM_KEY_WORD 20

// ------------------------------ Таблица лексем --------------------------------

#define typeID 1

//Ключевые слова
#define typeInt				10	// int 
#define typeShort			11	// short
#define typeLong			12	// long
#define typeBool			13	// bool
#define typeFor				14	// for
#define typeMain			15	// main
#define typeClass			16	// class
#define typeVoid			17	// void
#define typeTrue			18	// true
#define typeFalse			19	// false

//Константы
#define constInt			20	// Целые константы
#define constHex			21	// Шестнадцатеричные константы
#define constBool			22	// Логическая константа

//Специальные знаки
#define typeComma			30	// ,
#define typeSemicolon		31	// ;
#define typeLeftBracket		32	// (
#define typeRightBracket	33	// )
#define typeLeftBrace		34	// {
#define typeRightBrace		35	// }
#define typeDot				36	// .

//Знаки операций
#define typeEval			40	// =
#define typeUnEq			41	// !=
#define typeAnd				42	// &&
#define typeOr				43	// ||
#define typeEq				44	// ==
#define typeLessOrEq		45	// <=
#define typeLess			46	// <
#define typeMoreOrEq		47	// >=
#define typeMore			48	// >
#define typeMinus			49	// -
#define typePlus			50	// +
#define typeDiv				51	// /
#define typeMul				52	// *
#define typeMod				53	// %
#define typeNot				54	// !

//Конец исходного модуля
#define typeEnd				100
//Ошибочный символ
#define typeError			200

// -------------------------- Ошибки и предупреждения ---------------------------

// Типы ошибок
#define EWrongChar			1
#define EConst10StartFrom0	2
#define ELongIntConst		3
#define ELongHexConst		4
#define EbreakAfter0x		5

//Типы предупреждений
#define WLongId				20

typedef char TypeMod[MAX_TEXT];
typedef char TypeLex[MAX_LEX];

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#endif //COMPC___DEFS_H
