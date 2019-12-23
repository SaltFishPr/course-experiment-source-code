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
typedef enum { DG, DN, UDG, UDN } GraphKind;	//有向图0，有向网1，无向图2，无向网3

int ve[MAX_VERTEX_NUM], vl[MAX_VERTEX_NUM], c[MAX_VERTEX_NUM];	//最早时间，最晚时间，顶点入度

typedef struct ArcNode { 
	int adjvex;					//该指向节点的顶点在表中的位置
	struct ArcNode * nextarc;	//指向该节点的下一个弧
}ArcNode;

typedef struct VNode {
	VertexType subject;			//科目名
	ArcNode *firstarc;			//指向第一个弧
	int credit;					//指向学分信息
}VNode, AdjList[MAX_VERTEX_NUM];	//VNode为头节点，AdjList[]为表

typedef struct {
	AdjList vertices;			//图的邻接表
	int vexnum, arcnum;			//顶点数，弧数
	int kind;					//图的种类标识
}ALGraph;						//图结构体

typedef struct SNode {
	SElemType data;                 // 数据域
	struct SNode *next;             // 指针域
} SNode, *LinkStack;

Status InitStack(LinkStack &S) {	// 构造一个空栈S。
	S = (LinkStack)malloc(sizeof(SNode));
	if (!S)                         // 存储分配失败
		exit(OVERFLOW);             // exit(-2)程序异常退出
	S->next = NULL;
	return OK;
}

Status DestroyStack(LinkStack &S) {// 销毁栈S，S不再存在。
	LinkStack p = S->next, ptmp;   // p指向栈顶
	while (p) {                    // p指向栈底时，循环停止
		ptmp = p->next;
		free(p);                   // 释放每个数据结点的指针域
		p = ptmp;
	}
	free(S);
	return OK;
}

Status ClearStack(LinkStack &S) {//把S置为空栈。
	LinkStack p = S->next, ptmp; // p指向栈顶
	while (p) {                  // p指向栈底时，循环停止
		ptmp = p->next;
		free(p);                 // 释放每个数据结点的指针域
		p = ptmp;
	}
	S->next = NULL;
	return OK;
}

Status StackEmpty(LinkStack S) {//若S为空栈，返回TRUE，否则返回FALSE
	if (S->next == NULL)
		return TRUE;            // 返回1
	else
		return FALSE;           // 返回0
}

int StackLength(LinkStack S) {//返回S的元素个数，即栈的长度。
	int n = 0;
	LinkStack p = S->next;    // p指向栈顶
	while (p) {
		n++;
		p = p->next;
	}
	return n;
}

Status GetTop(LinkStack S, SElemType &e) {	//若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR。
	if (S->next == NULL)
		return ERROR;						// 栈空
	e = S->next->data;						// 取栈顶元素
	return OK;
}

Status Push(LinkStack &S, SElemType e) {//插入元素e为新的栈顶元素。
	LinkStack p = (LinkStack)malloc(sizeof(SNode));
	p->data = e;
	p->next = S->next;					// 新结点指向栈顶
	S->next = p;						// 更新栈顶指针
	return OK;
}

Status Pop(LinkStack &S, SElemType &e) {// 若栈不空，则删除S的栈顶元素，并用e返回其值；否则返回ERROR。
	// 若1个元素也没有：
	if (S->next == NULL)
		return ERROR;
	// 若有1个以上元素
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

Status CreateGraph(ALGraph &G) {	//创建图（邻接表）
	int i;
	int v1, v2;
	char  str[4];
	ArcNode *p, *q;

	ifstream fin("D:\\c++lianxi\\txtpath\\ALGraph.txt");

	//初始化
	G.kind = 0;
	G.vexnum = 0;
	G.arcnum = 0;
	for (i = 0; i < MAX_VERTEX_NUM; i++) {
		G.vertices[i].credit = 0;
		G.vertices[i].firstarc = NULL;
		strcpy(G.vertices[i].subject, "  ");
	}

	//输入
	cout << "课程总数：" << endl;
	fin >> G.vexnum;
	cout << "所有课程名称及学分：" << endl;
	for (i = 0; i < G.vexnum; i++) {
		fin >> G.vertices[i].subject;
		fin >> G.vertices[i].credit;
	}
	cout << "输入一课程与其直接先修课名称（一行输入一组），以-1结束输入" << endl;
	while (1) {

		fin >> str;
		if (strcmp(str, "-1") == 0)
			break;
		v1 = LocateVex(G, str);	//弧尾
		fin >> str;
		v2 = LocateVex(G, str);	//弧头

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

Status ToplogicalOrder(ALGraph G, LinkStack &T) {	//拓扑排序
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
	cout << "课①\t课②\t①学分\t①最早时间\t②最晚时间\t是否为关键路径\t" << endl;
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

void change(LinkStack T) {		//将T中的元素逆序
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
			fout << "第" << k << "个学期学";
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
			fout << "第" << k << "个学期学";
			change(S);
			while (!StackEmpty(S)) {
				Pop(S, j);
				fout << G.vertices[j].subject << " ";
			}
			fout << endl;
			break;
		}
	}
	if (!StackEmpty(T)) cout << "无适当排序方案！"<<endl;
	else cout << "数据保存位置  D:\course.txt" << endl;
}

int main() {
	ALGraph G;
	LinkStack T;//拓扑排序顶点栈
	int a, b;	//学分总数，一学期的学分上限
	CreateGraph(G);
	InitStack(T);
	ToplogicalOrder(G, T);
	cout << "请输入学期数，每学期学分上限" << endl;

	cin >> a >> b;
	courseArrange(G,T,a,b);
	system("pause");
	return 0;
}