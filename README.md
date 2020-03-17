
# 实验目的
	1. 实现压缩编码算法——Huffman编码
	2. 实现压缩编码算法——Shannon Fano编码
	3. 实现压缩编码算法——LZ编码
	4. 实现压缩编码算法——算数编码
	5. 利用上述压缩算法压缩图像、音频、视频文件，分析压缩算法的性能。
** 先上源代码，如果对实验的源代码感兴趣的同学，请到小猪嘎嘎的仓库下载**
[信源编码源代码](https://github.com/PiggyGaGa/Information-Theory-Source-Coding/)

# 第一章：Huffman编码的实现
## Huffman编码原理
数据压缩是一门通信原理里和计算机科学都会涉及到的学科，在通信中，一般称为信源编码，在计算机科学里，通常称为数据压缩，两者没有本质区别，从数学角度看，都是映射。压缩可以分为有损压缩和无损压缩。有损压缩，指的是压缩之后无法还原原始信息，但是可以达到很高的压缩率，主要应用于视频、通话、图像等信息的传输领域。无损压缩则用于文本文件等必须完整还原信息的领域。
	Huffman编码是一种可变长编码(VLC:ariable length coding)方式，于1952年由huffman提出。依据字符在需要编码文件中出现的概率提供对字符的唯一编码，并且保证了可变编码的平均编码最短，被称为最优二叉树，有时又称为最佳编码。
## Huffman编码过程
### 统计各个字符出现的频率
当谈到统计字符这里，经过了一番折腾后在这里我有很多想要总结的，对于数学专业的学生来讲我觉得这里是一个很难跨越的坎，为什么呢？原因在于这里涉及到计算机的储存原理，数据的读取，存储操作，这些操作都非常接近计算机底层，就拿数据的存储来说，首先我们要搞明白什么是ASCII文件，什么是二进制文件，这两类文件读取有什么差别，在计算机里又是如何存储的，这里参考网上博客的内容多多理解才好，这里我折腾了很久，首先搞明白ASCII和二进制文件这两个概念(概念很重要)，然后分析差别，最后学习C++提供的API，比较两者的不同。而我们数学院的同学接触计算机也不少，但是接触底层的人很少，所以当谈到一些数据结构、文件存储、硬件编程等等就会赶脚很无助，因为平时大家最多的就是用Matlab实现一些算法，对底层的接触的很少。当然Huffman算法也可以由Matlab来实现，我也看到网上有一些实例程序，但怎么说呢，如果用Matlab实现了Huffman我只能说，我学会了Huffman算法，但是不能说我学会了Huffman编码。因为数据编码这东西最直接联系的就是数据传输，我们需要知道计算机是怎样实现文件压缩、传输的过程的，就必须深入底层了解它的本质。
	**数据的本质：0和1**
	在我弄不明白Huffman编码的时候，我就问自己这样一个问题：Huffman编码是用来做什么的？信息论教科书上、数据结构教科书、各种Huffman编码的解释，Huffman 编码是一种编码方式，是一种用于无损数据压缩的熵编码（权编码）算法等等各种解释感觉很厉害的样子。听着好像懂了，但又好像没懂。但是一边看别人代码，一边写自己代码的过程，我变得有些心虚，心想我这是在做什么? 然后停下敲打键盘，想了一些问题。别担心，绝对不是高大上的问题，都是特别特别弱智的问题，我说的是特别，对，老师您没听错。
	1. 什么是 ”编码” ? 是的没错，就是这么弱智的问题，学了这么久信息论、C语言和数据结构我的确问了自己一个这么弱智的问题。似乎很简单，但往往这么简单的问题我们都忽略去思考了，最后发现：哦，原来我一直干的是这样一件事。
	2. 什么是ASCII编码？什么是ASCII编码，好像大家都知道到0的ASCII值是48，A打得ASCII值是65，即便不知道每一个字符的ASCII值，也知道在任何一本C语言教材的最后几页肯定有一张印有ASCII编码的。具体什么是ASCII我在这里就不介绍了，网上有很多解释，教科书也有很多，关键是理解。
	3. Huffman为什么会出现？我觉的这个问题也弱智的很，但是这个问题在我写Huffman编码过程给了我很大的帮助。答案很简单：为了压缩数据。那么数据是什么？计算机里数据的本质就是0和1，那么怎么压缩0和1？当然0和1不能压缩，压缩的是0和1的组合。0和1的组合代表的是什么？ 组合代表的是信息，不同的组合代表不同的信息。回过头去想ASCII编码，ASCII是定长编码，原来Huffman编码实现的是传输同样信息的情况下，尽量使平均码长变得最小，这样就实现了数据压缩的目的。
回答完上面这些问题我才真正着手开始进行编程。
	上面说道数据的读取和存储是一个很难绕过去的坎，那么到底怎么绕过去？刚开始我在纠结一个txt文档和mp4文件在数据读取时有什么不同吗？答案是：有。txt文档就是我们讲的ASCII文件，每8bit一个字符，而mp4文件本质上又是图像文件。这里真的很难绕过来，因为以前接触过的C语言API大部分是读取字符或字符串，很少用二进制的方式读取。但是当这样想后就明白了：所有文件在计算机里存储的都是0和1。编码的时候不用关心每个字符占多少bit，按照自己的编码方式编，只要能恢复0和1数据即可。所以，所有数据都通过二进制的方式读取，以二进制的方式存储，这样就没有必要担心文件类型所带来的困惑了。
	具体字符统计这部分我写了一个count()函数。函数的输入是一个文件的地址，输出是文件中各个字符的统计向量。当然这个待压缩文件不能过大，因为通过读zip实现原理博客中我发现rar和zip等压缩软件之所以那么快是进行了很多优化，以我现在的时间和能力还搞不懂里面的一些东西，所以我写的代码运行很慢，但是结果是正确的，压缩效果也很好。函数具体的实现在后面算法和性能分析部分讲。
###　创建Huffman树(核心)
根据字符的频率按Huffman算法构建Huffman算法创建Huffman树
步骤如下：
1. 为每个符号建立一个叶子节点，并加上其相应的发生频率
2. 当有一个以上的节点存在时，进行下列循环:
把这些节点作为带权值的二叉树的根节点，左右子树为空
选择两棵根结点权值最小的树作为左右子树构造一棵新的二叉树，且至新的二叉树的根结点的权值为其左右子树上根结点的权值之和。
把权值最小的两个根节点移除
将新的二叉树加入队列中。
3. 最后剩下的节点暨为根节点，存储为root节点，用于后面压缩编码操作。
树的建立过程是从叶子节点往上走，直到根节点。
建树过程如下图所示

![](http://picture.piggygaga.top/InformationTheory/second/CreateTRee.png "CreateHuffmanTree")

绿色椭圆表示叶子节点，棕色节点表示根节点，蓝色椭圆表示中间节点，首先从最底层的叶子节点开始创建树，然后向上走，建树过程中每次建完树的节点信息要从数组中删除节点信息，方便后面的建树操作。每次删除两个节点会建立一个新的合并节点，然后重新按频率从小到大排序，执行同样的操作直到最后剩下根节点，将根节点地址保存下来，在下一步压缩数据时使用。
经过此步骤，每个字符对应的Huffman编码都可以得到。我们可以将编码信息保存下来，类似ASCII编码表一样，我们把Huffman编码表也保存下来。
### Huffman编码
根据Huffman 树实现编码并将编码结果和要编码的数据建立映射关系，存储的压缩文件需要添加头文件，用于解码使用。
重新从文件中读取信息，此时不需要进行统计字符频率，只需要在Huffman表中找到对应的Huffman编码然后将编码压入压缩文件中。
压缩文件包括两部分：头文件，数据部分。压缩文件的结构见下图
![](http://picture.piggygaga.top/InformationTheory/second/Huffmanfile.PNG "压缩文件内容")
压缩数据首先以一个>开始表示这是一个Huffman压缩数据，这样做的目的是在解压过程会判断这是否是一个Huffman编码文件，如果没有这个标识，认定该文件不具备Huffman编码文件的特性性，也就没有办法解压。第二部分是头文件，存储着字符频数，这样做也是为了解压方便，解压过程和压缩过程是两个独立的过程，将字符频数存进头文件，在解压时需要利用头文件字符频数重新建立一棵Huffman树，和压缩过程的操作过程是一样的。最后通过读取源文件的字符，在Huffman编码表中找到对应的字符编码，将字符编码压进数据部分，因为Huffman编码是异字头码，所以不需要做分隔，把所有数据压进去即可。
### Huffman 解码
根据获取的Huffman码来逆向获取编码信息，而且从解压文件中一次性获取的数据是一个很长的字符串，这个串是压缩后的huffman编码，实际上是机器码。
解码过程
![](http://picture.piggygaga.top/InformationTheory/second/decode.png "解码过程")
解码过程共分为3部分，首先程序读取压缩文件的头部获取字符频数，并据此构建Huffman树，其次读取数据部分的Huffman编码，并根据Huffman树得到编码的字符值，最后将字符压入解压文件中，如果读到文件结尾，结束解码过程。

## Huffman编码算法C++实现和性能分析
编程语言：C++
编程环境：Visual studio 2015
本次实验我采用面向对象的编程思想，每种算法建立一个对象，Huffman编码我建立了一个Huffman类。所有的编码操作都是基于这个类。
Huffman类如下所示
```
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
```
程序调用过程中只会用到公有属性和公有函数，所以下面依次介绍公有属性和公有函数的功能。
1. root 是HuffmanNode类型的指针，用来存储Huffman树的根节点的地址。
2. fileAddress 是string类型字符串，用来存储待压缩文件的文件路径。
3. NumOfCahr 是long int 类型的数据，表示文件中字符总数。
4. charCountFrequency 是CountVector 类型的数组，存储每种字符的频率。
5. HuffmanCodeVec 是HuffmanCode 类型的数组，存储每种字符的Huffman编码。
6. Huffman() 是构造函数，用来初始化对象。
7. count() 函数统计各个字符出现的频数， 结果存在charCountFrequency中。
8. CreateHuffmanTree() 构造Huffman树，结果存储在root中。
9. GetHuffmanCode() 通过Huffman树获取Huffman编码。
10. WriteCode() 文件压缩函数，将原始文件的信息压缩为拓展名为.dada的文件。
11. Decode() 文件解码函数，输入一个Huffman编码后的.dada文件，输出原始文件。
**主函数部分如下图：**
```
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
```
文件压缩步骤：
第一步：读入待压缩的文件名
第二步：建立huf对象为Huffman类型
第三步：cout()函数计算各个字符频数
第四步：CreatehuffmanTree()建立Huffman树
第五步：GetHuffmanCode()获取Huffman编码
第六步：WriteCode()压缩文件
第七步：Decode() 解码

1. 文本压缩
原始文本为一个名为haha.txt的文本文档，该文档大小为4096 bytes
![](http://picture.piggygaga.top/InformationTheory/second/hahaSource.PNG "文本文档")
压缩后的文件为一个二进制文件，用二进制查看软件打开后是乱码文件
![](http://picture.piggygaga.top/InformationTheory/second/yasuo.PNG "二进制查看器查看")
文件大小为10385Bytes，等等，10385Bytes，为啥变大了？不是压缩么？
这个现象后面解释，下面继续看解码效果。
解码文件为out.txt文件
![](http://picture.piggygaga.top/InformationTheory/second/hahaSource.PNG "解码文件")
我们可以看到解码文件和原始文件一样，所以正确性没有任何问题。
上面说道压缩后的文件反而比原来的文件大，其实这并不奇怪，因为压缩文件比原始文件多了数据头部，head部分也是会占用一定的空间的，所以才会产生这种情况，所以Huffman编码不适合文件很小的数据压缩，数据要大一些才会有明显差异。经过实验测试文件大于1M后才会有压缩效果。
2. 图像压缩
测试图片是功夫熊猫的一张图片
![](http://picture.piggygaga.top/InformationTheory/second/Abao.jpg "待压缩图像")
图片大小为47kb，压缩后的图像为48kb，因为图片太小所以还是没有明显的压缩效果。图像文件之所以压缩效果不好是因为图像格式本身已经是经过压缩后的文件，已经用Huffman压缩算法或其他压缩算法压缩过了，不同的图像格式有不同的压缩算法，一般通常是集中算法混合使用，所以根据信息论理论，当压缩的文件接近最佳压缩比时此时无论怎样做都无法进行更优的无损压缩了，注意这里必须是无损压缩，有损压缩还是可以继续进行的，这里Huffman是一种无损压缩算法。
压缩用时：
![](http://picture.piggygaga.top/InformationTheory/second/KongFutime.PNG "压缩用时")
总共有256个字符被统计出来，压缩数据耗时25000ms。解码用时几乎为0ms，所以可以看出解码过程不需要统计字符频数，速度相当快。
3. 视频压缩
视频压缩的是《当爱已成往事》mv
![](http://picture.piggygaga.top/InformationTheory/second/Love.PNG "love")
文件大小为7.82kb，压缩后的文件大小为：7.81压缩率为99.8%
综上；程序可以对所有文件进行压缩，当原始文件小于7M时，压缩文件大于原始文件，并不能实现压缩效果，当文件达到13M时压缩比率可达到77%，且当文件越大压缩效果越好
## Huffman算法实现过程中的心得体会
解码过程出现了一些问题，开始出现文件错误的信息，后来调试发现root节点没有赋予初始的空间，使得程序崩溃。后来解决这一问题后又出现了解码文件乱码的现象，这个问题真的是很坑，后来找到原因是：编码和解码过程中建的树不一致，导致编码和解码的字符编码不能对应产生了乱码现象。现在程序可以完成独立的编码，独立的解码工作。总结一下Huffman编码，通过程序实现的Huffman可以达到无损压缩的目的，且文件越大压缩的效果越好，当文件大于7M左右时可以达到压缩的目的，文件小于7M左右时并不能达到压缩目的。文件很大时可以达到很高的压缩比例。但Huffman有一个很致命的缺点：很耗时。每次编码都要统计字符频数，在统计字符频数的时候要遍历一次文件，然后利用字符频数向量建立Huffman树，此后还要遍历一次文件来压缩文件，所以Huffman编码非常耗时，主要时间花费在统计字符频数那里。
# 第二章：Shannon Fano编码
## 一. Shannon Fano编码原理
Fano编码和Huffman编码稍有不同，它不是最佳编码方法，但有时也可以得到最佳的性能。Huffman是由叶子节点合并构建Huffman树，利用的是合并的思想，而Fano编码方法正好相反，Fano编码是从整体进行分割，到最后叶子节点结束。
	首先，将信源符号以概率低贱的次序排列起来，将排列好的信源符号划分成两大组，使魅族的概率和近于相同，并各赋予一个二元吗符号‘0’和‘1’.然后，将每一大组的信源符号再分成两组，使同一组的两个小组的概率近于相同，并分别赋予-个二元码符号。依次下去，直至每个小组只剩一个心愿符号为止。最后，由前向后读取码符号序列。这样，信源符号所对应的码符号序列则为编得的码字。

## 二. Shannon Fano 编码过程
1. 字符频率统计
和Huffman编码类似Shannon Fano 编码同样也要经过字符统计。
2. 构建Fano树
	这里是和Huffman编码不一样的地方，Huffman是由叶子节点向树根的方向逐步构造Huffman编码树，而Shannon Fano 编码是从树根逐渐拆分执行递归操作，最后生成一颗编码树。
3. 获取Fano编码
4. 译码
## 二. Shannon Fano 编码的实现和性能分析
Shannon Fano编码通过建立一个Fano类实现。
下面是Fano类的具体定义如下：
```
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
 
```
关键属性：
1. struct FanoNode : Shannon Fano树的节点数据
2. struct CountVector : 用于存储字符频数的数据类型
3. FanoCode : 存储Shannon Fano编码的数据类型
4. FanoNode *root : 存储Fano树的根节点
5. string fileAddress : 待压缩文件的路径
6. vector<CountVector> charFrequency: 用于存储所有字符频数的向量
7. vector<FanoCode> FanoCodeVec : 用于存储所有字符的Shannon Fano编码
关键函数：
1. void open(string fileaddress) 打开待压缩的文件
2. void cout() 获取文件中所有字符的字符频数。
3. void CreateTree(vector<CountVector> charFrequency, FanoNode *rootNode)  获取Fano树
4. void GetFanoCode(FanoNode* root, int depth) 获取Fano编码
5. void WriteCode(vector<FanoCode> HFCode) 压缩文件
6. void Decode(string sourcefile, string dstfile) 文件解压。           
压缩比例
| | | | |
|:-:|:-:|:-:|:-:|
|原始文件	|890Bytes(文本)|46.7kb(图片)	|7.82M(视频)|
|压缩文件|	1146Bytes|	47.9kb	|7.81M|
|压缩率	|1.29	|1.025|	99.8%|
|压缩用时	|0/s	|27/s|	6400/s|
|解压用时|	0/s	|1/s	|2400/s|

由于程序设置问题，当文件大于13M后会堆栈溢出，这里后续需要调整程序进行优化。
## 三. Shannon Fano编码总结
Shannon Fano编码和Huffman编码比较类似，编码过程也没有太大区别。唯一的区别在结果。Shannon Fano相比于Huffman编码，其压缩速率没有差异，原因在于两者都需要知道每种字符的概率信息，所以在编码之前必须统计每种字符的频数。另一方面，Shannon 平均压缩比例要比Huffman的低一些，虽然有时Shannon Fano可以达到最优编码，但是大部分情况是不能达到的，所以其平均压缩比例要略低于Huffman编码。
由于篇幅有限，下一篇博客介绍LZ编码和算数编码，最后附上Huffman编码和Fano编码的源代码

**HuffmanClass.h**

```
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

```

**HuffmanMain.cpp**
```
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
`
	getchar();
}

```

**ShannonFano.h**
```
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
```
**Fano.cpp**
```
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
```
