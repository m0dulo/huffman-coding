#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <windows.h>

#define ORIGINAL_FILE_SUFFIX ".txt"
#define COMPRESSED_FILE_SUFFIX ".cod"

const int MAX_FILE_NAME_LEN = 256;

using namespace std;

typedef struct HTNode{
    long weight;
    char code;
    struct HTNode *parent,*lchild,*rchild;
}HTNode,*HuffmanTree;

typedef struct{
    char code;
    long times;
}StatisticsNode;

bool CompareTimeNode(StatisticsNode a,StatisticsNode b);
bool CompareHTNode(HuffmanTree a,HuffmanTree b);

class Huffman : public QObject
{
    Q_OBJECT

public:
    Huffman(QObject *parent);
    ~Huffman();

    void setOutputFile(const QString& file);

    void HuffmanDecode(const char *FileName);
    void HuffmanCode(const char *FileName);
    HuffmanTree CreateHuffmanTree(StatisticsNode *TN,int n);
    void ShowHuffmanTree(HuffmanTree HT);
    void CreateHuffmanCode(char **Code,HuffmanTree HT,char *LastCode);
    int ChangeCodeToChar(HuffmanTree HT,queue<char> &MyQueue);



private:
    void log(const QString& log);
    

signals:
    void logSignal(const QString& log);
    void progressSignal(int);

private:
    char m_OutputFile[MAX_FILE_NAME_LEN];

};

#endif // HUFFMAN_H

