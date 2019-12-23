#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <fstream>
using namespace std;

#define MAX_VERTEX_NUM 20
#define OK 1
#define TRUE    1
#define FALSE   0
#define ERROR   0
#define INFEASIBLE  -1

typedef char VertexType[3];
typedef int SElemType;
typedef int Status;
typedef enum { DG, DN, UDG, UDN } GraphKind;	//����ͼ0��������1������ͼ2��������3

int ve[MAX_VERTEX_NUM], vl[MAX_VERTEX_NUM], c[MAX_VERTEX_NUM];	//����ʱ�䣬����ʱ�䣬�������

typedef struct ArcNode { 
	int adjvex;					//��ָ��ڵ�Ķ����ڱ��е�λ��
	struct ArcNode * nextarc;	//ָ��ýڵ����һ����
}ArcNode;

typedef struct VNode {
	VertexType subject;			//��Ŀ��
	ArcNode *firstarc;			//ָ���һ����
	int credit;					//ָ��ѧ����Ϣ
}VNode, AdjList[MAX_VERTEX_NUM];	//VNodeΪͷ�ڵ㣬AdjList[]Ϊ��

typedef struct {
	AdjList vertices;			//ͼ���ڽӱ�
	int vexnum, arcnum;			//������������
	int kind;					//ͼ�������ʶ
}ALGraph;						//ͼ�ṹ��

typedef struct SNode {
	SElemType data;                 // ������
	struct SNode *next;             // ָ����
} SNode, *LinkStack;

Status InitStack(LinkStack &S) {	// ����һ����ջS��
	S = (LinkStack)malloc(sizeof(SNode));
	if (!S)                         // �洢����ʧ��
		exit(OVERFLOW);             // exit(-2)�����쳣�˳�
	S->next = NULL;
	return OK;
}

Status DestroyStack(LinkStack &S) {// ����ջS��S���ٴ��ڡ�
	LinkStack p = S->next, ptmp;   // pָ��ջ��
	while (p) {                    // pָ��ջ��ʱ��ѭ��ֹͣ
		ptmp = p->next;
		free(p);                   // �ͷ�ÿ�����ݽ���ָ����
		p = ptmp;
	}
	free(S);
	return OK;
}

Status ClearStack(LinkStack &S) {//��S��Ϊ��ջ��
	LinkStack p = S->next, ptmp; // pָ��ջ��
	while (p) {                  // pָ��ջ��ʱ��ѭ��ֹͣ
		ptmp = p->next;
		free(p);                 // �ͷ�ÿ�����ݽ���ָ����
		p = ptmp;
	}
	S->next = NULL;
	return OK;
}

Status StackEmpty(LinkStack S) {//��SΪ��ջ������TRUE�����򷵻�FALSE
	if (S->next == NULL)
		return TRUE;            // ����1
	else
		return FALSE;           // ����0
}

int StackLength(LinkStack S) {//����S��Ԫ�ظ�������ջ�ĳ��ȡ�
	int n = 0;
	LinkStack p = S->next;    // pָ��ջ��
	while (p) {
		n++;
		p = p->next;
	}
	return n;
}

Status GetTop(LinkStack S, SElemType &e) {	//��ջ���գ�����e����S��ջ��Ԫ�أ�������OK�����򷵻�ERROR��
	if (S->next == NULL)
		return ERROR;						// ջ��
	e = S->next->data;						// ȡջ��Ԫ��
	return OK;
}

Status Push(LinkStack &S, SElemType e) {//����Ԫ��eΪ�µ�ջ��Ԫ�ء�
	LinkStack p = (LinkStack)malloc(sizeof(SNode));
	p->data = e;
	p->next = S->next;					// �½��ָ��ջ��
	S->next = p;						// ����ջ��ָ��
	return OK;
}

Status Pop(LinkStack &S, SElemType &e) {// ��ջ���գ���ɾ��S��ջ��Ԫ�أ�����e������ֵ�����򷵻�ERROR��
	// ��1��Ԫ��Ҳû�У�
	if (S->next == NULL)
		return ERROR;
	// ����1������Ԫ��
	e = S->next->data;
	LinkStack ptmp = S->next->next;
	free(S->next);
	S->next = ptmp;
	return OK;
}

int LocateVex(ALGraph G, VertexType u) {
	int i;
	for (i = 0; i < G.vexnum; ++i)
		if (strcmp(u, G.vertices[i].subject) == 0)
			return i;
	return -1;
}

Status CreateGraph(ALGraph &G) {	//����ͼ���ڽӱ�
	int i;
	int v1, v2;
	char  str[4];
	ArcNode *p, *q;

	ifstream fin("D:\\c++lianxi\\txtpath\\ALGraph.txt");

	//��ʼ��
	G.kind = 0;
	G.vexnum = 0;
	G.arcnum = 0;
	for (i = 0; i < MAX_VERTEX_NUM; i++) {
		G.vertices[i].credit = 0;
		G.vertices[i].firstarc = NULL;
		strcpy(G.vertices[i].subject, "  ");
	}

	//����
	cout << "�γ�������" << endl;
	fin >> G.vexnum;
	cout << "���пγ����Ƽ�ѧ�֣�" << endl;
	for (i = 0; i < G.vexnum; i++) {
		fin >> G.vertices[i].subject;
		fin >> G.vertices[i].credit;
	}
	cout << "����һ�γ�����ֱ�����޿����ƣ�һ������һ�飩����-1��������" << endl;
	while (1) {

		fin >> str;
		if (strcmp(str, "-1") == 0)
			break;
		v1 = LocateVex(G, str);	//��β
		fin >> str;
		v2 = LocateVex(G, str);	//��ͷ

		p = G.vertices[v2].firstarc;
		if (p == NULL) {
			q = (ArcNode*)malloc(sizeof(ArcNode));
			if (!q) return FALSE;
			q->adjvex = v1;
			q->nextarc = NULL;
			G.vertices[v2].firstarc = q;
		}
		else {
			while (p->nextarc != NULL) {
				p = p->nextarc;
			}
			q = (ArcNode*)malloc(sizeof(ArcNode));
			if (!q) return FALSE;
			*q = { v1,NULL };
			p->nextarc = q;
		}
		c[v1]++;
		G.arcnum++;
	}
	return OK;
}

Status ToplogicalOrder(ALGraph G, LinkStack &T) {	//��������
	int i, j, k, count = 0;
	ArcNode *p;
	LinkStack S;

	InitStack(S);

	for (i = 0; i < G.vexnum; ++i) {
		if (!c[i]) {
			Push(S, i);
		}
	}

	while (!StackEmpty(S)) {
		Pop(S, j);
		Push(T, j);
		++count;
		for (p = G.vertices[j].firstarc; p; p = p->nextarc) {
			k = p->adjvex;
			if ((--c[k]) == 0) {
				Push(S, k);
			}
			if (ve[j] + G.vertices[j].credit > ve[k]) {
				ve[k] = ve[j] + G.vertices[j].credit;
			}
		}
	}
	if (count < G.vexnum) return ERROR;
	else return OK;
}
/*
Status CriticalPath(ALGraph G, LinkStack T) {
	int i, j, k, dut, ee, el;
	char tag;
	ArcNode *p;
	*vl = {};
	if (!ToplogicalOrder(G, T)) return ERROR;
	for (i = 0; i < G.vexnum; ++i) {
		vl[i] = ve[G.vexnum - 1];
	}
	while (!StackEmpty(T)) {
		Pop(T, j);
		p = G.vertices[j].firstarc;
		for (; p; p = p->nextarc) {
			k = p->adjvex;
			dut = G.vertices[j].credit;
			if (vl[k] - dut < vl[j]) {
				vl[j] = vl[k] - dut;
			}
		}
	}
	cout << "�΢�\t�΢�\t��ѧ��\t������ʱ��\t������ʱ��\t�Ƿ�Ϊ�ؼ�·��\t" << endl;
	for (j = 0; j < G.vexnum; ++j) {
		for (p = G.vertices[j].firstarc; p; p = p->nextarc) {
			k = p->adjvex;
			dut = G.vertices[j].credit;
			ee = ve[j];
			el = vl[k] - dut;
			tag = (ee == el) ? '*' : ' ';
			cout << j+1 << "\t" << k+1 << "\t" << dut << "\t" << ee << "\t\t" << el << "\t\t" << tag << endl;
		}
	}
	return OK;
}
*/

void change(LinkStack T) {		//��T�е�Ԫ������
	int m = StackLength(T);
	int *t;
	t = (int *)malloc(sizeof(int)*m);

	int i, j;
	for (i = 0; i < m; i++) {
		if(!StackEmpty(T)) Pop(T, j);
		else break;
		t[i] = j;
	}
	for (i = 0; i < m; i++) {
		Push(T, t[i]);
	}
}

void see(LinkStack T) {
	change(T);
	int i;
	while (!StackEmpty(T)) {
		Pop(T, i);
		cout << i << " ";
	}
}

void courseArrange(ALGraph G, LinkStack T, int a, int b) {
	int i, j, k = 1, sum = 0, t;
	ofstream fout;
	fout.open("D:\\course.txt");
	change(T);
	LinkStack S;
	InitStack(S);
	
	while (1) {
		Pop(T, i);
		if (sum + G.vertices[i].credit <= b) {
			sum += G.vertices[i].credit;
			Push(S, i);
		}
		else {
			sum = 0;
			Push(T, i);
			fout << "��" << k << "��ѧ��ѧ";
			change(S);
			while (!StackEmpty(S)) {
				Pop(S, j);
				fout << G.vertices[j].subject << " ";
			}
			fout << endl;
			k++;
		}
		if (k > a) break;
		if (StackEmpty(T)) {
			fout << "��" << k << "��ѧ��ѧ";
			change(S);
			while (!StackEmpty(S)) {
				Pop(S, j);
				fout << G.vertices[j].subject << " ";
			}
			fout << endl;
			break;
		}
	}
	if (!StackEmpty(T)) cout << "���ʵ����򷽰���"<<endl;
	else cout << "���ݱ���λ��  D:\course.txt" << endl;
}

int main() {
	ALGraph G;
	LinkStack T;//�������򶥵�ջ
	int a, b;	//ѧ��������һѧ�ڵ�ѧ������
	CreateGraph(G);
	InitStack(T);
	ToplogicalOrder(G, T);
	cout << "������ѧ������ÿѧ��ѧ������" << endl;

	cin >> a >> b;
	courseArrange(G,T,a,b);
	system("pause");
	return 0;
}