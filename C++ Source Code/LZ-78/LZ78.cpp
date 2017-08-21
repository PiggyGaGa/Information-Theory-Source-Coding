#include "LZ78.h"
#include <string>
#include <time.h>
int main()
{
	LZ78 haha;
	clock_t start, end;
	start = clock();
	haha.open("./KongFu.jpg");  //打开文件
	haha.Press();   //压缩文件
	end = clock();
	cout << "压缩文件用时：" << endl << endl;
	cout << double((end - start) / CLOCKS_PER_SEC) << "/s" << endl << endl;
	start = clock();
	LZ78 nothaha;
	nothaha.Decode("./KongFu.jpg.lz", "KongFuout.jpg");
	cout << "解压用时：" << endl << endl;
	cout << double((start - end) / CLOCKS_PER_SEC) << "/s" << endl << endl;
	getchar();
}