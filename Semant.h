#ifndef __SEMAN
#define __SEMAN
#include "Lexeme.h"

#define EMPTY -1

// Структура типа объекта
enum OBJECT_TYPE {
	OBJ_EMPTY = -1,				// Пустой объект
	OBJ_VAR,					// Переменная
	OBJ_FUNCT,					// Функция
	OBJ_CLASS,					// Класс
	OBJ_BLOCK,					// Составной оператор (блок кода)
	OBJ_FOR						// Цикл for
};

// Структура типа данных
enum DATA_TYPE {
	TYPE_UNKNOWN = -1,			// Неопределенный тип данных
	TYPE_INT,					// Целочисленный
	TYPE_SHORT,					// Целочисленное маленькое
	TYPE_LONG,					// Целочисленное большое
	TYPE_BOOL,					// Булевое
	TYPE_CLASS					// Экземпляр класса
};

// Структура узла дерева
struct Node {

	// Данные, общие для всех типов объектов

	OBJECT_TYPE ObjectType;		// Тип объекта
	DATA_TYPE DataType;			// Тип данных

	// Обязательные данные для некоторых типов объектов

	TypeLex id;					// Идентификатор объекта
	TypeLex ClassName;			// Имя класса (если тип данных — класс)
	int FlagConst;				// Признак константы
	char* Data;					// Указатель на данные или NULL
	bool isInit;				// Флаг инициализации
	int Param;					// Количество параметров (для функций)
};

// Класс дерева
class Tree {

	Node* n;					// Данные узла
	Tree* Up, * Left, * Right;	// Родительский узел, левый и правый потомки

public:
	static Tree* Cur;			// Текущий узел дерева

	// Вспомогательные методы
	static Node* CreateEmpty();							// Создает пустой узел дерева
	static const char* getTypeName(DATA_TYPE dt);		// Возвращает строковое представление типа данных

	// Конструкторы
	Tree(Tree* l, Tree* r, Tree* u, Node* Data);		// Конструктор с полной инициализацией
	Tree();												// Конструктор по умолчанию
	~Tree();											// Деструктор дерева

	// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
	void SetLeft(Node* Data);							// Устанавливает левый потомок узла
	void SetRight(Node* Data);							// Устанавливает правый потомок узла
	Tree* FindUp(Tree* From, TypeLex id);				// Поиск узла вверх по дереву
	Tree* FindUpFunct(Tree* From, TypeLex id);			// Поиск функции вверх по дереву
	Tree* FindRightLeft(Tree* From, TypeLex id);		// Поиск узла вправо-влево
	Tree* FindUpOneLevel(Tree* From, TypeLex id);		// Поиск узла вверх на одном уровне
	Tree* FindUp(TypeLex id);							// Поиск узла вверх от текущего
	void Print(int def = 0);							// Печать дерева
	void PrintNode(Node* n);							// Печать узла

	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
	void SetCur(Tree* a);								// Установка текущего узла
	Tree* GetCur(void);									// Получение текущего узла
	Tree* SemInclude(DATA_TYPE data_t, OBJECT_TYPE obj_t, TypeLex a = nullptr, TypeLex className = nullptr); // Включение нового элемента в семантическую таблицу
	void SemSetType(Tree* Addr, DATA_TYPE t);			// Установка типа данных для адреса
	void SemSetId(Tree* Addr, TypeLex id);				// Установка идентификатора для адреса
	void SemSetParam(Tree* Addr, int n);				// Установка количества параметров
	void SemControlParam(Tree* Addr, int n);			// Контроль количества параметров
	void SemSetInit(Tree* Addr);						// Установка флага инициализации
	void SemCheckParTypes(Tree* Addr, DATA_TYPE* types);// Проверка типов параметров
	Tree* SemGetVar(TypeLex a);							// Поиск переменной по идентификатору
	Tree* SemGetFunct(TypeLex a);						// Поиск функции по идентификатору
	DATA_TYPE SemGetType(Tree* Addr);					// Получение типа данных по адресу
	OBJECT_TYPE SemGetObjType(Tree* Addr);				// Получение типа объекта по адресу
	bool SemIsInit(Tree* Addr);							// Проверка инициализации узла
	bool SemCheckRange(long long value, DATA_TYPE type);// Проверка значения на соответствие типу
	static DATA_TYPE SemGetTypeExpr(DATA_TYPE dt1 = TYPE_UNKNOWN, DATA_TYPE dt2 = TYPE_UNKNOWN, int sign = 0);// Определение типа выражения
	int DupControl(Tree* Addr, TypeLex a);				// Контроль дублирования идентификатора
};
#endif

