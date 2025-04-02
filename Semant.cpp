#include "Semant.h"
#include "Lexeme.h"
#include "Scanner.h"
#define max(a, b) a < b ? b : a

//--------------------------------------
//	ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
//--------------------------------------

// конструктор создает узел с заданными связями и данными
Tree::Tree(Tree* l, Tree* r, Tree* u, Node* d) { 
	n = new Node();
	Up = u; Left = l; Right = r;				// установили ссылки
	memcpy(n, d, sizeof(Node));					// установили данные
}

// конструктор создает новый узел с пустыми связями и данными
Tree::Tree(void) {
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

Node* Tree::CreateEmpty() {
	
	// Создаем пустую вершину
	Node emptyNode;
	memcpy(emptyNode.id, "", 1);				// Пустой идентификатор
	emptyNode.ObjectType = OBJ_EMPTY;			// Тип объекта:			Empty
	emptyNode.DataType = TYPE_UNKNOWN;			// Тип данных:			Unknown
	emptyNode.Data = nullptr;					// Данные:				пустой указатель
	emptyNode.isInit = false;					// Инициализация:		false
	emptyNode.Param = 0;						// Кол-во параметров:	0
	emptyNode.ClassName[0] = '\0';				// Имя класса для типа: ""

	return &emptyNode;
}

const char* Tree::getTypeName(DATA_TYPE dt) {
	if (dt == TYPE_INT) return BLUE "TYPE_INT" RESET;
	if (dt == TYPE_SHORT) return BLUE "TYPE_SHORT" RESET;
	if (dt == TYPE_LONG) return BLUE "TYPE_LONG" RESET;
	if (dt == TYPE_BOOL) return BLUE "TYPE_BOOL" RESET;
	if (dt == TYPE_CLASS) return GREEN "TYPE_CLASS" RESET;
	return BLACK "TYPE_UNKNOWN" RESET;
}

// создать левого потомка от текущей вершины this
void Tree::SetLeft(Node* Data) {
	Tree* a = new Tree(NULL, NULL, this, Data); // новая вершина
	Left = a;									// связали this с новой вершиной
}

// создать правого потомка от текущей вершины this
void Tree::SetRight(Node* Data) {
	Tree* a = new Tree(NULL, NULL, this, Data); // новая вершина
	Right = a;									// связали this с новой вершиной
}

// поиск данных в дереве, начиная от текущей вершины this до его корня вверх по связям
Tree* Tree::FindUp(TypeLex id) {
	return FindUp(this, id);
}

// Поиск элемента id вверх по дереву от текущей вершины From.
// Поиск осуществляется на одном уровне вложенности по левым связям
Tree* Tree::FindUpOneLevel(Tree* From, TypeLex id) {
	Tree* i = From;								// текущая вершина поиска
	while ((i != NULL) && (i->Up != NULL) && (i->Up->Right != i)) {
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i;							// нaшли совпадающий идентификатор
		i = i->Up;								// поднимаемся наверх по связям
	}
	return NULL;
}

// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
Tree* Tree::FindUp(Tree* From, TypeLex id) {
	Tree* i = From;								// текущая вершина поиска
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up;								// поднимаемся наверх по связям
	return i;
}

// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
Tree* Tree::FindUpFunct(Tree* From, TypeLex id) {

	Tree* i = From;								// текущая вершина поиска
	Tree* j = NULL;

	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0)) {
		if (i->n->ObjectType == OBJ_CLASS) {
			j = FindRightLeft(i, id);
			if (j != NULL) return j;
		}
		i = i->Up;								// поднимаемся наверх по связям
	}
	return i;
}

// поиск прямых потомков текущей вершины this
//Tree* Tree::FindRightLeft(TypeLex id) {
//	return FindRightLeft(this, id);
//}

// поиск прямых потомков заданной вершины From
Tree* Tree::FindRightLeft(Tree* From, TypeLex id) {
	Tree* i = From->Right; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	// обходим только соседей по левым связям
	return i;
}

//void Tree::Print(int def) {
//	PrintNode(n);
//	if (Left != NULL || Right != NULL) printf(":   ");
//	else printf(" конечная");
//	if (Left != NULL) {
//		PrintNode(Left->n);
//		printf(" /");
//		printf("л");
//	}
//	if (Right != NULL) {
//		printf(" п");
//		printf("\\ ");
//		PrintNode(Right->n);
//	}
//	printf("\n");
//	if (Left != NULL) Left->Print();
//	if (Right != NULL) Right->Print();
//}

void Tree::Print(int def) {
	if (n->ObjectType != OBJ_EMPTY)for (int i = 0; i < def; i++) printf("\t");
	if(n->ObjectType != OBJ_EMPTY) PrintNode(n);
	if (Left != NULL || Right != NULL);
	else { if (n->ObjectType != OBJ_EMPTY) { printf(" конечная");  }printf("\n");
	}
	if (n->ObjectType != OBJ_EMPTY) printf("\n");
	if (Right != NULL) {   Right->Print(def + 1); }
	if (Left != NULL) {
		Left->Print(def);
	}
}

void Tree::PrintNode(Node* n) {
	if (n->ObjectType == OBJ_BLOCK) printf("<Вершина " BLACK "OBJ_BLOCK" RESET ">");
	else if (n->ObjectType == OBJ_CLASS) printf("<Вершина " GREEN "OBJ_CLASS" RESET " [id:" BLACK "%s" RESET "]>", n->id);
	else if (n->ObjectType == OBJ_EMPTY) printf("<Вершина " BLACK "OBJ_EMPTY" RESET ">");
	else if (n->ObjectType == OBJ_FOR) printf("<Вершина " BOLDMAGENTA "OBJ_FOR" RESET ">");
	else if (n->ObjectType == OBJ_FUNCT) printf("<Вершина " YELLOW "OBJ_FUNCT" RESET " [id:" BLACK "%s" RESET ", params:" BLACK "%d" RESET ", type:%s]>", n->id, n->Param, getTypeName(n->DataType));
	else if (n->ObjectType == OBJ_VAR) printf("<Вершина " BLUE "OBJ_VAR" RESET " [id:" BLACK "%s" RESET ", type:%s]>", n->id, getTypeName(n->DataType));
	else printf("<Вершина>");
}

//--------------------------------------
//	    СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
//--------------------------------------

Tree* Tree::Cur = (Tree*)NULL;

// установить текущий узел дерева
void Tree::SetCur(Tree* a) { Cur = a; }

// получить значение текущего узла дерева
Tree* Tree::GetCur(void) { return Cur; }


// занесение идентификатора a в таблицу с типом t
Tree* Tree::SemInclude(DATA_TYPE data_t, OBJECT_TYPE obj_t, TypeLex a, TypeLex className) {

	// Проверка на дублирование идентификатора
	if (a != nullptr && DupControl(Cur, a)) {
		printf(RED "Ошибка" RESET " (семантическая): Повторное описание идентификатора " BOLDBLACK "%s" RESET ".\n", a);
		return nullptr;							// или можно выбросить исключение
	}

	Tree* v;									// Возвращаемая вершина
	Node b;										// Новая вершина для добавления в дерево

	// Инициализация общей части узла
	b.ObjectType = obj_t;						// Устанавливаем тип объекта
	b.DataType = data_t;						// Устанавливаем тип данных
	b.Data = nullptr;							// Данные пока не инициализированы
	b.isInit = false;							
	b.Param = 0;								// Количество параметров (по умолчанию 0)

	// Если указан идентификатор, копируем его
	if (a != nullptr)
		memcpy(b.id, a, strlen(a) + 1);			// Копируем идентификатор
	else
		b.id[0] = '\0';							// идентификатор не указан

	// Если тип данных класс и указано имя класса, копируем его
	if (data_t == TYPE_CLASS && className != nullptr) {
		if (FindUp(Cur, className) == nullptr) { printf(RED "Ошибка" RESET " (семантическая): Идентификатор " BOLDBLACK "%s" RESET " не определен.\n", className); return nullptr;
		}
		memcpy(b.ClassName, className, strlen(className) + 1);
	}
	else
		b.ClassName[0] = '\0';					// Имя класса не указано

	// Обработка в зависимости от типа объекта
	if (obj_t == OBJ_VAR) {

		// Добавляем переменную
		Cur->SetLeft(&b);						// Создаем левую вершину
		Cur = Cur->Left;						// Переходим на новую вершину
		v = Cur;								// Возвращаем новую вершину
	}
	else if (obj_t == OBJ_FUNCT || obj_t == OBJ_CLASS || obj_t == OBJ_BLOCK || obj_t == OBJ_FOR) {

		// Добавляем
		Cur->SetLeft(&b);						// Создаем левую вершину
		Cur = Cur->Left;						// Переходим на новую вершину
		v = Cur;								// Это точка возврата после выхода из функции

		// Создаем пустую правую вершину для области функции
		Cur->SetRight(CreateEmpty());			// Создаем правую вершину
		Cur = Cur->Right;						// Переходим на новую вершину
	}
	else {
		// Неподдерживаемый тип объекта
		printf(RED "Ошибка" RESET " (семантическая): Неподдерживаемый тип объекта\n");
		return nullptr;
	}

	return v;
}

// установить тип t для переменной по адресу Addr
void Tree::SemSetType(Tree* Addr, DATA_TYPE t) {
	Addr->n->DataType = t;
}

// установить имя id для переменной по адресу Addr
void Tree::SemSetId(Tree* Addr, TypeLex id) {
	memcpy(Addr->n->id, id, strlen(id) + 1);
}

// установить число формальных параметров n для функции по адресу Addr
void Tree::SemSetParam(Tree* Addr, int num) {
	Addr->n->Param = num;
}

void Tree::SemSetInit(Tree* Addr) {
	Addr->n->isInit = true;
}

void Tree::SemCheckParTypes(Tree* Addr, DATA_TYPE* types) {

	int count = Addr->n->Param;

	if (count) {
		Addr = Addr->Right;
		Addr = Addr->Left;
	}

	for (int i = 0; i < count; i++) {
		if(*(types+i) != Addr->n->DataType) 
			printf(RED "Ошибка" RESET " (семантическая): Несоответствие типа параметра " BLUE "%s.\n" RESET, Addr->n->id);
		Addr = Addr->Left;
	}

}

// проверить равенство числа формальных параметров значению
// n для функции по адресу Addr
void Tree::SemControlParam(Tree* Addr, int num) {
	if (num != Addr->n->Param)
		printf(RED "Ошибка" RESET " (семантическая): Неверное число параметров у вызова функции " YELLOW "%s.\n" RESET, Addr->n->id);
}

// найти в таблице переменную с именем a
// и вернуть ссылку на соответствующий элемент дерева
DATA_TYPE Tree::SemGetType(Tree* Addr) {

	if (Addr == NULL) return TYPE_UNKNOWN;

	if (Addr->n->DataType == TYPE_UNKNOWN)
		printf(RED "Ошибка" RESET " (семантическая): Использование переменной " BLACK "%s" RESET" с неизвестным типом.\n", Addr->n->id);

	return Addr->n->DataType;
}

OBJECT_TYPE Tree::SemGetObjType(Tree* Addr) {

	if (Addr == NULL) return OBJ_EMPTY;

	return Addr->n->ObjectType;
}

// найти в таблице переменную с именем a
// и вернуть ссылку на соответствующий элемент дерева.
Tree* Tree::SemGetVar(TypeLex a) {

	Tree* v = FindUp(Cur, a);
	if (v == NULL)
		printf(RED "Ошибка" RESET " (семантическая): Идентификатор " BLACK "%s" RESET " не определен.\n", a);
	if (v != NULL && v->n->ObjectType != OBJ_VAR)
		printf(RED "Ошибка" RESET " (семантическая): Идентификатор " BLACK "%s" RESET" не является переменной.\n", a);
	return v;
}

Tree* Tree::SemGetFunct(TypeLex a) {

	Tree* v = FindUpFunct(Cur, a);
	if (v == NULL)
		printf(RED "Ошибка" RESET " (семантическая): Идентификатор " BLACK "%s" RESET " не определен.\n", a);
	if (v != NULL && v->n->ObjectType != OBJ_FUNCT)
		printf(RED "Ошибка" RESET " (семантическая): Идентификатор " BLACK "%s" RESET" не является функцией.\n", a);
	return v;
}

// найти в таблице переменную с именем a
// и вернуть ссылку на соответствующий элемент дерева.
bool Tree::SemIsInit(Tree* Addr) {

	if (Addr == NULL) return false;

	if (Addr->n->isInit == false)
		printf(RED "Ошибка" RESET " (семантическая): Использование неинициализированной переменной " BLACK "%s" RESET".\n", Addr->n->id);

	return Addr->n->isInit;
}

// Проверяет, помещается ли значение в диапазон указанного типа.
bool Tree::SemCheckRange(long long value, DATA_TYPE type) {

	if (type == TYPE_SHORT) return -32767 <= value && value <= 32767;
	if (type == TYPE_INT) return -2147483648 <= value && value <= 2147483647;
	if (type == TYPE_LONG) return -2147483648 <= value && value <= 2147483647;
	if (type == TYPE_BOOL) return 0 == value || value == 1;

	// Для TYPE_UNKNOWN, TYPE_CLASS и других типов диапазон не проверяется
	return false;
}


// Функция для получения типа DATA_TYPE результата операции.
DATA_TYPE Tree::SemGetTypeExpr(DATA_TYPE dt1, DATA_TYPE dt2, int sign) {

	// Логические операции всегда возвращают bool
	if (sign == typeAnd || sign == typeOr || sign == typeNot ||
		sign == typeEq || sign == typeUnEq || sign == typeLess ||
		sign == typeLessOrEq || sign == typeMore || sign == typeMoreOrEq) {
		return TYPE_BOOL;
	}

	// Арифметические операции возвращают тип, который является результатом приведения типов
	if (sign == typePlus || sign == typeMinus || sign == typeMul || sign == typeDiv || sign == typeMod) {
		// Если один из операндов имеет тип TYPE_LONG, результат будет TYPE_LONG
		if (dt1 == TYPE_LONG || dt2 == TYPE_LONG) {
			return TYPE_LONG;
		}
		// Если один из операндов имеет тип TYPE_INT, результат будет TYPE_INT
		if (dt1 == TYPE_INT || dt2 == TYPE_INT) {
			return TYPE_INT;
		}
		// Если один из операндов имеет тип TYPE_SHORT, результат будет TYPE_SHORT
		if (dt1 == TYPE_SHORT || dt2 == TYPE_SHORT) {
			return TYPE_SHORT;
		}
		// Если оба операнда имеют тип TYPE_BOOL, результат будет TYPE_INT (так как bool может быть приведен к int)
		if (dt1 == TYPE_BOOL && dt2 == TYPE_BOOL) {
			return TYPE_INT;
		}
	}

	// Операция присваивания возвращает тип левого операнда
	if (sign == typeEval) {

		if (dt1 == TYPE_INT) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_BOOL) return TYPE_INT;
			if (dt2 == TYPE_LONG) { 
				printf(RED "Ошибка" RESET " (семантическая): Нельзя присвоить значение типа %s переменной типа %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_INT;
			}
		}

		if (dt1 == TYPE_SHORT) {
			if (dt2 == TYPE_SHORT || dt2 == TYPE_BOOL) return TYPE_SHORT;
			if (dt2 == TYPE_INT) {
				printf(RED "Ошибка" RESET " (семантическая): Нельзя присвоить значение типа %s переменной типа %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_SHORT;
			}
			if (dt2 == TYPE_LONG) {
				printf(RED "Ошибка" RESET " (семантическая): Нельзя присвоить значение типа %s переменной типа %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_SHORT;
			}
		}

		if (dt1 == TYPE_LONG) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_BOOL || dt2 == TYPE_LONG) return TYPE_LONG;
		}

		if (dt1 == TYPE_BOOL) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_LONG) {
				printf(RED "Ошибка" RESET " (семантическая): Нельзя присвоить значение типа %s переменной типа %s.\n", getTypeName(dt2), getTypeName(dt1));
				return TYPE_BOOL;
			}
			if (dt2 == TYPE_BOOL) return TYPE_BOOL;
		}

		if (dt1 != dt2) printf(RED "Ошибка" RESET " (семантическая): Нельзя присвоить значение типа %s переменной типа %s.\n", getTypeName(dt2), getTypeName(dt1));
		else return dt1;
	}

	// Если операция не распознана, возвращаем TYPE_UNKNOWN
	return TYPE_UNKNOWN;
}



// Проверка идентификатора а на повторное описание внутри блока.
// Поиск осуществляется вверх от вершины Addr.
int Tree::DupControl(Tree* Addr, TypeLex a) {
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}
