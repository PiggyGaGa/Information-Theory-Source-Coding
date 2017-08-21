#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
class Huffman
{
public:
	struct HuffmanNode
	{
		unsigned char value; //节点值
		int frequency = 0; //节点频数
		struct HuffmanNode *Lchild = NULL;
		struct HuffmanNode *Rchild = NULL;

	};
private:
	struct CountVector
	{
		unsigned char value; //字符
		int frequency = 0;  //字符频数
		struct HuffmanNode *nodeAddress = NULL;
	};


	struct HuffmanCode
	{
		unsigned char value;
		int frequency = 0;
		string code;
		int codelen;
	};

	//根节点

	static bool mysortfunction(CountVector A, CountVector B)
	{  //用于sort排序算法
		return A.frequency < B.frequency;
	}

public:
	HuffmanNode *root;
	string fileAddress;
	long int NumOfChar = 0;
	vector<CountVector> charCountFrequency;  //用于存储字符频数
	vector<HuffmanCode> HuffmanCodeVec;
	Huffman(string sourcefile);  //构造函数
	void count();   //统计各个字符的频数函数
	void CreateHuffmanTree(vector<CountVector> charFrequency);  //创建huffman树
	void GetHuffmanCode(HuffmanNode *root, int len);
	void WriteCode(vector<HuffmanCode> hfCode);
	void Decode(string sourcefile, string dstfile);
};

Huffman::Huffman(string sourcefile)
{
	fileAddress = sourcefile;  //初始化文件读入地址
}

void Huffman::count()
{
	ifstream readfile;
	readfile.open(fileAddress, ios::in | ios::binary);
	unsigned char *now = new unsigned char;  //存储当前读取到的字符
	while (!readfile.eof())
	{
		CountVector *presentChar = new CountVector;  //读取到的字符信息
		readfile.read((char*)now, sizeof(unsigned char));
		int flag = 0;   //标志是否是新出现的字符
		for (int i = 0; i < charCountFrequency.size(); i++)
		{
			if (*now == charCountFrequency[i].value)
			{
				charCountFrequency[i].frequency++;
				NumOfChar++;
				flag = 1;
			}

		}
		if (flag == 0)
		{
			presentChar->value = *now;
			presentChar->frequency++;
			NumOfChar++;
			charCountFrequency.push_back(*presentChar);
		}
	}
	readfile.close();
}

void Huffman::CreateHuffmanTree(vector<CountVector> charFrequency)
{
	vector<CountVector>  buildtree;
	//HuffmanNode newNode;
	HuffmanNode *rootnode = new HuffmanNode;
	buildtree = charFrequency;
	sort(buildtree.begin(), buildtree.end(), mysortfunction);
	int treedepth = 0;
	while (buildtree.size() > 1)
	{
		HuffmanNode *nodeLeft = new HuffmanNode,
			*nodeRight = new HuffmanNode,
			*newNode = new HuffmanNode;
		CountVector insertnew;
		if (buildtree[0].nodeAddress != NULL)
		{  //如果是叶子节点的话  左右子树的地址都为NULL
			nodeLeft->Lchild = buildtree[0].nodeAddress->Lchild;
			nodeLeft->Rchild = buildtree[0].nodeAddress->Rchild;
		}
		else
		{
			nodeLeft->Lchild = NULL;
			nodeLeft->Rchild = NULL;
		}
		if (buildtree[1].nodeAddress != NULL)
		{
			nodeRight->Lchild = buildtree[1].nodeAddress->Lchild;
			nodeRight->Rchild = buildtree[1].nodeAddress->Rchild;
		}
		else
		{
			nodeRight->Lchild = NULL;
			nodeRight->Rchild = NULL;
		}
		nodeLeft->frequency = buildtree[0].frequency;
		nodeLeft->value = buildtree[0].value;
		nodeRight->frequency = buildtree[1].frequency;
		nodeRight->value = buildtree[1].value;
		newNode->frequency = nodeRight->frequency + nodeLeft->frequency;
		newNode->Lchild = nodeLeft;
		newNode->Rchild = nodeRight;
		insertnew.frequency = newNode->frequency;
		insertnew.value = 0;
		insertnew.nodeAddress = newNode;
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.erase(buildtree.begin());
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.erase(buildtree.begin());
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.insert(buildtree.begin(), insertnew);
		sort(buildtree.begin(), buildtree.end(), mysortfunction);   //每次更新完要排序
		rootnode = newNode;
		treedepth++;

	}
	//cout << treedepth;
	root = rootnode;
}

void  Huffman::GetHuffmanCode(HuffmanNode* root, int depth)
{
	static char code[512];
	//判断左儿子
	if (root->Lchild != NULL)
	{
		code[depth] = '0';
		code[depth + 1] = '\0';
		GetHuffmanCode(root->Lchild, depth + 1);
	}
	if (root->Rchild != NULL)
	{
		code[depth] = '1';
		code[depth + 1] = '\0';
		GetHuffmanCode(root->Rchild, depth + 1);
	}
	else
	{
		HuffmanCode insertCode;
		int codelength = 0;
		for (int i = 0; i < charCountFrequency.size(); i++)
		{
			if (root->value == charCountFrequency[i].value)
			{
				insertCode.code = code;
				insertCode.value = charCountFrequency[i].value;
				insertCode.frequency = charCountFrequency[i].frequency;
			}
		}
		for (int j = 0; code[j] != '\0'; j++)
		{
			codelength++;
		}
		insertCode.codelen = codelength;
		HuffmanCodeVec.push_back(insertCode);

	}

}

void Huffman::WriteCode(vector<HuffmanCode> HFCode)
{
	//从文件总读取字符并进行编码
	int codeNum = HFCode.size();
	string address = fileAddress;
	ofstream writecode;
	ifstream read;
	read.open(address, ios::in | ios::binary);   //读入文件
	writecode.open(address + ".dada", ios::out | ios::binary);   //以*.dada命名
	unsigned char *now = new unsigned char; //读取的 当前字符
	unsigned char save = 0;  //每一次保存一个字节的长度
	int len = 0;
	long int totalLen = 0; //文件编码总长
	int last; //最后写入时的位数

	for (int i = 0; i < HFCode.size(); i++)
	{
		totalLen = totalLen + HFCode[i].codelen;
	}
	last = totalLen % 8;
	// 将Huffman编码写入头部，当作头文件方便译码操作。
	char head = '>';
	writecode.write((char*)&head, sizeof(char));
	writecode.write((char *)&codeNum, sizeof(int));
	writecode.write((char *)& last, sizeof(int));  //写入最后一次写入的位数
	for (int i = 0; i < codeNum; i++)
	{    //写入字符值和频数
		writecode.write((char*)&charCountFrequency[i].value, sizeof(unsigned char));
		writecode.write((char*)&charCountFrequency[i].frequency, sizeof(int));
	}
	//read.read((char*)now, 1);
	read.read((char*)now, sizeof(unsigned char));
	while (!read.eof())
	{
		int flag = 0;
		for (int i = 0; i < HFCode.size(); i++)
		{
			if (*now == HFCode[i].value)
			{
				flag = 1;
				for (int j = 0; j < HFCode[i].codelen; j++)
				{
					if (len != 0)
						save = save << 1;
					save = save | (HFCode[i].code[j] - '0');
					len++;
					if (len == 8)
					{
						writecode.write((char *)&save, sizeof(unsigned char));
						save = 0;
						len = 0;
					}
				}

			}
		}
		if (flag == 0)
		{
			cout << *now << "没在表中找到" << endl;
		}
		read.read((char*)now, sizeof(unsigned char));
		//*now = read.get();
	}
	if (len != 0)
	{
		writecode.write((char*)&save, sizeof(unsigned char));
	}

	read.close();
	writecode.close();

}

void Huffman::Decode(string sourcefile, string dstfile)
{
	ifstream read;
	ofstream write;
	vector<CountVector> arr;
	unsigned char now;  //读取的当前字符
	int last = 0;   //最后一次读取的位数
	int n; //字符种数
	read.open(sourcefile, ios::in | ios::binary);  //读取解码文件
	write.open(dstfile, ios::out | ios::binary); //打开解码后的文件
	read.read((char*)&now, sizeof(now));
	if (!(now == '>'))
	{
		cout << "该文件的Huffman编码格式不正确" << endl << endl;
		read.close();
		return;
	}
	read.read((char*)&n, sizeof(int));
	read.read((char*)&last, sizeof(last));

	for (int i = 0; i < n; i++)
	{
		CountVector *insert = new CountVector;

		read.read((char*)&(insert->value), sizeof(unsigned char));
		read.read((char*)&(insert->frequency), sizeof(int));
		arr.push_back(*insert);
	}
	this->root = new HuffmanNode;
	CreateHuffmanTree(arr);
	GetHuffmanCode(root, 0);
	HuffmanNode *pNow = root;

	unsigned char *temp = new unsigned char; //每次读一个字节
	read.read((char*)temp, sizeof(unsigned char));
	while (!read.eof())
	{
		unsigned char *ifLast = new unsigned char; //用于判断是否读到文件末尾
		read.read((char*)ifLast, sizeof(unsigned char));
		int i;
		if (read.eof())
		{
			i = last - 1;
		}
		else
		{
			i = 7;
		}
		for (; i >= 0; i--)
		{
			if ((*temp >> i & 1) == 0)   //向右移动7位判断读出的是0 还是1 
				pNow = pNow->Lchild;
			else
				pNow = pNow->Rchild;
			if (pNow->Lchild == NULL && pNow->Rchild == NULL)
			{
				write.write((char*)&(pNow->value), sizeof(unsigned char));
				pNow = root;
			}
		}
		temp = ifLast;
	}

	read.close();
	write.close();

}





