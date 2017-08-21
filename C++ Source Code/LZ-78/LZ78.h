#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class LZ78
{
public:
	struct Dictionary
	{
		unsigned int Index;
		int preIndex;
		unsigned char lastChar;
		vector<unsigned char> stringInDic;
	};
public:
	struct OutData
	{
		unsigned int preIndex;
		unsigned char lastChar;
	};
public:
	string fileAddress;
	LZ78();   //构造函数
	void open(string);
	void Press();
	void Decode(string sourcefile, string dstfile);
private:
	bool IfStringInDic(vector<Dictionary> CurrentString, vector<unsigned char> DataDic, unsigned int &Index);
private:
	vector<unsigned char> FindPreString(vector<Dictionary> DataDic, unsigned int);
};

LZ78::LZ78()
{

}
void LZ78::open(string input)
{
	fileAddress = input;
}

void LZ78::Press()
{
	ifstream read;
	read.open(fileAddress, ios::in|ios::binary);
	if (!read )
	{
		cout << "文件读取错误" << endl << endl;
		return;
	}
	ofstream write;
	write.open(fileAddress + ".lz", ios::out|ios::binary);
	if (!write)
	{
		cout << "输出文件不能建立(*.lz)" << endl << endl;
	}
	
	unsigned char *firstchar = new unsigned char;
	read.read((char*)firstchar, sizeof(unsigned char));
	vector<Dictionary> DataDic;    //建立字典
	while (!read.eof())
	{
		
		if (DataDic.size() == 0)
		{
			Dictionary firstDic;
			OutData *firstout = new OutData;
			firstDic.Index = 1;
			firstDic.preIndex = 0;
			firstDic.lastChar = *firstchar;
			firstDic.stringInDic.push_back(*firstchar);
			DataDic.push_back(firstDic);
			firstout->lastChar = *firstchar;
			firstout->preIndex = 0;
			write.write((char*)firstout, sizeof(OutData));
		}
		else
		{
			unsigned char *now = new unsigned char; //用于读取的字符
			unsigned char *reallast = new unsigned char;
			vector<unsigned char> CurrentString;
			unsigned int index = 0;  //字符串存在在字典中的位置， 初始设为0
			Dictionary currentInfo;   //存储当前的单词到字典中
			OutData *currentOutdata = new OutData;  //存储当前编码信息，一会压缩进如压缩文件。
			
			int EOFflag = 0;
			do
			{
				read.read((char*)now, sizeof(unsigned char));
				if (read.eof())
				{
					EOFflag = 1;  //标记是否到文件的结尾
					break;
				}
				else
				{
					CurrentString.push_back(*now);
				}
			} 
			while (IfStringInDic(DataDic, CurrentString, index));


			if (EOFflag == 1)
			{
				if (CurrentString.size() == 0)
				{
					break;  //如果当前字符串中没有字符，直接跳出循环
				}
				else
				{     //如果当前字符串中有字符，对这段字符进行压缩。
					*reallast = CurrentString[CurrentString.size() - 1];
					CurrentString.erase(CurrentString.end() - 1);
					IfStringInDic(DataDic, CurrentString, index);

				}
				
			}
			else
			{
				*reallast = *now;
			}
			currentInfo.Index = DataDic.size() + 1;
			currentInfo.lastChar = *reallast;
			currentInfo.preIndex = index;
			currentInfo.stringInDic = CurrentString;
			DataDic.push_back(currentInfo);

			currentOutdata->lastChar = *reallast;
			currentOutdata->preIndex = index;
			write.write((char*)currentOutdata, sizeof(OutData));
			
		}

	}

	read.close();
	write.close();
}

bool LZ78::IfStringInDic(vector<Dictionary> DataDic, vector<unsigned char> CurrentString, unsigned int &Index)
{
	int flag = 0;
	for (int i = 0; i < DataDic.size(); i++)
	{
		if (CurrentString == DataDic[i].stringInDic)
		{
			Index = DataDic[i].Index;
			flag = 1;
			return true;
		}
		
	}
	if (flag == 0)
	{
		return false;
	}
}


void LZ78::Decode(string sourcefile, string dstfile)
{
	ifstream readfile;
	ofstream putfile;
	readfile.open(sourcefile, ios::in | ios::binary);
	putfile.open(dstfile, ios::out | ios::binary);
	OutData *getdata = new OutData;
	readfile.read((char*)getdata, sizeof(OutData));
	vector<Dictionary> DataDic;    //建立字典
	Dictionary *spacefirst = new Dictionary;
	spacefirst->Index = 0;
	spacefirst->lastChar = '0';
	spacefirst->preIndex = 0;
	//spacefirst->stringInDic
	DataDic.push_back(*spacefirst);
	while (!readfile.eof())
	{
		Dictionary *now = new Dictionary;
		now->lastChar = getdata->lastChar;
		now->Index = DataDic.size();
		now->preIndex = getdata->preIndex;
		vector<unsigned char> preString;  //存储前一个的字符串
		if (now->preIndex != 0)
		{ //如果preIndex等于0那么此字符是新出现的字符，否则在前面字典中找
			preString = FindPreString(DataDic, now->preIndex);
		}
		preString.push_back(now->lastChar);  
		now->stringInDic = preString;  //获取此单词的字符串。
		DataDic.push_back(*now);
		for (int i = 0; i < preString.size(); i++)
		{
			putfile.write((char*)&preString[i], sizeof(unsigned char));
		}
		readfile.read((char*)getdata, sizeof(OutData));
		
	}
	readfile.close();
	putfile.close();
}

vector<unsigned char>  LZ78::FindPreString(vector<Dictionary> DataDic, unsigned int preindex)
{
	if (DataDic.size() < 1)
	{
		cout << "不能找到前一个字符串" << endl;
	}
	else
	{
		for (int i = 0; i < DataDic.size(); i++)
		{
			if (preindex == DataDic[i].Index)
			{
				return DataDic[i].stringInDic;
			}
		}
	}
}