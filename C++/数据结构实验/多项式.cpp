#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1  
#define OVERFLOW -2

typedef struct		//项的表示，多项式的项作为LinkList的数据元素
{
	float coef; //系数
	int expn;   //指数
}term, ElemType;//两个类型名：term用于本ADT，ElemType为LinkList的数据对象名 

typedef struct LNode		//节点类型，包含数据以及指向下一个节点的指针
{
	ElemType data;
	struct LNode *next;
}LNode, *Link, *Position;

typedef struct				//链表类型
{
	Link head, tail;	//头节点与尾节点
	int len;
}LinkList;

typedef LinkList polynomial;			//用带头结点的有序链表表示多项式
typedef int Status;						//将Status定义为int型

Status InitList(polynomial &L) {		//构造一个空的链表
	Link p;
	p = (Link)malloc(sizeof(LNode));
	if (p)
	{
		p->next = NULL;
		L.head = L.tail = p;
		L.len = 0;
		return OK;
	}
	else {
		return ERROR;
	}
}

Status MakeNode(Link &p, ElemType e) {	//分配由p指向的值为e的节点
	p = (Link)malloc(sizeof(LNode));	//分配新结点  
	if (!p) return ERROR;				//分配失败  
	p->data = e;						//用e给节点数据赋值
	return OK;
}

Position PriorPos(LinkList L, Link p) {	//已知p指向L中的一个节点，返回p所指节点的直接前驱的位置
	Link q;
	q = L.head;
	while (q->next != p) {
		q = q->next;
	}
	return q;
}

Position NextPos(LinkList L,Link p){	//已知p指向L中的一个节点，返回p所指节点的直接后继的位置
	return p->next;
}

Status ClearList(LinkList &L) {			//将线性表L置为空表
	Link p, q;//两个指向节点的指针

	if (L.head != L.tail) {
		p = q = L.head->next;
		L.head->next = NULL;

		while (p != L.tail) {//遍历
			p = q->next;
			free(q);
			q = p;
		}
		free(q);

		L.tail = L.head;
		L.len = 0;
	}
	return OK;
}

void FreeNode(Link &p) {				//释放p所指节点并使p为空
	free(p);
	p = NULL;
}

ElemType GetCurElem(Link p) {			//已知p指向线性链表中的一个节点，返回p所指向节点的值
	return p->data;
}

Status ListEmpty(LinkList L) {			//判断链表是否为空表
	return L.len == 0;
}

Status SetCurElem(Link &p, ElemType e) {//已知p指向线性链表中的一个节点，用e更新p所指向节点的值
	if (p) {
		p->data = e;
		return OK;
	}
	else {
		return FALSE;
	}
}

Position GetHead(LinkList L) {//返回线性链表P的头指针
		return L.head;
}

Status InsFirst(LinkList &L, Link h, Link s)	//将s所指的结点插入在h后的第一个结点之前
{
	s->next = h->next;
	h->next = s;
	if (h == L.tail) {
		L.tail = h->next;
	}
	L.len++;
	return OK;
}

Status DelFirst(LinkList &L, Link h, Link &q) {	//删除h后的第一个结点并以q返回
	q = h->next;
	if (q) {
		h->next = q->next;
		if (!q->next) {
			L.tail = h;
		}
		L.len--;
		return OK;
	}
	else
		return FALSE;
}

Status Append(LinkList &L, Link s) {	//将s所指的一串结点连接在线性链表L的最后一个结点之后，并将L的尾指针指向新的结点
	int i = 1;
	L.tail->next = s;
	while (s->next) {
		i++;
		s = s->next;
	}
	L.tail = s;
	L.len += i;
	return OK;
}

Status LocateElem(LinkList L, ElemType e, Position &q, int(*compare)(ElemType, ElemType)){	//在L中寻找指数与e相同的结点，用q指示其位置
	Link p, pp;
	p = L.head;
	do
	{
		pp = p;
		p = p->next;
	} while (p && compare(p->data, e) < 0);   //找到第一个大于等于e的位置

	if (!p || compare(p->data, e) > 0)   //到表尾或者找到的位置的值并不相等
	{
		q = pp;   //使q指示第一个大于e的结点的直接前驱
		return FALSE;
	}
	else   //找到相等
	{
		q = p;
		return TRUE;
	}
}

Status OrderInsert(LinkList &L, ElemType e, int cmp(ElemType, ElemType)) {					//将e生成结点并按照指数的顺序插入到线性链表L中
	Position q, s;
	if (LocateElem(L, e, q, cmp)) {
		q->data.coef += e.coef;
		if (!q->data.coef) {
			s = PriorPos(L, q);
			if (!s) {
				s = L.head;
			}
			DelFirst(L, s, q);
			FreeNode(q);
		}
		return OK;
	}
	else {
		if (MakeNode(s, e)) {
			InsFirst(L,q, s);
			return OK;
		}
		return ERROR;
	}
}

int cmp(term a, term b) {		//比较指数
	if (a.expn == b.expn) return 0;
	if (a.expn < b.expn) return -1;
	return 1;
}

void CreatPolyn(polynomial &P, int m) {	//输入m项的系数和指数，建立表示一元多项式的有序链表P
	int i;
	ElemType e;
	Position q , s;
	InitList(P);
	LNode *h = GetHead(P);
	h->data.coef = 0.0;
	h->data.expn = -1;
	for (i = 1; i <= m; ++i){
		scanf("%f %d",&e.coef,&e.expn);
		if (!LocateElem(P, e, q, cmp)){
			if (MakeNode(s, e)) {
				InsFirst(P,q, s);
			}
		}
	}
}

void DestoryPolyn(polynomial &P) {		//销毁一元多项式P
	ClearList(P);
	FreeNode(P.head);
	P.tail = NULL;
	P.len = 0;
}

void PrintPolyn(polynomial P) {			//打印输出一元多项式P
	Link q;
	q = P.head->next;
	printf("系数  指数\n");
	while (q) {
		printf("%.2f	%d\n", q->data.coef, q->data.expn);
		q = q->next;
	}
}

int PolyLength(polynomial P) {			//返回一元多项式P中的项数
	return P.len;
}

void AddPolyn(polynomial &Pa, polynomial &Pb) {	//一元多项式的相加
	Position ha, hb, qa, qb;
	term a, b;

	ha = GetHead(Pa);
	hb = GetHead(Pb);
	qa = NextPos(Pa,ha);
	qb = NextPos(Pb,hb);

	while (qb && qa) {
		a = GetCurElem(qa);
		b = GetCurElem(qb);
		switch (cmp(a, b)) {
		case -1:
			ha = qa;
			qa = NextPos(Pa,qa);
			break;
		case 0:
			a.coef += b.coef;
			if (a.coef  != 0.0) {
				SetCurElem(qa, a);
			}
			else {
				DelFirst(Pa, ha,qa);
				FreeNode(qa);
			}
			DelFirst(Pb, hb, qb);
			FreeNode(qb);
			qb = NextPos(Pb, hb);
			qa = NextPos(Pa, ha);
			break;
		case 1 :
			DelFirst(Pb,hb, qb);
			InsFirst(Pa,ha, qb);
			qb = NextPos(Pb, hb);
			ha = NextPos(Pa, ha);
			break;
		}
	}
	if (!ListEmpty(Pb)) {
		Append(Pa, qb);
	}
	FreeNode(hb);
}

void MultipyPolyn(polynomial &Pa, polynomial &Pb)	//一元多项式的相乘
{
	polynomial Pc;
	Position qa, qb;
	term a, b, c;

	InitList(Pc);
	qa = GetHead(Pa);
	qa = qa->next;
	while (qa) {
		a = GetCurElem(qa);
		qb = GetHead(Pb);
		qb = qb->next;
		while (qb) {
			b = GetCurElem(qb);
			c.coef = a.coef * b.coef;
			c.expn = a.expn + b.expn;
			OrderInsert(Pc, c, cmp);
			qb = qb->next;
		}
		qa = qa->next;
	}
	DestoryPolyn(Pb);
	ClearList(Pa);
	Pa.head = Pc.head;
	Pa.tail = Pc.tail;
	Pa.len = Pc.len;
}


int main() {	//主函数
	polynomial p, q;
	int m;

	printf("请输入一个一元多项式的非零项的个数：");
	scanf("%d", &m);
	CreatPolyn(p, m);
	PrintPolyn(p);

	printf("请输入一个一元多项式的非零项的个数：");
	scanf("%d", &m);
	CreatPolyn(q, m);
	PrintPolyn(q);

	//AddPolyn(p, q);
	MultipyPolyn(p, q);
	PrintPolyn(p);
	DestoryPolyn(p);
	system("pause");
	return 0;
}