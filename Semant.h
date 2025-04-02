#ifndef __SEMAN
#define __SEMAN
#include "Lexeme.h"

#define EMPTY -1

// ��������� ���� �������
enum OBJECT_TYPE {
	OBJ_EMPTY = -1,				// ������ ������
	OBJ_VAR,					// ����������
	OBJ_FUNCT,					// �������
	OBJ_CLASS,					// �����
	OBJ_BLOCK,					// ��������� �������� (���� ����)
	OBJ_FOR						// ���� for
};

// ��������� ���� ������
enum DATA_TYPE {
	TYPE_UNKNOWN = -1,			// �������������� ��� ������
	TYPE_INT,					// �������������
	TYPE_SHORT,					// ������������� ���������
	TYPE_LONG,					// ������������� �������
	TYPE_BOOL,					// �������
	TYPE_CLASS					// ��������� ������
};

// ��������� ���� ������
struct Node {

	// ������, ����� ��� ���� ����� ��������

	OBJECT_TYPE ObjectType;		// ��� �������
	DATA_TYPE DataType;			// ��� ������

	// ������������ ������ ��� ��������� ����� ��������

	TypeLex id;					// ������������� �������
	TypeLex ClassName;			// ��� ������ (���� ��� ������ � �����)
	int FlagConst;				// ������� ���������
	char* Data;					// ��������� �� ������ ��� NULL
	bool isInit;				// ���� �������������
	int Param;					// ���������� ���������� (��� �������)
};

// ����� ������
class Tree {

	Node* n;					// ������ ����
	Tree* Up, * Left, * Right;	// ������������ ����, ����� � ������ �������

public:
	static Tree* Cur;			// ������� ���� ������

	// ��������������� ������
	static Node* CreateEmpty();							// ������� ������ ���� ������
	static const char* getTypeName(DATA_TYPE dt);		// ���������� ��������� ������������� ���� ������

	// ������������
	Tree(Tree* l, Tree* r, Tree* u, Node* Data);		// ����������� � ������ ��������������
	Tree();												// ����������� �� ���������
	~Tree();											// ���������� ������

	// ������� ��������� ��������� ������
	void SetLeft(Node* Data);							// ������������� ����� ������� ����
	void SetRight(Node* Data);							// ������������� ������ ������� ����
	Tree* FindUp(Tree* From, TypeLex id);				// ����� ���� ����� �� ������
	Tree* FindUpFunct(Tree* From, TypeLex id);			// ����� ������� ����� �� ������
	Tree* FindRightLeft(Tree* From, TypeLex id);		// ����� ���� ������-�����
	Tree* FindUpOneLevel(Tree* From, TypeLex id);		// ����� ���� ����� �� ����� ������
	Tree* FindUp(TypeLex id);							// ����� ���� ����� �� ��������
	void Print(int def = 0);							// ������ ������
	void PrintNode(Node* n);							// ������ ����

	// ������������� ������������
	void SetCur(Tree* a);								// ��������� �������� ����
	Tree* GetCur(void);									// ��������� �������� ����
	Tree* SemInclude(DATA_TYPE data_t, OBJECT_TYPE obj_t, TypeLex a = nullptr, TypeLex className = nullptr); // ��������� ������ �������� � ������������� �������
	void SemSetType(Tree* Addr, DATA_TYPE t);			// ��������� ���� ������ ��� ������
	void SemSetId(Tree* Addr, TypeLex id);				// ��������� �������������� ��� ������
	void SemSetParam(Tree* Addr, int n);				// ��������� ���������� ����������
	void SemControlParam(Tree* Addr, int n);			// �������� ���������� ����������
	void SemSetInit(Tree* Addr);						// ��������� ����� �������������
	void SemCheckParTypes(Tree* Addr, DATA_TYPE* types);// �������� ����� ����������
	Tree* SemGetVar(TypeLex a);							// ����� ���������� �� ��������������
	Tree* SemGetFunct(TypeLex a);						// ����� ������� �� ��������������
	DATA_TYPE SemGetType(Tree* Addr);					// ��������� ���� ������ �� ������
	OBJECT_TYPE SemGetObjType(Tree* Addr);				// ��������� ���� ������� �� ������
	bool SemIsInit(Tree* Addr);							// �������� ������������� ����
	bool SemCheckRange(long long value, DATA_TYPE type);// �������� �������� �� ������������ ����
	static DATA_TYPE SemGetTypeExpr(DATA_TYPE dt1 = TYPE_UNKNOWN, DATA_TYPE dt2 = TYPE_UNKNOWN, int sign = 0);// ����������� ���� ���������
	int DupControl(Tree* Addr, TypeLex a);				// �������� ������������ ��������������
};
#endif

