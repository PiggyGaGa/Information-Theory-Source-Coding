#include "fano.h"
#include <string>
#include <time.h>
int main()
{
	string filepath;
	cout << "请输入待压缩文件的地址" << endl << endl;
	getline(cin, filepath);
	clock_t start, end;
	start = clock();
	/*
	Fano myFano;
	myFano.open(filepath);
	myFano.count();
	myFano.root = new Fano::FanoNode;
	myFano.CreateTree(myFano.charFrequency, myFano.root);
	myFano.GetFanoCode(myFano.root, 0);
	myFano.WriteCode(myFano.FanoCodeVec);
	end = clock();
	cout << "压缩文件用时：" << double((end - start) / CLOCKS_PER_SEC) << "/s" << endl;
	*/
	Fano myfanoDecode;
	myfanoDecode.open(filepath);
	myfanoDecode.Decode(filepath + ".dada", "./Result/ " + filepath);
	end = clock();
	cout << "解压文件用时:" << double((start - end) / CLOCKS_PER_SEC) << "/s" << endl;
	getchar();

}