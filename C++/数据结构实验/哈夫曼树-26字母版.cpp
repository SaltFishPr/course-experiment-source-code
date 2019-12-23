#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

int weight[26];		//存储字符串各字符出现频率
typedef struct {
	char ch;
	 int weight;
	 int parent, lchild, rchild;
}HTNode, *HuffmanTree;  //HuffmanTree是一个数组  HTNode是节点，也是HuffmanTree数组的数据

typedef char **HuffmanCode;

void counter(string str) {	//遍历字符串  权重保存在weight中
	int i, index = str.size();
	for (i = 0; i < index; i++) {
		switch (str[i]) {
		case 'a':weight[0]++; break;
		case 'b':weight[1]++; break;
		case 'c':weight[2]++; break;
		case 'd':weight[3]++; break;
		case 'e':weight[4]++; break;
		case 'f':weight[5]++; break;
		case 'g':weight[6]++; break;
		case 'h':weight[7]++; break;
		case 'i':weight[8]++; break;
		case 'j':weight[9]++; break;
		case 'k':weight[10]++; break;
		case 'l':weight[11]++; break;
		case 'm':weight[12]++; break;
		case 'n':weight[13]++; break;
		case 'o':weight[14]++; break;
		case 'p':weight[15]++; break;
		case 'q':weight[16]++; break;
		case 'r':weight[17]++; break;
		case 's':weight[18]++; break;
		case 't':weight[19]++; break;
		case 'u':weight[20]++; break;
		case 'v':weight[21]++; break;
		case 'w':weight[22]++; break;
		case 'x':weight[23]++; break;
		case 'y':weight[24]++; break;
		case 'z':weight[25]++; break;
		}
	}
}
/*
选择权最小的两个节点
*/
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
/*
哈夫曼编码函数
参数 HT 构造哈夫曼树
参数 HC 构造哈夫曼编码字符串
参数 *w 权重值（即weight[]）
参数 n  字母数 26
*/
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n) {
	HuffmanTree p;
	int s1, s2;
	char *cd;
	int c1, i, c2;

	if (n <= 1) {
		return;
	}	
	 int m = 2 * n - 1;  //n个字符，m个节点
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
	for (p = HT+1, i = 1; i <= n; ++i, ++p, ++w) {
		*p = { (char)(i + 96),*w,0,0,0 };
	}
	for (; i <= m; ++i, ++p) {
		*p = { ' ',0,0,0,0 };
	}
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
/*
解码函数
*/
string HuffmanDeCoding(HuffmanTree HT, HuffmanCode HC, string str) {
	int i = 0, j, t;
	string str1 = "";

	while (i < str.length()) {
		t = 51;
		for (j = i; j < str.length(); j++) {
			if (str[j] == '0') {
				t = HT[t].lchild;
			}
			else {
				t = HT[t].rchild;
			}
			if (HT[t].lchild == 0 || HT[t].rchild == 0) {
				str1 += HT[t].ch;
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
	int n = 26;
	int *w = &weight[0];

	counter(str);
	HuffmanCoding(HT, HC, w, n);
	for (int i = 1; i <= n; i++) {
		cout << HT[i].ch << "的编码是：" << HC[i] << endl;
	}
	cout << "请输入数字码：";
	cin >> str1;
	cout << endl << "译码结果为：" << HuffmanDeCoding(HT, HC, str1)<<endl;
	system("pause");
	return 0;
}