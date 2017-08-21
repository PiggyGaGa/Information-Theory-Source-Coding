#include <string>
#include "huffmanClass.h"
#include <time.h>
int main()
{
	clock_t start, end, start1, end1;
	cout << "！-------------Huffman压缩编码---------！" << endl << endl;
	cout << "!--------------作者：小猪嘎嘎------------！" << endl << endl;
	cout << "!--------------压缩程序----------------! " << endl << endl;
	cout << "!--------------csdn-------! " << endl << endl;
	string filePath;
	cout << "请输入待编码文件地址" << endl << endl;
	getline(cin, filePath);
	Huffman huf(filePath);
	start = clock();
	huf.count();  //获取字符频数存在charCountFrequency数组中
	cout << huf.charCountFrequency.size() << endl;
	//getchar();
	huf.CreateHuffmanTree(huf.charCountFrequency);
	huf.GetHuffmanCode(huf.root, 0);
	huf.WriteCode(huf.HuffmanCodeVec);
	end = clock();
	cout << "压缩使用时间为 :   " << double((end - start) / CLOCKS_PER_SEC) * 1000 << " /ms" << endl << endl;
	
	cout << "!--------------解码程序------------!" << endl << endl;
	//cout << "!--------------请输入待解码的文件--------------!" << endl << endl;
	//string outfilePath;
	//getline(cin, outfilePath);
	//Huffman hufdecode(outfilePath);
	//huf.root = new Huffman::HuffmanNode;
	start1 = clock();
	Huffman hufde(filePath);
	hufde.Decode(filePath + ".dada", "./Out/" + filePath);
	end1 = clock();
	cout << "解码使用时间为 :   " << double((end1 - start1) / CLOCKS_PER_SEC) * 1000 << " /ms" << endl << endl;

	getchar();
}