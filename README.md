<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=default"> </script>
# 实验目的
    1. 实现压缩编码算法——Huffman编码
    2. 实现压缩编码算法——Shannon Fano编码
    3. 实现压缩编码算法——LZ编码
    4. 实现压缩编码算法——算数编码
    5. 利用上述压缩算法压缩图像、音频、视频文件，分析压缩算法的性能。

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
![CreateHuffmanTree](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/CreateTRee.png "CreateHuffmanTree")
绿色椭圆表示叶子节点，棕色节点表示根节点，蓝色椭圆表示中间节点，首先从最底层的叶子节点开始创建树，然后向上走，建树过程中每次建完树的节点信息要从数组中删除节点信息，方便后面的建树操作。每次删除两个节点会建立一个新的合并节点，然后重新按频率从小到大排序，执行同样的操作直到最后剩下根节点，将根节点地址保存下来，在下一步压缩数据时使用。
经过此步骤，每个字符对应的Huffman编码都可以得到。我们可以将编码信息保存下来，类似ASCII编码表一样，我们把Huffman编码表也保存下来。
### Huffman编码
根据Huffman 树实现编码并将编码结果和要编码的数据建立映射关系，存储的压缩文件需要添加头文件，用于解码使用。
重新从文件中读取信息，此时不需要进行统计字符频率，只需要在Huffman表中找到对应的Huffman编码然后将编码压入压缩文件中。
压缩文件包括两部分：头文件，数据部分。压缩文件的结构见下图
![压缩文件的内容](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/Huffmanfile.PNG "压缩文件内容")
压缩数据首先以一个>开始表示这是一个Huffman压缩数据，这样做的目的是在解压过程会判断这是否是一个Huffman编码文件，如果没有这个标识，认定该文件不具备Huffman编码文件的特性性，也就没有办法解压。第二部分是头文件，存储着字符频数，这样做也是为了解压方便，解压过程和压缩过程是两个独立的过程，将字符频数存进头文件，在解压时需要利用头文件字符频数重新建立一棵Huffman树，和压缩过程的操作过程是一样的。最后通过读取源文件的字符，在Huffman编码表中找到对应的字符编码，将字符编码压进数据部分，因为Huffman编码是异字头码，所以不需要做分隔，把所有数据压进去即可。
### Huffman 解码
根据获取的Huffman码来逆向获取编码信息，而且从解压文件中一次性获取的数据是一个很长的字符串，这个串是压缩后的huffman编码，实际上是机器码。
解码过程
![解码过程](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/decode.png "解码过程")
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
![文本文档](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/hahaSource.PNG "文本文档")
压缩后的文件为一个二进制文件，用二进制查看软件打开后是乱码文件
![二进制查看器查看](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/yasuo.PNG "二进制查看器查看")
文件大小为10385Bytes，等等，10385Bytes，为啥变大了？不是压缩么？
这个现象后面解释，下面继续看解码效果。
解码文件为out.txt文件
![解码文件](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/hahaSource.PNG "解码文件")
我们可以看到解码文件和原始文件一样，所以正确性没有任何问题。
上面说道压缩后的文件反而比原来的文件大，其实这并不奇怪，因为压缩文件比原始文件多了数据头部，head部分也是会占用一定的空间的，所以才会产生这种情况，所以Huffman编码不适合文件很小的数据压缩，数据要大一些才会有明显差异。经过实验测试文件大于1M后才会有压缩效果。
2. 图像压缩
测试图片是功夫熊猫的一张图片
![待压缩图像](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/Abao.jpg "待压缩图像")
图片大小为47kb，压缩后的图像为48kb，因为图片太小所以还是没有明显的压缩效果。图像文件之所以压缩效果不好是因为图像格式本身已经是经过压缩后的文件，已经用Huffman压缩算法或其他压缩算法压缩过了，不同的图像格式有不同的压缩算法，一般通常是集中算法混合使用，所以根据信息论理论，当压缩的文件接近最佳压缩比时此时无论怎样做都无法进行更优的无损压缩了，注意这里必须是无损压缩，有损压缩还是可以继续进行的，这里Huffman是一种无损压缩算法。
压缩用时：
![压缩用时](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/KongFutime.PNG "压缩用时")
总共有256个字符被统计出来，压缩数据耗时25000ms。解码用时几乎为0ms，所以可以看出解码过程不需要统计字符频数，速度相当快。
3. 视频压缩
视频压缩的是《当爱已成往事》mv
![当爱已成往事](http://or6i73qhr.bkt.clouddn.com/InformationTheory/second/Love.PNG "love")
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
|原始文件   |890Bytes(文本)|46.7kb(图片)    |7.82M(视频)|
|压缩文件|  1146Bytes|  47.9kb  |7.81M|
|压缩率    |1.29   |1.025| 99.8%|
|压缩用时   |0/s    |27/s|  6400/s|
|解压用时|  0/s |1/s    |2400/s|

由于程序设置问题，当文件大于13M后会堆栈溢出，这里后续需要调整程序进行优化。
## 三. Shannon Fano编码总结
Shannon Fano编码和Huffman编码比较类似，编码过程也没有太大区别。唯一的区别在结果。Shannon Fano相比于Huffman编码，其压缩速率没有差异，原因在于两者都需要知道每种字符的概率信息，所以在编码之前必须统计每种字符的频数。另一方面，Shannon 平均压缩比例要比Huffman的低一些，虽然有时Shannon Fano可以达到最优编码，但是大部分情况是不能达到的，所以其平均压缩比例要略低于Huffman编码。

上一篇文章给出了Huffman编码和Shannon Fano编码的编码原理以及C++的程序，程序可以用来实现给任意类型的文件进行无损压缩，缺点是比较耗时，不能作为正常的通用压缩软件来使用，但是作为算法理解，算法思路是没有问题的，后续可能需要进行优化，下面的LZ编码和算数编码和Huffman、Fano编码是走的截然不同的道路，他们的思想差别很大，但却殊途同归，**在算法理解上我借助了一些网友前辈的博客中的例子，文章最后我也会指出引用了那些文章，谢谢前辈们给出的经典的例子能让晚辈站在巨人的肩膀上，所以在此声明，希望我的引用不会侵犯到前辈们的权益**
# 第三章：算数编码的实现
## 一. 算数编码的原理
算数编码不同于Huffman编码，它是非分组(非块)码。它从全序列出发，考虑符号之间的依赖关系来进行编码的。
算数编码主要的编码方法正是计算输入信源符号序列所对应的区间。术编码的基本原理是将编码的消息表示成实数0和1之间的一个间隔（Interval），消息越长，编码表示它的间隔就越小，表示这一间隔所需的二进制位就越多。
算术编码用到两个基本的参数：符号的概率和它的编码间隔。信源符号的概率决定压缩编码的效率，也决定编码过程中信源符号的间隔，而这些间隔包含在0到1之间。编码过程中的间隔决定了符号压缩后的输出。
给定事件序列的算术编码步骤如下：
（1）编码器在开始时将“当前间隔” [ L， H) 设置为[0，1)。
（2）对每一事件，编码器按步骤（a）和（b）进行处理
（a）编码器将“当前间隔”分为子间隔，每一个事件一个。
（b）一个子间隔的大小与下一个将出现的事件的概率成比例，编码器选择子间隔对应于下一个确切发生的事件相对应，并使它成为新的“当前间隔”。
（3）最后输出的“当前间隔”的下边界就是该给定事件序列的算术编码。
设Low和High分别表示“当前间隔”的下边界和上边界，CodeRange为编码间隔的长度，LowRange(symbol)和HighRange(symbol)分别代表为了事件symbol分配的初始间隔下边界和上边界。算术编码也是一种对错误很敏感的编码方法，如果有一位发生错误就会导致整个消息译错。 
算术编码可以是静态的或者自适应的。在静态算术编码中，信源符号的概率是固定的。在自适应算术编码中，信源符号的概率根据编码时符号出现的频繁程度动态地 进行修改，在编码期间估算信源符号概率的过程叫做建模。需要开发动态算术编码的原因是因为事先知道精确的信源概率是很难的，而且是不切实际的。当压缩消息 时，我们不能期待一个算术编码器获得最大的效率，所能做的最有效的方法是在编码过程中估算概率。因此动态建模就成为确定编码器压缩效率的关键。
## 二. 算数编码过程
假设有信源符号{A, B. C, D}, 对应的概率和初始编码间隔如下表所示
| | | | | | |
|:-:|:-:|:-:|:-:|:-:|
|符号|    A|   B| C   |D|
|概率 |01.    |0.4    |0.2    |0.3|
|初始编码间隔 |[0, 0.1)   |[0.1, 0.5)|    [0.5, 0.7)| [0.7, 1)|
如果二进制消息序列的输入为：CADACDB。编码时首先输入的符号是C， 找到他的编码范围是[0.5, 0.7)。由于消息由于消息中第二个符号A的编码范围是[0, 0.1)，因此它的间隔就取[0.5， 0.7)的第一个十分之一作为新间隔[0.5, 0.52)。依此类推，编码第3个符号D时取新间隔为[0.514, 0.52)，编码第4个符号A时，取新间隔为[0.514, 0.5146)，…。消息的编码输出可以是最后一个间隔中的任意数
编码过程如下表所示
表3.2 算数编码的编码过程
|步骤 |   输入符号    |编码间隔 | 编码判决|
|:-:|:-:|:-:|:-:|
|1  |C| [0.5, 0.7]| 符号的间隔范围[0.5， 0.7]| 
|2  |A  |[0.5, 0.52]    |[0.5， 0.7]间隔的第一个1/10|
|3  |D| [0.514, 0.52]   |[0.5， 0.52]间隔的最后一个1/10|
|4| A   |[0.514, 0.5146]|   [0.514， 0.52]间隔的第一个1/10|
|5| C|  [0.5143, 0.51442]   |[0.514， 0.5146]间隔的第五个1/10开始，二个1/10|
|6| D|  [0.514384, 0.51442]|    [0.5143， 0.51442]间隔的最后3个1/10|
|7| B|  [0.5143836,0.514402]|   [0.514384，0.51442]间隔的4个1/10，从第1个1/10开始|
|8| ||从[0.5143876， 0.514402]中选择一个数作为输出：0.5143876|

译码过程如下表
表3.3 算数编码的译码过程
|步骤|    间隔| 译码符号|   译码判决|
 |:-:|:-:|:-:|:-:|
|1| [0.5, 0.7]| C|  0.51439在间隔 [0.5, 0.7)|
|2| [0.5, 0.52] |A| 0.51439在间隔 [0.5, 0.7)的第1个1/10|
|3| [0.514, 0.52]|  D   |0.51439在间隔[0.5, 0.52)的第7个1/10|
|4| [0.514, 0.5146] |A| 0.51439在间隔[0.514, 0.52]的第1个1/10|
|5| [0.5143, 0.51442]|  C|  0.51439在间隔[0.514, 0.5146]的第5个1/10|
|6| [0.514384, 0.51442]|    D|  0.51439在间隔[0.5143, 0.51442]的第7个1/10|
|7| [0.51439,0.5143948] |B| 0.51439在间隔[0.51439, 0.5143948]的第1个1/10|
|8| ||译码的消息：C A D A C D B|

## 三. 算数编码的实现
这几种算法唯独算数编码我没有用C++实现，当时记得为了应付课堂作业，借用了网上一位博友的代码，大家如果想借鉴这个算法实现的代码，我这里可以给出我更改后的版本，但并不是我原创的，但是很抱歉具体是在哪里借鉴的我忘记了，当时比较草率，没有想太多，这里我还是把过程及测试结果给大家介绍清楚，代码的话如果能找到原主人最好不过了，这里我给出我改过的版本。
算数编码我只是使用字符串进行了测试，不能做到像Huffman编码一样对任何类型的文件都进行编码和译码。
主函数
```
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
```
特殊结构和功能函数的定义
```
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

void disp()

{
    
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
```

# 第四章：LZ编码的实现之LZ-78编码
## 一. LZ-78编码原理
经过查找资料，LZ编码并不是一种编码，而是一组编码，由LZ-77和LZ-78演变而来有很多种变形。所以这里我只选取了一种比较简单的算法LZ-78算法。
LZ-78编码算法是一种分段编码算法。算法的压缩过程非常简单。在压缩时维护一个动态词典Dictionary，其包括了历史字符串的index与内容。设信源符号集 $A={a_0,a_1,a_2...a_{q-1}}$共q个字符，设输入信源序列为 $$S_1S_2...S_n$$
编码就是将此序列分成不同的X段。分段的原则是：
1. 先取第一个符号作为第一段，然后再继续分段
2. 若出现有与前面相同符号时，就在添加进跟随后面的一个符号一起组成一个段，以使与前面的段不同。
3. 尽可能取最少个连通着的信源符号，并保证隔断都不相同。直至信源符号徐立结束。
这样，不同的段内的信源符号可看成一短语，可得不同段所对应的短语字典表。若编成二元码，段号用二进制表示，段号所需码长为$l=\lceil logX(n) \rceil$ ，X(n)是段号数。
## 二. LZ-78编码过程

LZ-78编码在压缩时维护一个动态词典Dictionary，其包括了历史字符串的index与内容；压缩情况分为三种：
1. 若当前字符c未出现在词典中，则编码为(0, c)；
2. 若当前字符c出现在词典中，则与词典做最长匹配，然后编码为(prefixIndex,lastChar)，其中，prefixIndex为最长匹配的前缀字符串，lastChar为最长匹配后的第一个字符；
3. 为对最后一个字符的特殊处理，编码为(prefixIndex,)
举例
以字符串“ABBCBCABABCAABCAAB”压缩编码构造字典的过程如下
![LZ-78编码](http://or6i73qhr.bkt.clouddn.com/InfomationTheory/second/LZ-78.png "LZ-78编码")
1. A 不在字典中; 插入A
2. B不在字典中; 插入B
3. B 在字典中.
    BC 不在字典中; 插入BC  
4. B在字典中.
    BC在字典中.
    BCA不在字典中.;插入BCA
5. B在字典中.
    BA不在字典中; 插入BA.
6. B在字典中.
    BC在字典中.
    BCA在字典中.
    BCAA不在字典中;插入BCAA
7. B在字典中.
    BC在字典中.
    BCA在字典中.
    BCAA在字典中.
    BCAAB 不在字典中; 插入BCAAB.
## 三. LZ-78编码编程实现和性能分析
LZ-78算法我构建了一个LZ78类。类的定义如下
```
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
```
核心属性
1. struct Dictionary: 存储字典信息
2. struct OutData: 存储输出信息即上面所提到的二元组(preIndex, lastChar);
3. string fileAddress: 需要压缩的文件的路径名称
核心函数：
1. void open(string address) : 打开待压缩文件
2. void Press():  压缩文件操作
3. void Decode(string sourcefile, string dstfile): 解码操作
正式操作见下面主函数：
主函数：
```
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
```
文件压缩步骤
第一步：建立haha对象为LZ78类型
第二步：打开待压缩的文件
第三步：压缩文件
第四部：压缩结束
文件解压步骤
第一步：建立nothaha对象为LZ78类型
第二步：解压文件
第三步：解压结束
LZ-78编码的性能测试见下表
表4.1 LZ-78编码性能测试
|原始文件|  890Bytes(文本)    |46.7kb(图像)|    7.82M(视频)|
|-|-|-|-|
|压缩文件   |3133Bytes  |47.9kb |7.81M|
|压缩率    |1.29   |1.025| 99.8%|
|压缩用时|  6/s|    20/s    |/|
|解码用时   |0/s|   0/s|    /|

看到自己程序跑的结果自己都想笑，这也太慢了点了，有时候想安慰一下自己，可能是C++读取文件的API很耗时，要想速度快，有朝一日自己写读取文件的API，励志。
最后附上LZ-78程序的C++源代码

# 总结
经过此次试验真的是收获了很多，首先是对各种编码的熟悉，从Huffman编码、Shannon Fano码、算数编码到LZ编码家族，了解了Zip，winrar等压缩软件的压缩原理，以及压缩算法从兴起到应用，里面还有一些版权纠纷的故事(LZW版权问题)。到现在的操作系统使用的压缩软件的情况，Windows主要的软件是winrar和zip， linux系统下主要是.gz和gzip软件。其原理都是基于DELLATE算法的。DELLATE算法是以Huffman编码和LZW编码为核心的。另一方面，从信息论角度细致的学习了各种编码的编码原理和译码方式，这些收获相信在今后的学习中都会有用的。这次实验不仅锻炼了编程能力，而且学习到很多编码领域的新知识。


**参考文献**：
《信息论与编码》傅祖芸、赵建中 电子工业出版社
C++实现Huffman文件编码和解码：http://www.cnblogs.com/matrix-r/p/3354887.html 
zip压缩原理 http://blog.jobbole.com/76676/
Huffman对文件编码和解码http://blog.csdn.net/computer_liuyun/article/details/41446773 
huffman编码原理与实现：http://blog.csdn.net/abcjennifer/article/details/8020695 
二进制和ASCII编码： http://blog.csdn.net/sxhelijian/article/details/29594687  
压缩编码的历史：http://blog.csdn.net/kimylrong/article/details/39405981
Lz78算法：http://www.cnblogs.com/aeexiaoqiang/p/6529375.html
算数编码： http://blog.csdn.net/adam_tu/article/details/7696455


