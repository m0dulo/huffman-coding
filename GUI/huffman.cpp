#include "Huffman.h"
#include <QDebug>
#include "string.h"

#define FILE_SIGN "LEAGEL"
const int FILE_SIGN_LEN = 6;


Huffman::Huffman(QObject *parent)
    : QObject(parent)
{

}

Huffman::~Huffman()
{

}

void Huffman::setOutputFile(const QString& file)
{
    strncpy(m_OutputFile,file.toStdString().c_str(),sizeof(m_OutputFile));
}

void Huffman::HuffmanCode(const char *FileName)
{
    ifstream FileIn(FileName, ios::binary);
    /*
    char *TempFileName = new char[strlen(FileName) + 1];
    strcpy(TempFileName, FileName);
    strcpy(TempFileName + strlen(TempFileName) - 4, ".cod");
    //cout << TempFileName << endl;
    ofstream FileOut(TempFileName, ios::binary | ios::trunc);
    */
    ofstream FileOut(m_OutputFile, ios::binary | ios::trunc);
    if (FileIn.bad() || FileOut.bad()) {
        cout << "open file err" << endl;
        log(QString("Open file[%1] or file[%2]error")
            .arg(FileName).arg(m_OutputFile));
        return;
    }
    //文件打开测试
    /*
    char ch;
    while ((ch = FileIn.get()) != EOF){
    cout << ch;
    }
    */

    //统计字符出现概率
    //初始化
    StatisticsNode TimeNode[256];
    for (int i = 0; i < 256; i++) {
        TimeNode[i].code = i;
        TimeNode[i].times = 0;
    }
    //统计次数
    char ch;

    while (!FileIn.eof()) {
        ch = FileIn.get();
        TimeNode[(int)((unsigned char)ch)].times++;
    }
    /*
    for (int i = 0;i < 256;i ++){
    cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
    */
    //排序
    sort(TimeNode, TimeNode + 256, CompareTimeNode);
    //测试排序结果
    /*
    for (int i = 0;i < 256;i ++){
    cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
    */

    //把一次都没有用到的删去
    int n;
    n = 256;
    for (int i = 0; i < 256; i++) {
        if (TimeNode[i].times == 0) {
            n = i;//记录真正的数组长度
            break;
        }
    }
    /*
    for (int i = 0;i < n;i ++){
    cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
    */

    //根据概率生成Huffman树
    HuffmanTree NewHT = CreateHuffmanTree(TimeNode, n);

    //ShowHuffmanTree(NewHT);
    //system("pause");

    //根据Huffman树生成编码数组
    char *CodeN[256];
    for (int i = 0; i < 256; i++) {
        CodeN[i] = NULL;
    }

    CreateHuffmanCode(CodeN, NewHT, (char*)"");
    //测试CODE是否正确

    /*
    for (int i = 0;i < 256;i ++){
    cout << i << ":" << (CodeN[i] == NULL ? "NULL" : CodeN[i]) << endl;
    }
    */

    //根据编码数组进行编码并且写入文件
    //标志符HC
    FileOut.write(FILE_SIGN,FILE_SIGN_LEN);
    //文件名
    //cout << FileName << endl;
    //FileOut.write(FileName, strlen(FileName) + 1);
    //TimeNode 长度
    FileOut.write((char *)&n, sizeof(n));
    //TimeNode
    FileOut.write((char *)TimeNode, sizeof(StatisticsNode) * n);
    //Huffman编码
    FILE *fp=fopen(FileName,"r");
    fseek(fp,0L,SEEK_END);
    long FileLength=ftell(fp);
    fclose(fp);
    //cout << FileLength << endl;

    FileOut.write((char *)&FileLength, sizeof(FileLength));
    FileIn.clear();
    FileIn.seekg(0);
    queue<char> MyQueue;
    char TempStr[300];
    char TempChar;
    int charCnt = 0;
    while (!FileIn.eof()) {
        ch = FileIn.get();
        charCnt++;
        //二进制编码入队列
        strcpy(TempStr, CodeN[int((unsigned char)(ch))]);
        //cout << "bin: " << TempStr << endl;
        for (int i = 0; TempStr[i]; i++) {
            MyQueue.push(TempStr[i]);
        }
        while (MyQueue.size() >= 8) {
            int kk;
            kk = 0;
            for (int i = 7; i >= 0; i--) {
                kk += pow(2, i) * (MyQueue.front() - '0');
                MyQueue.pop();
            }
            //cout << "kk:" << kk << endl;
            TempChar = (char)kk;
            FileOut.write(&TempChar, 1);
        }
        //CodeN[int(ch)]

        emit progressSignal((charCnt * 100) / FileLength);
    }
    //处理最后不足8位的
    int kk = 0, ii = 7;
    while (MyQueue.size() != 0) {
        kk += pow(2, ii) * (MyQueue.front() - '0');
        MyQueue.pop();
        ii--;
    }
    TempChar = (char)kk;

    //cout << "kk:" << kk << endl;
    FileOut.write(&TempChar, 1);


    FileIn.close();
    FileOut.close();

    emit progressSignal(100);

    FILE *fp2=fopen(m_OutputFile,"r");
    fseek(fp2,0L,SEEK_END);
    long FileLength2=ftell(fp2);
    fclose(fp2);

    //delete TempFileName;
    cout << "compress finish,rate is:" << FileLength2 * 100 / FileLength << "%" << endl;
    log(QString("compress finish,rate is:%1%").arg(((double)FileLength2 * 100 / FileLength)));
}
void Huffman::HuffmanDecode(const char *FileName)
{
    ifstream FileIn(FileName, ios::binary);
   
    char Sign[FILE_SIGN_LEN + 1];
    //读入标志符
    FileIn.read(Sign,FILE_SIGN_LEN );
    Sign[FILE_SIGN_LEN] = '\0';
    //cout << Sign << endl;
    if (strcmp(Sign, FILE_SIGN) != 0) {
        cout << "Not valid cod file" << endl;
        log(QString("Not valid cod file[%1]").arg(FileName));
        return;
    }
    //读入原文件名
    //char *TempFileName = new char[strlen(FileName) + 1];
    //FileIn.read(TempFileName, strlen(FileName) + 1);

    ofstream FileOut(m_OutputFile, ios::binary);

    int n;
    FileIn.read((char *)&n, sizeof(n));
    //cout << "n:" << n << endl;
    //system("pause");
    StatisticsNode TimeNode[256];

    FileIn.read((char *)&TimeNode, sizeof(StatisticsNode) * n);
    //读取文件长度
    long FileLength;
    FileIn.read((char *)&FileLength, sizeof(FileLength));
    //cout << "FileLength:" << FileLength << endl;

    HuffmanTree NewHT = CreateHuffmanTree(TimeNode, n);

    //ShowHuffmanTree(NewHT);
    //system("pause");
    queue<char> MyQueue;
    stack<char> TempStack;

    //根据Huffman树找原码,并且保存
    char ch;
    int charCnt = 0;
    //cout << FileIn.get() << endl;
    while (!FileIn.eof()) {
        ch = FileIn.get();
        charCnt++;
        //16进制转化成2进制
        int k;
        //cout << ch << endl;
        k = (unsigned char)ch;
        //cout << k << endl;

        int xi;
        while (k != 0) {
            char tempc;
            tempc = k % 2 + '0';
            TempStack.push(tempc);
            k = k / 2;
        }

        //不足8位用0填
        while (TempStack.size() < 8) {
            TempStack.push('0');
        }

        //使用栈逆置
        while (!TempStack.empty()) {
            //cout << TempStack.top() << " ";
            MyQueue.push(TempStack.top());
            TempStack.pop();
        }
        //cout << endl;

        while (MyQueue.size() >= 32) {
            int tempch;
            tempch = ChangeCodeToChar(NewHT, MyQueue);
            //cout << "size:" << MyQueue.size() << endl;
            //cout << "tempch:" << (int)(unsigned char)tempch << endl;
            FileOut << (char)tempch;
        }

        emit progressSignal((charCnt * 100) / FileLength);

    }
    //处理文件尾
    //cout << "tail:" << endl;
    while (MyQueue.size() >= 8) {
        int tempch;
        tempch = ChangeCodeToChar(NewHT, MyQueue);
        //cout << "tempch:" << (int)(unsigned char)tempch << endl;
        if (tempch == 256) {
            break;
        }
        FileOut << (char)tempch;
        if ((long)FileOut.tellp() == FileLength) {
            break;
        }
    }

    //delete TempFileName;
    FileIn.close();
    FileOut.close();

    emit progressSignal(100);

    cout << "decode finish" << endl;
    log("decode finish");
    //cout << TempFileName << endl;
    //ofstream FileOut(TempFileName,ios::binary | ios::trunc);
}

int Huffman::ChangeCodeToChar(HuffmanTree HT,queue<char> &MyQueue){
    if (HT->lchild == NULL && HT->rchild == NULL){
        return HT->code;
    }
    else{
        if (!MyQueue.empty()){
            int n;
            n = MyQueue.front();
            MyQueue.pop();
            if (n == '0'){
                return ChangeCodeToChar(HT->lchild,MyQueue);
            }
            else{
                return ChangeCodeToChar(HT->rchild,MyQueue);
            }
        }
        else{
            return 256;
        }
    }
}

void Huffman::log(const QString& log)
{
    qDebug()<<QString("In Huffman::log");
    emit logSignal(log);
}


void Huffman::CreateHuffmanCode(char *Code[],HuffmanTree HT,char *LastCode){
    //cout << "code:" << LastCode << endl;
    if (HT->lchild == NULL && HT->rchild == NULL){
        Code[(int)((unsigned char)(HT->code))] = new char[strlen(LastCode) + 1];
        strcpy(Code[(int)((unsigned char)(HT->code))],LastCode);
    }
    else{
        //cout << "left" << endl;
        char *TempCode = new char[strlen(LastCode) + 2];
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT->lchild,strcat(TempCode,"0"));
        //cout << "right" << endl;
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT->rchild,strcat(TempCode,"1"));
    }
}

void Huffman::ShowHuffmanTree(HuffmanTree HT){
    if (HT->lchild == NULL && HT->rchild == NULL){
        //cout << (int)(unsigned char)(HT->code) << ":" << HT->weight << endl;
        log(QString("%1:%2").arg((int)(unsigned char)(HT->code)).arg(HT->weight));
    }
    else{
        //cout << "left" << endl;
        log("left");
        ShowHuffmanTree(HT->lchild);
        //cout << "right" << endl;
        log("right");
        ShowHuffmanTree(HT->rchild);
    }
}

bool CompareTimeNode(StatisticsNode a,StatisticsNode b){
    return a.times > b.times;
}

bool CompareHTNode(HuffmanTree a,HuffmanTree b){
    return a->weight > b->weight;
}

HuffmanTree Huffman::CreateHuffmanTree(StatisticsNode *TN,int n){
    if (n <= 1)
        return NULL;

    //建立树林，保存在索引中
    HuffmanTree TreeId[256];

    for(int i = 0;i < n;i ++){
        TreeId[i] = new HTNode;
        TreeId[i]->weight = TN[i].times;
        TreeId[i]->code = TN[i].code;
        TreeId[i]->lchild = NULL;
        TreeId[i]->rchild = NULL;
        TreeId[i]->parent = NULL;
    }

    while (n != 1){
        std::sort(TreeId,TreeId + n,CompareHTNode);
        HuffmanTree TempHT = new HTNode;
        TempHT->lchild = TreeId[n - 1];
        TempHT->rchild = TreeId[n - 2];
        TempHT->weight = TreeId[n - 1]->weight + TreeId[n - 2]->weight;
        TempHT->parent = NULL;
        TreeId[n - 1]->parent = TempHT;
        TreeId[n - 2]->parent = TempHT;
        TreeId[n - 2] = TempHT;
        n --;
    }
    return TreeId[0];
}

