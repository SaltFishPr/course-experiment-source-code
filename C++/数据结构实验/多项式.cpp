#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1  
#define OVERFLOW -2

typedef struct		//��ı�ʾ������ʽ������ΪLinkList������Ԫ��
{
	float coef; //ϵ��
	int expn;   //ָ��
}term, ElemType;//������������term���ڱ�ADT��ElemTypeΪLinkList�����ݶ����� 

typedef struct LNode		//�ڵ����ͣ����������Լ�ָ����һ���ڵ��ָ��
{
	ElemType data;
	struct LNode *next;
}LNode, *Link, *Position;

typedef struct				//��������
{
	Link head, tail;	//ͷ�ڵ���β�ڵ�
	int len;
}LinkList;

typedef LinkList polynomial;			//�ô�ͷ�������������ʾ����ʽ
typedef int Status;						//��Status����Ϊint��

Status InitList(polynomial &L) {		//����һ���յ�����
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

Status MakeNode(Link &p, ElemType e) {	//������pָ���ֵΪe�Ľڵ�
	p = (Link)malloc(sizeof(LNode));	//�����½��  
	if (!p) return ERROR;				//����ʧ��  
	p->data = e;						//��e���ڵ����ݸ�ֵ
	return OK;
}

Position PriorPos(LinkList L, Link p) {	//��֪pָ��L�е�һ���ڵ㣬����p��ָ�ڵ��ֱ��ǰ����λ��
	Link q;
	q = L.head;
	while (q->next != p) {
		q = q->next;
	}
	return q;
}

Position NextPos(LinkList L,Link p){	//��֪pָ��L�е�һ���ڵ㣬����p��ָ�ڵ��ֱ�Ӻ�̵�λ��
	return p->next;
}

Status ClearList(LinkList &L) {			//�����Ա�L��Ϊ�ձ�
	Link p, q;//����ָ��ڵ��ָ��

	if (L.head != L.tail) {
		p = q = L.head->next;
		L.head->next = NULL;

		while (p != L.tail) {//����
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

void FreeNode(Link &p) {				//�ͷ�p��ָ�ڵ㲢ʹpΪ��
	free(p);
	p = NULL;
}

ElemType GetCurElem(Link p) {			//��֪pָ�����������е�һ���ڵ㣬����p��ָ��ڵ��ֵ
	return p->data;
}

Status ListEmpty(LinkList L) {			//�ж������Ƿ�Ϊ�ձ�
	return L.len == 0;
}

Status SetCurElem(Link &p, ElemType e) {//��֪pָ�����������е�һ���ڵ㣬��e����p��ָ��ڵ��ֵ
	if (p) {
		p->data = e;
		return OK;
	}
	else {
		return FALSE;
	}
}

Position GetHead(LinkList L) {//������������P��ͷָ��
		return L.head;
}

Status InsFirst(LinkList &L, Link h, Link s)	//��s��ָ�Ľ�������h��ĵ�һ�����֮ǰ
{
	s->next = h->next;
	h->next = s;
	if (h == L.tail) {
		L.tail = h->next;
	}
	L.len++;
	return OK;
}

Status DelFirst(LinkList &L, Link h, Link &q) {	//ɾ��h��ĵ�һ����㲢��q����
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

Status Append(LinkList &L, Link s) {	//��s��ָ��һ�������������������L�����һ�����֮�󣬲���L��βָ��ָ���µĽ��
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

Status LocateElem(LinkList L, ElemType e, Position &q, int(*compare)(ElemType, ElemType)){	//��L��Ѱ��ָ����e��ͬ�Ľ�㣬��qָʾ��λ��
	Link p, pp;
	p = L.head;
	do
	{
		pp = p;
		p = p->next;
	} while (p && compare(p->data, e) < 0);   //�ҵ���һ�����ڵ���e��λ��

	if (!p || compare(p->data, e) > 0)   //����β�����ҵ���λ�õ�ֵ�������
	{
		q = pp;   //ʹqָʾ��һ������e�Ľ���ֱ��ǰ��
		return FALSE;
	}
	else   //�ҵ����
	{
		q = p;
		return TRUE;
	}
}

Status OrderInsert(LinkList &L, ElemType e, int cmp(ElemType, ElemType)) {					//��e���ɽ�㲢����ָ����˳����뵽��������L��
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

int cmp(term a, term b) {		//�Ƚ�ָ��
	if (a.expn == b.expn) return 0;
	if (a.expn < b.expn) return -1;
	return 1;
}

void CreatPolyn(polynomial &P, int m) {	//����m���ϵ����ָ����������ʾһԪ����ʽ����������P
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

void DestoryPolyn(polynomial &P) {		//����һԪ����ʽP
	ClearList(P);
	FreeNode(P.head);
	P.tail = NULL;
	P.len = 0;
}

void PrintPolyn(polynomial P) {			//��ӡ���һԪ����ʽP
	Link q;
	q = P.head->next;
	printf("ϵ��  ָ��\n");
	while (q) {
		printf("%.2f	%d\n", q->data.coef, q->data.expn);
		q = q->next;
	}
}

int PolyLength(polynomial P) {			//����һԪ����ʽP�е�����
	return P.len;
}

void AddPolyn(polynomial &Pa, polynomial &Pb) {	//һԪ����ʽ�����
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

void MultipyPolyn(polynomial &Pa, polynomial &Pb)	//һԪ����ʽ�����
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


int main() {	//������
	polynomial p, q;
	int m;

	printf("������һ��һԪ����ʽ�ķ�����ĸ�����");
	scanf("%d", &m);
	CreatPolyn(p, m);
	PrintPolyn(p);

	printf("������һ��һԪ����ʽ�ķ�����ĸ�����");
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