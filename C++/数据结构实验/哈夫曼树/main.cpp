/*
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

int k = 1;

typedef struct {
	char ch;
	 int weight;
	 int parent, lchild, rchild;
}HTNode, *HuffmanTree;  //HuffmanTree是一个数组  HTNode是节点，也是HuffmanTree数组的数据

typedef char **HuffmanCode;


void Select(HuffmanTree HT, int t, int &s1, int &s2) {
	 int i, m1 = INT_MAX, m2 = INT_MAX;
	s1 = s2 = 1;
	for (i = 1; i <= t; i++) {
		if (HT[i].weight < m1&& HT[i].weight>-1) {
			m2 = m1;
			m1 = HT[i].weight;
			s2 = s1;
			s1 = i;	
		}
		else if (HT[i].weight < m2&&HT[i].weight>-1) {
			m2 = HT[i].weight;
			s2 = i;
		}
	}
}

int HuffmanInitialization(string str,HuffmanTree &HT) {
	int i,j;

	for (i = 0; i < str.length(); i++) {
		for (j = 1; j <= str.length(); j++) {
			if (HT[j].ch == str[i]) {
				HT[j].weight++;
				break;
			}
		}
		if (j > str.length())
		{
			HT[k].ch = str[i];
			HT[k].weight = 1;
			k++;
		}
	}
	return k;
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, string str, int n) {
	
	int s1, s2;
	char *cd;
	int c1, i, c2, k = 1;
	n = k - 1;
	if (n <= 1) {
		return;
	}	
	int m = 2 * n - 1;  //n个字符，m个节点
	for (i = n + 1; i <= m; ++i) {
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i; HT[s2].parent = i;
		HT[i].lchild = s1; HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
		HT[s1].weight = -1;
		HT[s2].weight = -1;
	}
	HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= n; ++i) {
		int start = n - 1;
		c1 = i;
		c2 = HT[i].parent;
		while ( c2 != 0 ){
			if (HT[c2].lchild == c1) {
				cd[--start] = '0';
			}
			else {
				cd[--start] = '1';
			}
			c1 = c2;
			c2 = HT[c1].parent;
		}
		HC[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy(HC[i], &cd[start]);
	}
	free(cd);
}

string HuffmanDeCoding(HuffmanTree HT, HuffmanCode HC, string str,int root) {
	int i = 0, j;
	string str1 = "";

	while (i < str.length()) {
		int m = root;
		for (j = i; j < str.length(); j++) {
			if (str[j] == '0') {
				m = HT[m].lchild;
			}
			else {
				m = HT[m].rchild;
			}
			if (HT[m].lchild == 0 || HT[m].rchild == 0) {
				str1 += HT[m].ch;
				break;
			}
		}
		i = j + 1;
	}
	return str1;
}
int main() {
	string str,str1;
	cin >> str;
	HuffmanTree HT;
	HuffmanCode HC;
	HuffmanTree p;
	int i;
	
	HT = (HuffmanTree)malloc((257) * sizeof(HTNode));
	int n = HuffmanInitialization(str, HT) - 1;
	int m = 2 * n - 1;
	for (p = HT + k, i = 1; i <= m; ++i, ++p) {
		*p = { ' ',0,0,0,0 };
	}
	HuffmanCoding(HT, HC, str, n);
	for (int i = 1; i <= n; i++) {
		cout << HT[i].ch << "的编码是：" << HC[i] << endl;
	}
	cout << "请输入数字码：";
	cin >> str1;
	cout << endl << "译码结果为：" << HuffmanDeCoding(HT, HC, str1,m)<<endl;
	system("pause");
	return 0;
}
*/