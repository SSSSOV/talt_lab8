#include "Semant.h"
#include "Lexeme.h"
#include "Scanner.h"
#define max(a, b) a < b ? b : a

//--------------------------------------
//	������� ��������� ��������� ������
//--------------------------------------

// ����������� ������� ���� � ��������� ������� � �������
Tree::Tree(Tree* l, Tree* r, Tree* u, Node* d) { 
	n = new Node();
	Up = u; Left = l; Right = r;				// ���������� ������
	memcpy(n, d, sizeof(Node));					// ���������� ������
}

// ����������� ������� ����� ���� � ������� ������� � �������
Tree::Tree(void) {
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

Node* Tree::CreateEmpty() {
	
	// ������� ������ �������
	Node emptyNode;
	memcpy(emptyNode.id, "", 1);				// ������ �������������
	emptyNode.ObjectType = OBJ_EMPTY;			// ��� �������:			Empty
	emptyNode.DataType = TYPE_UNKNOWN;			// ��� ������:			Unknown
	emptyNode.Data = nullptr;					// ������:				������ ���������
	emptyNode.isInit = false;					// �������������:		false
	emptyNode.Param = 0;						// ���-�� ����������:	0
	emptyNode.ClassName[0] = '\0';				// ��� ������ ��� ����: ""

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

// ������� ������ ������� �� ������� ������� this
void Tree::SetLeft(Node* Data) {
	Tree* a = new Tree(NULL, NULL, this, Data); // ����� �������
	Left = a;									// ������� this � ����� ��������
}

// ������� ������� ������� �� ������� ������� this
void Tree::SetRight(Node* Data) {
	Tree* a = new Tree(NULL, NULL, this, Data); // ����� �������
	Right = a;									// ������� this � ����� ��������
}

// ����� ������ � ������, ������� �� ������� ������� this �� ��� ����� ����� �� ������
Tree* Tree::FindUp(TypeLex id) {
	return FindUp(this, id);
}

// ����� �������� id ����� �� ������ �� ������� ������� From.
// ����� �������������� �� ����� ������ ����������� �� ����� ������
Tree* Tree::FindUpOneLevel(Tree* From, TypeLex id) {
	Tree* i = From;								// ������� ������� ������
	while ((i != NULL) && (i->Up != NULL) && (i->Up->Right != i)) {
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i;							// �a��� ����������� �������������
		i = i->Up;								// ����������� ������ �� ������
	}
	return NULL;
}

// ����� ������ � ������ �� �������� ������� From
// �� ��� ����� ����� �� ������
Tree* Tree::FindUp(Tree* From, TypeLex id) {
	Tree* i = From;								// ������� ������� ������
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up;								// ����������� ������ �� ������
	return i;
}

// ����� ������ � ������ �� �������� ������� From
// �� ��� ����� ����� �� ������
Tree* Tree::FindUpFunct(Tree* From, TypeLex id) {

	Tree* i = From;								// ������� ������� ������
	Tree* j = NULL;

	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0)) {
		if (i->n->ObjectType == OBJ_CLASS) {
			j = FindRightLeft(i, id);
			if (j != NULL) return j;
		}
		i = i->Up;								// ����������� ������ �� ������
	}
	return i;
}

// ����� ������ �������� ������� ������� this
//Tree* Tree::FindRightLeft(TypeLex id) {
//	return FindRightLeft(this, id);
//}

// ����� ������ �������� �������� ������� From
Tree* Tree::FindRightLeft(Tree* From, TypeLex id) {
	Tree* i = From->Right; // ������� ������� ������
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	// ������� ������ ������� �� ����� ������
	return i;
}

//void Tree::Print(int def) {
//	PrintNode(n);
//	if (Left != NULL || Right != NULL) printf(":   ");
//	else printf(" ��������");
//	if (Left != NULL) {
//		PrintNode(Left->n);
//		printf(" /");
//		printf("�");
//	}
//	if (Right != NULL) {
//		printf(" �");
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
	else { if (n->ObjectType != OBJ_EMPTY) { printf(" ��������");  }printf("\n");
	}
	if (n->ObjectType != OBJ_EMPTY) printf("\n");
	if (Right != NULL) {   Right->Print(def + 1); }
	if (Left != NULL) {
		Left->Print(def);
	}
}

void Tree::PrintNode(Node* n) {
	if (n->ObjectType == OBJ_BLOCK) printf("<������� " BLACK "OBJ_BLOCK" RESET ">");
	else if (n->ObjectType == OBJ_CLASS) printf("<������� " GREEN "OBJ_CLASS" RESET " [id:" BLACK "%s" RESET "]>", n->id);
	else if (n->ObjectType == OBJ_EMPTY) printf("<������� " BLACK "OBJ_EMPTY" RESET ">");
	else if (n->ObjectType == OBJ_FOR) printf("<������� " BOLDMAGENTA "OBJ_FOR" RESET ">");
	else if (n->ObjectType == OBJ_FUNCT) printf("<������� " YELLOW "OBJ_FUNCT" RESET " [id:" BLACK "%s" RESET ", params:" BLACK "%d" RESET ", type:%s]>", n->id, n->Param, getTypeName(n->DataType));
	else if (n->ObjectType == OBJ_VAR) printf("<������� " BLUE "OBJ_VAR" RESET " [id:" BLACK "%s" RESET ", type:%s]>", n->id, getTypeName(n->DataType));
	else printf("<�������>");
}

//--------------------------------------
//	    ������������� ������������
//--------------------------------------

Tree* Tree::Cur = (Tree*)NULL;

// ���������� ������� ���� ������
void Tree::SetCur(Tree* a) { Cur = a; }

// �������� �������� �������� ���� ������
Tree* Tree::GetCur(void) { return Cur; }


// ��������� �������������� a � ������� � ����� t
Tree* Tree::SemInclude(DATA_TYPE data_t, OBJECT_TYPE obj_t, TypeLex a, TypeLex className) {

	// �������� �� ������������ ��������������
	if (a != nullptr && DupControl(Cur, a)) {
		printf(RED "������" RESET " (�������������): ��������� �������� �������������� " BOLDBLACK "%s" RESET ".\n", a);
		return nullptr;							// ��� ����� ��������� ����������
	}

	Tree* v;									// ������������ �������
	Node b;										// ����� ������� ��� ���������� � ������

	// ������������� ����� ����� ����
	b.ObjectType = obj_t;						// ������������� ��� �������
	b.DataType = data_t;						// ������������� ��� ������
	b.Data = nullptr;							// ������ ���� �� ����������������
	b.isInit = false;							
	b.Param = 0;								// ���������� ���������� (�� ��������� 0)

	// ���� ������ �������������, �������� ���
	if (a != nullptr)
		memcpy(b.id, a, strlen(a) + 1);			// �������� �������������
	else
		b.id[0] = '\0';							// ������������� �� ������

	// ���� ��� ������ ����� � ������� ��� ������, �������� ���
	if (data_t == TYPE_CLASS && className != nullptr) {
		if (FindUp(Cur, className) == nullptr) { printf(RED "������" RESET " (�������������): ������������� " BOLDBLACK "%s" RESET " �� ���������.\n", className); return nullptr;
		}
		memcpy(b.ClassName, className, strlen(className) + 1);
	}
	else
		b.ClassName[0] = '\0';					// ��� ������ �� �������

	// ��������� � ����������� �� ���� �������
	if (obj_t == OBJ_VAR) {

		// ��������� ����������
		Cur->SetLeft(&b);						// ������� ����� �������
		Cur = Cur->Left;						// ��������� �� ����� �������
		v = Cur;								// ���������� ����� �������
	}
	else if (obj_t == OBJ_FUNCT || obj_t == OBJ_CLASS || obj_t == OBJ_BLOCK || obj_t == OBJ_FOR) {

		// ���������
		Cur->SetLeft(&b);						// ������� ����� �������
		Cur = Cur->Left;						// ��������� �� ����� �������
		v = Cur;								// ��� ����� �������� ����� ������ �� �������

		// ������� ������ ������ ������� ��� ������� �������
		Cur->SetRight(CreateEmpty());			// ������� ������ �������
		Cur = Cur->Right;						// ��������� �� ����� �������
	}
	else {
		// ���������������� ��� �������
		printf(RED "������" RESET " (�������������): ���������������� ��� �������\n");
		return nullptr;
	}

	return v;
}

// ���������� ��� t ��� ���������� �� ������ Addr
void Tree::SemSetType(Tree* Addr, DATA_TYPE t) {
	Addr->n->DataType = t;
}

// ���������� ��� id ��� ���������� �� ������ Addr
void Tree::SemSetId(Tree* Addr, TypeLex id) {
	memcpy(Addr->n->id, id, strlen(id) + 1);
}

// ���������� ����� ���������� ���������� n ��� ������� �� ������ Addr
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
			printf(RED "������" RESET " (�������������): �������������� ���� ��������� " BLUE "%s.\n" RESET, Addr->n->id);
		Addr = Addr->Left;
	}

}

// ��������� ��������� ����� ���������� ���������� ��������
// n ��� ������� �� ������ Addr
void Tree::SemControlParam(Tree* Addr, int num) {
	if (num != Addr->n->Param)
		printf(RED "������" RESET " (�������������): �������� ����� ���������� � ������ ������� " YELLOW "%s.\n" RESET, Addr->n->id);
}

// ����� � ������� ���������� � ������ a
// � ������� ������ �� ��������������� ������� ������
DATA_TYPE Tree::SemGetType(Tree* Addr) {

	if (Addr == NULL) return TYPE_UNKNOWN;

	if (Addr->n->DataType == TYPE_UNKNOWN)
		printf(RED "������" RESET " (�������������): ������������� ���������� " BLACK "%s" RESET" � ����������� �����.\n", Addr->n->id);

	return Addr->n->DataType;
}

OBJECT_TYPE Tree::SemGetObjType(Tree* Addr) {

	if (Addr == NULL) return OBJ_EMPTY;

	return Addr->n->ObjectType;
}

// ����� � ������� ���������� � ������ a
// � ������� ������ �� ��������������� ������� ������.
Tree* Tree::SemGetVar(TypeLex a) {

	Tree* v = FindUp(Cur, a);
	if (v == NULL)
		printf(RED "������" RESET " (�������������): ������������� " BLACK "%s" RESET " �� ���������.\n", a);
	if (v != NULL && v->n->ObjectType != OBJ_VAR)
		printf(RED "������" RESET " (�������������): ������������� " BLACK "%s" RESET" �� �������� ����������.\n", a);
	return v;
}

Tree* Tree::SemGetFunct(TypeLex a) {

	Tree* v = FindUpFunct(Cur, a);
	if (v == NULL)
		printf(RED "������" RESET " (�������������): ������������� " BLACK "%s" RESET " �� ���������.\n", a);
	if (v != NULL && v->n->ObjectType != OBJ_FUNCT)
		printf(RED "������" RESET " (�������������): ������������� " BLACK "%s" RESET" �� �������� ��������.\n", a);
	return v;
}

// ����� � ������� ���������� � ������ a
// � ������� ������ �� ��������������� ������� ������.
bool Tree::SemIsInit(Tree* Addr) {

	if (Addr == NULL) return false;

	if (Addr->n->isInit == false)
		printf(RED "������" RESET " (�������������): ������������� �������������������� ���������� " BLACK "%s" RESET".\n", Addr->n->id);

	return Addr->n->isInit;
}

// ���������, ���������� �� �������� � �������� ���������� ����.
bool Tree::SemCheckRange(long long value, DATA_TYPE type) {

	if (type == TYPE_SHORT) return -32767 <= value && value <= 32767;
	if (type == TYPE_INT) return -2147483648 <= value && value <= 2147483647;
	if (type == TYPE_LONG) return -2147483648 <= value && value <= 2147483647;
	if (type == TYPE_BOOL) return 0 == value || value == 1;

	// ��� TYPE_UNKNOWN, TYPE_CLASS � ������ ����� �������� �� �����������
	return false;
}


// ������� ��� ��������� ���� DATA_TYPE ���������� ��������.
DATA_TYPE Tree::SemGetTypeExpr(DATA_TYPE dt1, DATA_TYPE dt2, int sign) {

	// ���������� �������� ������ ���������� bool
	if (sign == typeAnd || sign == typeOr || sign == typeNot ||
		sign == typeEq || sign == typeUnEq || sign == typeLess ||
		sign == typeLessOrEq || sign == typeMore || sign == typeMoreOrEq) {
		return TYPE_BOOL;
	}

	// �������������� �������� ���������� ���, ������� �������� ����������� ���������� �����
	if (sign == typePlus || sign == typeMinus || sign == typeMul || sign == typeDiv || sign == typeMod) {
		// ���� ���� �� ��������� ����� ��� TYPE_LONG, ��������� ����� TYPE_LONG
		if (dt1 == TYPE_LONG || dt2 == TYPE_LONG) {
			return TYPE_LONG;
		}
		// ���� ���� �� ��������� ����� ��� TYPE_INT, ��������� ����� TYPE_INT
		if (dt1 == TYPE_INT || dt2 == TYPE_INT) {
			return TYPE_INT;
		}
		// ���� ���� �� ��������� ����� ��� TYPE_SHORT, ��������� ����� TYPE_SHORT
		if (dt1 == TYPE_SHORT || dt2 == TYPE_SHORT) {
			return TYPE_SHORT;
		}
		// ���� ��� �������� ����� ��� TYPE_BOOL, ��������� ����� TYPE_INT (��� ��� bool ����� ���� �������� � int)
		if (dt1 == TYPE_BOOL && dt2 == TYPE_BOOL) {
			return TYPE_INT;
		}
	}

	// �������� ������������ ���������� ��� ������ ��������
	if (sign == typeEval) {

		if (dt1 == TYPE_INT) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_BOOL) return TYPE_INT;
			if (dt2 == TYPE_LONG) { 
				printf(RED "������" RESET " (�������������): ������ ��������� �������� ���� %s ���������� ���� %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_INT;
			}
		}

		if (dt1 == TYPE_SHORT) {
			if (dt2 == TYPE_SHORT || dt2 == TYPE_BOOL) return TYPE_SHORT;
			if (dt2 == TYPE_INT) {
				printf(RED "������" RESET " (�������������): ������ ��������� �������� ���� %s ���������� ���� %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_SHORT;
			}
			if (dt2 == TYPE_LONG) {
				printf(RED "������" RESET " (�������������): ������ ��������� �������� ���� %s ���������� ���� %s.\n", getTypeName(dt2), getTypeName(dt1)); 
				return TYPE_SHORT;
			}
		}

		if (dt1 == TYPE_LONG) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_BOOL || dt2 == TYPE_LONG) return TYPE_LONG;
		}

		if (dt1 == TYPE_BOOL) {
			if (dt2 == TYPE_INT || dt2 == TYPE_SHORT || dt2 == TYPE_LONG) {
				printf(RED "������" RESET " (�������������): ������ ��������� �������� ���� %s ���������� ���� %s.\n", getTypeName(dt2), getTypeName(dt1));
				return TYPE_BOOL;
			}
			if (dt2 == TYPE_BOOL) return TYPE_BOOL;
		}

		if (dt1 != dt2) printf(RED "������" RESET " (�������������): ������ ��������� �������� ���� %s ���������� ���� %s.\n", getTypeName(dt2), getTypeName(dt1));
		else return dt1;
	}

	// ���� �������� �� ����������, ���������� TYPE_UNKNOWN
	return TYPE_UNKNOWN;
}



// �������� �������������� � �� ��������� �������� ������ �����.
// ����� �������������� ����� �� ������� Addr.
int Tree::DupControl(Tree* Addr, TypeLex a) {
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}
