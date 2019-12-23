#include<iostream>
using namespace std;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

typedef int ElemType;
typedef int Status;

typedef struct {
	ElemType * elem;
	int length;
	int listsize;
}SqList;

typedef SqList HeapType;

Status InitList_Sq(SqList &L) {	//创建线性顺序表
	L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L.elem) exit(OVERFLOW);
	L.length = 0;	//表长度
	L.listsize = LIST_INIT_SIZE;	//存储容量
	return OK;
}

Status ListInsert_Sq(SqList &L, int i, ElemType e) {	//插入元素
	ElemType * newbase;
	int *q;

	if ((i < 1) || (i > L.length + 1)) return ERROR;
	if (L.length >= L.listsize) {
		newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
		if (!newbase)exit(OVERFLOW);
		L.elem = newbase;
		L.listsize += LISTINCREMENT;
	}
	q = &(L.elem[i]);
	for (int *p = &(L.elem[L.length - 1]); p >= q; --p) {
		*(p + 1) = *p;
	   }
	*q = e;
	L.length++;
	return OK;
}

Status ListDelete_Sq(SqList &L, int i, ElemType &e) {	//删除元素
	int *p, *q;

	if ((i < 1) || (i > L.length)) return ERROR;
	p = &(L.elem[i - 1]);
	e = *p;
	q = L.elem + L.length - 1;
	for (++p; p <= q; ++p) {
		*(p - 1) = *p;
	}
	--L.length;
	return OK;
}

Status ClearList(SqList &L){	//将线性表置为空表
	if (!L.elem) return ERROR;
	L.length = 0;
	return OK;
}

void PrintList(SqList L) {
	for (int i = 1; i <= L.length; i++) {
		cout << L.elem[i]<<"  ";
	}
	cout << endl << endl;
}

void InsertSort(SqList &L) {	//直接插入排序
	int exch = 0;
	int cmp = 0;
	int j;
	for (int i = 2; i <= L.length; i++) {
		if (L.elem[i] < L.elem[i - 1]) {
			L.elem[0] = L.elem[i];
			L.elem[i] = L.elem[i - 1];
			exch++;
			cmp++;
			for (j = i - 2; L.elem[0] < L.elem[j]; --j) {
				cmp++;
				L.elem[j + 1] = L.elem[j];
				exch++;
			}
			L.elem[j + 1] = L.elem[0];
		}
	}
	cout << "直接插入排序的比较次数为：" << cmp << "   交换次数为：" << exch << endl;
}

void BInsertSort(SqList &L) {	//折半插入排序
	int exch = 0;
	int cmp = 0;
	int low, high, m, j;

	for (int i = 2; i <= L.length; i++) {
		cmp++;
		L.elem[0] = L.elem[i];
		exch++;
		low = 1; high = i - 1;
		while (low <= high) {
			cmp++;
			m = (low + high) / 2;
			if (L.elem[0] < L.elem[m]) {
				cmp++;
				high = m - 1;
			}
			else {
				low = m + 1;
			}
			exch++;
		}
		for (j = i - 1; j >= high + 1; --j) {
			L.elem[j + 1] = L.elem[j];
			exch++;
		}
		L.elem[high + 1] = L.elem[0];
		exch++;
	}
	cout << "折半插入排序的比较次数为：" << cmp << "   交换次数为：" << exch << endl;
}

int Partition(SqList &L,int low,int high, int &cmpQuick, int &exchQuick) {
	int privotkey;

	L.elem[0] = L.elem[low];
	privotkey = L.elem[low];
	exchQuick++;
	while (low < high) {
		cmpQuick++;
		while (low < high&&L.elem[high] >= privotkey) {
			cmpQuick++;
			--high;
		}
		L.elem[low] = L.elem[high];
		exchQuick++;
		while (low < high&&L.elem[low] <= privotkey) {
			cmpQuick++;
			++low;
		}
		L.elem[high] = L.elem[low];
		exchQuick++;
	}
	L.elem[low] = L.elem[0];
	exchQuick++;
	return low;
}

void QSort(SqList &L,int low,int high,int &cmpQuick,int &exchQuick) {
	int privotloc;
	if (low < high) {
		cmpQuick++;
		privotloc = Partition(L, low, high, cmpQuick, exchQuick);
		QSort(L, low, privotloc - 1, cmpQuick, exchQuick);
		QSort(L, privotloc + 1, high, cmpQuick, exchQuick);
	}
}

void QuickSort(SqList &L) {	//快速排序
	static int cmpQuick = 0, exchQuick = 0;

	QSort(L, 1, L.length, cmpQuick, exchQuick);
	cout << "快速排序的比较次数为：" << cmpQuick << "   交换次数为：" << exchQuick << endl;
}

void HeapAdjust(HeapType &H, int s, int m, int &cmpHeap, int &exchHeap) {	//调整为大顶堆
	int rc,j;

	rc = H.elem[s];
	exchHeap++;
	for (j = 2 * s; j <= m; j *= 2) {
		if ((j < m) && (H.elem[j] < H.elem[j + 1])) {
			cmpHeap++;
			++j;
		}
		if (!(rc < H.elem[j])) { 
			cmpHeap++;
			break;
		}
		H.elem[s] = H.elem[j];
		s = j;
		exchHeap++;
	}
	H.elem[s] = rc;
	exchHeap++;
}

void HeapSort(HeapType &H) {	//堆排序
	int t;
	static int cmpHeap = 0, exchHeap = 0;
	for (int i = H.length / 2; i > 0; --i) {
		HeapAdjust(H, i, H.length, cmpHeap, exchHeap);
	}
	for (int i = H.length; i > 1; --i) {
		t = H.elem[1];
		H.elem[1] = H.elem[i];
		H.elem[i] = t;
		exchHeap++;
		HeapAdjust(H, 1, i - 1, cmpHeap, exchHeap);
	}
	cout << "堆排序的比较次数为：" << cmpHeap << "   交换次数为：" << exchHeap << endl;
}

void Initial(SqList &L,int a[]) {
	ClearList(L);
	for (int i = 1; i <= 10; i++) {
		ListInsert_Sq(L, i, a[i-1]);
	}
}

int main() {
	int a[10] = {49,38,65,97,76,13,27,47,85,38};
	SqList L;
	InitList_Sq(L);

	Initial(L, a);
	InsertSort(L);
	PrintList(L);

	Initial(L, a);
	BInsertSort(L);
	PrintList(L);

	Initial(L, a);
	QuickSort(L);
	PrintList(L);

	Initial(L, a);
	HeapSort(L);
	PrintList(L);

	system("pause");
	return 0;
}