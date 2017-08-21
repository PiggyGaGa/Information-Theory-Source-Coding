#include <string>
#include <cstring>
#include <vector>
#include <iostream>
using namespace std;
#define N 100 //输入的字符应该不超过50个
struct L //结构用于求各字符及其概率
{
	char ch; //存储出现的字符（不重复）
	int num; //存储字符出现的次数
	double f;//存储字符的概率
};
//显示信息
void disp();
//求概率函数，输入：字符串；输出：字符数组、字符的概率数组;返回：数组长度； int proba(string str,char c[],long double p[],int count);
//求概率的辅助函数
int search(vector<L> arch, char, int n);
long double bma(char c[], long double p[], string str, int number, int size);
int proba(string str, char c[], long double p[], int count);
//编码函数，输入：字符串，字符数组，概率数组，以及数组长度；输出：编码结果 long double bma(char c[],long double p[],string str,int number,int size);
//译码函数，输入：编码结果，字符串，字符数组，概率数组，以及它们的长度；输出：字符串
//该函数可以用于检测编码是否正确
void yma(string str, char c[], long double p[], int number, int size, long double input);


int main()
{
	string str; //输入要编码的String类型字符串
	int number = 0, size = 0; //number--字符串中不重复的字符个数；size--字符串长度 
	char c[N]; //用于存储不重复的字符
	long double p[N], output; //p[N]--不重复字符的概率，output--编码结果 
	disp();
	cout << "输入要编码的字符串:";
	getline(cin, str); //输入要编码的字符串
	size = str.length(); //字符串长度
	number = proba(str, c, p, size);//调用求概率函数，返回不重复字符的个数
	cout.setf(ios::fixed); //“魔法配方”规定了小数部分的个数
	cout.setf(ios::showpoint); //在此规定编码结果的小数部分有十个
	cout.precision(10);//调用编码函数，返回编码结果
	output = bma(c, p, str, number, size);//调用译码函数，输出要编码的字符串，
	yma(str, c, p, number, size, output); //以验证编码是否正确	
	getchar();
	return 0;
}

//显示信息

void disp()

{
	cout << endl;
	cout << "!-------------------算术编码--------------------!\n";
	cout << "!-------------------作者：小猪嘎嘎----------------!\n";
	cout << "!---------------               --------------!\n";
	cout << endl;
	cout << "此程序只需要输入要编码的字符串，不需要输入字符概率\n";
	cout << endl;

}

//求概率函数

int proba(string str, char c[], long double p[], int count)

{

	cout.setf(ios::fixed); //“魔法配方”规定了小数部分位数为三位
	cout.setf(ios::showpoint);
	cout.precision(3);
	vector<L>pt; //定义了结构类型的向量，用于同时存储不重复的字符和其概率
	L temp; //结构类型的变量
	temp.ch = str[0]; //暂存字符串的第一个字符，它的个数暂设为1
	temp.num = 1;
	temp.f = 0.0;
	pt.push_back(temp); //将该字符及其个数压入向量
	for (int i = 1; i<count; i++)//对整个字符串进行扫描

	{
		temp.ch = str[i]; //暂存第二个字符
		temp.num = 1;
		temp.f = 0.0;
		for (int j = 0; j<pt.size(); j++) //在结构向量中寻找是否有重复字符出现
		{ //若重复，该字符个数加1，并跳出循环 
			int k; //若不重复，则压入该字符，并跳出循环 
			k = search(pt, str[i], pt.size());
			if (k >= 0)

			{
				pt[k].num++;
				break;
			}
			else
			{
				pt.push_back(temp);
				break;
			}

		}

	}

	for (int i = 0; i<pt.size(); i++) //计算不重复字符出现的概率

	{
		pt[i].f = double(pt[i].num) / count;
	}
	int number = pt.size(); //计算不重复字符出现的次数
	cout << "各字符概率如下：\n";
	for (int i = 0; i<number; i++) //显示所得的概率，验证是否正确
	{
		if (count == 0)
		{
			cout << "NO sample!\n";
		}
		else
		{
			c[i] = pt[i].ch;
			p[i] = pt[i].f;
			cout << c[i] << "的概率为：" << p[i] << endl;
		}
	}
	return number; //返回不重复字符的个数

}
//求概率的辅助函数
//若搜索发现有重复字符返回正数
//否则，返回-1
int search(vector<L> arch, char ch1, int n)
{
	for (int i = 0; i<n; i++)
	{
		if (ch1 == arch[i].ch) return i;
	}
return -1;
}
//编码函数
long double bma(char c[], long double p[], string str, int number, int size)
{
	long double High = 0.0, Low = 0.0, high, low, range;
	//High--下一个编码区间的上限，Low--下一个编码区间的下限；
	//high--中间变量，用来计算下一个编码区间的上限；
	//low--中间变量，用来计算下一个编码区间的下限；
	//range--上一个被编码区间长度
	int i, j = 0;
	for (i = 0; i<number; i++)
	{
		if (str[0] == c[i]) break; //编码第一个字符
	}
	while (j<i)
	{
		Low += p[j++]; //寻找该字符的概率区间下限
	}
	range = p[j]; //得到该字符的概率长度
	High = Low + range; //得到该字符概率区间上限
	for (i = 1; i<size; i++) //开始编码第二个字符
	{
		for (j = 0; j<number; j++) //寻找该字符在c数组中的位置
		{
			if (str[i] == c[j])
			{
				if (j == 0) //若该字符在c数组中的第一个字符
				{
					low = Low; //此时该字符的概率区间下限刚好为零 
					high = Low + p[j] * range;
					High = high;
					range *= p[j]; //求出该字符的编码区间长度
				}
				else //若该编码字符不是c数组中的第一个 
				{
					float proba_next = 0.0;
					for (int k = 0; k <= j - 1; k++)
						proba_next += p[k]; //再次寻找字符的概率区间下限
					low = Low + range*proba_next; //编码区间下限 
					high = Low + range*(proba_next + p[j]);//编码区间上限
					Low = low; //编码区间下限 
					High = high; //编码区间上限 
					range *= p[j]; //编码区间长度
				}
			}
			else continue; //i++，编码下一个字符 
		}
	}
	cout << endl;
	cout << "输入字符串的编码为:" << Low << endl;
	return Low;
}
//译码函数
void yma(string str, char c[], long double p[], int number, int size, long double input)
{
	vector<char> v; //定义char类型向量v
	long double temp; //中间变量
	long double sum[N]; //存储不重复字符概率区间的下限
	sum[0] = 0.0; //数组第一个元素为0
	for (int i = 1; i<number + 1; i++) //计算数组各元素的值
	{
		sum[i] = sum[i - 1] + p[i - 1];
	}
	for (int j = 0; j<size; j++)
	{
		for (int k = 0; k<number; k++)
		{
			//确定被编码字符的下限属于【0，1】之间的哪一段 
			if ((input>sum[k]) && (input<sum[k + 1])) //发现在哪就将属于该段的字符压入向量v

			{
				v.push_back(str[j]);
				temp = (input - sum[k]) / (sum[k + 1] - sum[k]);//计算下一个被编码字符的下限 
				input = temp;
				break;
			}
			else
				continue;
		}
	}
	cout << endl;
	cout << "译码输出为："; //将译码结果输出
	for (int m = 0; m<v.size(); m++)
	{
		cout << v[m];
	}
	cout << endl;
}
