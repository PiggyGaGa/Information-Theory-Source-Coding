#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

class Fano
{

	
public:
	struct FanoNode
	{
		unsigned char value; // 字符
		struct FanoNode *Lchild = NULL;  //左孩子
		struct FanoNode *Rchild = NULL;   //右孩子
	};

private:
	struct CountVector
	{
		unsigned char value;
		int frequency = 0;
		struct FanoNode *nodeAddress = NULL;
	};

private:
	struct FanoCode
	{
		unsigned char value;
		int frequency;
		string code;
		int codelen;
	};
private:
	static bool mysortfunction(CountVector A, CountVector B)
	{  //排序函数
		return A.frequency > B.frequency;
	}
public:
	FanoNode *root;  //存储树的结构
	string fileAddress;
	long int NumOfChar;
	vector<CountVector> charFrequency;  //字符频率
	vector<FanoCode> FanoCodeVec;  //存储Fano码， 包括码长，码字
	Fano();
	void count();
	void open(string add);
	void CreateTree(vector<CountVector> charFrequency, FanoNode *rootNode);
	void GetFanoCode(FanoNode* root, int depth);
	void WriteCode(vector<FanoCode> HFCode); 
	void Decode(string sourcefile, string dstfile);
private:
	void splitVec(vector<CountVector> charFr, vector<CountVector> &charFr1, vector<CountVector> &charFr2);

};

Fano::Fano()
{
	NumOfChar = 0;
}
void Fano::open(string add)
{
	fileAddress = add;
}
void Fano::count()
{
	ifstream readfile;
	readfile.open(fileAddress, ios::in | ios::binary);
	unsigned char *now = new unsigned char;  //´æ´¢µ±Ç°¶ÁÈ¡µ½µÄ×Ö·û
	while (!readfile.eof())
	{
		CountVector *presentChar = new CountVector;  //¶ÁÈ¡µ½µÄ×Ö·ûÐÅÏ¢
		readfile.read((char*)now, sizeof(unsigned char));
		int flag = 0;   //±êÖ¾ÊÇ·ñÊÇÐÂ³öÏÖµÄ×Ö·û
		for (int i = 0; i < charFrequency.size(); i++)
		{
			if (*now == charFrequency[i].value)
			{
				charFrequency[i].frequency++;
				NumOfChar++;
				flag = 1;
			}

		}
		if (flag == 0)
		{
			presentChar->value = *now;
			presentChar->frequency++;
			NumOfChar++;
			charFrequency.push_back(*presentChar);
		}
	}
	readfile.close();
}

void Fano::CreateTree(vector<CountVector> charFr, FanoNode *rootNode)
{
	vector<CountVector> buildtree = charFr;
	if (buildtree.size() == 1)
	{
		//root->Lchild = new FanoNode;
		//root->Rchild = new FanoNode;
		rootNode->Lchild = NULL;
		rootNode->Rchild = NULL;
		rootNode->value = buildtree[0].value;
	
	}
	else
	{
		sort(buildtree.begin(), buildtree.end(), mysortfunction);
		vector<CountVector> charFr1, charFr2;
		splitVec(buildtree, charFr1, charFr2);
		rootNode->Lchild = new FanoNode;
		CreateTree(charFr1, rootNode->Lchild);
		rootNode->Rchild = new FanoNode;
		CreateTree(charFr2, rootNode->Rchild);
		rootNode->value = 0;
	}
	return;
}

void Fano::splitVec(vector<CountVector> charFr, vector<CountVector> &charFr1, vector<CountVector> &charFr2)
{
	int length = charFr.size();
	if (length == 1)
	{
		cout << "拆分的数组长度不够" << endl;
	}
	long int NumOfCharf = 0;
	for (int i = 0; i < length; i++)
	{
		NumOfCharf = NumOfCharf + charFr[i].frequency;

	}
	double ratio = 0;
	int splitIndex = 0;  //切割处的索引
	for (int i = 0; i < length; i++)
	{
		ratio = ratio + double(charFr[i].frequency / NumOfCharf);
		if (ratio > 0.5)
		{
			if (i > 0)
			{
				splitIndex = i - 1;
				break;
			}
			else
			{
				splitIndex = i;
				break;
			}
			
		}
	}

	for (int i = 0; i < splitIndex + 1; i++)
	{
		charFr1.push_back(charFr[i]);
	}
	for (int i = splitIndex + 1; i < charFr.size(); i++)
	{
		charFr2.push_back(charFr[i]);
	}
}

void  Fano::GetFanoCode(FanoNode* root, int depth)
{
	static char code[512];
	//ÅÐ¶Ï×ó¶ù×Ó
	if (root->Lchild != NULL)
	{
		code[depth] = '0';
		code[depth + 1] = '\0';
		GetFanoCode(root->Lchild, depth + 1);
	}
	if (root->Rchild != NULL)
	{
		code[depth] = '1';
		code[depth + 1] = '\0';
		GetFanoCode(root->Rchild, depth + 1);
	}
	else
	{
		FanoCode insertCode;
		int codelength = 0;
		for (int i = 0; i < charFrequency.size(); i++)
		{
			if (root->value == charFrequency[i].value)
			{
				insertCode.code = code;
				insertCode.value = charFrequency[i].value;
				insertCode.frequency = charFrequency[i].frequency;
			}
		}
		for (int j = 0; code[j] != '\0'; j++)
		{
			codelength++;
		}
		insertCode.codelen = codelength;
		FanoCodeVec.push_back(insertCode);

	}

}

void Fano::WriteCode(vector<FanoCode> HFCode)
{
	//读取文件并写入数据
	int codeNum = HFCode.size();
	string address = fileAddress;
	ofstream writecode;
	ifstream read;
	read.open(address, ios::in | ios::binary);   //以二进制方式读取
	writecode.open(address + ".dada", ios::out | ios::binary);   //以二进制方式写入
	unsigned char *now = new unsigned char; //存储字符值
	unsigned char save = 0;  //保存当前字符
	int len = 0;
	long int totalLen = 0; //总长
	int last; //结尾字符长度

	for (int i = 0; i < HFCode.size(); i++)
	{
		totalLen = totalLen + HFCode[i].codelen;
	}
	last = totalLen % 8;
	//
	char head = '>';
	writecode.write((char*)&head, sizeof(char));
	writecode.write((char *)&codeNum, sizeof(int));
	writecode.write((char *)& last, sizeof(int));  //Ð´Èë×îºóÒ»´ÎÐ´ÈëµÄÎ»Êý
	for (int i = 0; i < codeNum; i++)
	{    //Ð´Èë×Ö·ûÖµºÍÆµÊý
		writecode.write((char*)&HFCode[i].value, sizeof(HFCode[i].value));
		writecode.write((char*)&HFCode[i].frequency, sizeof(HFCode[i].frequency));
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
			cout << *now << "没有找到该字符属性" << endl;
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

void Fano::Decode(string sourcefile, string dstfile)
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
	this->root = new FanoNode;
	CreateTree(arr, root);
	GetFanoCode(root, 0);
	FanoNode *pNow = root;

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
