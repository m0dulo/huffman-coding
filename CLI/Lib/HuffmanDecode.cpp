#include "ChangeCodetoChar.h"
#include "CreatHuffmanTree.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void HuffmanDecode(char *FileName)
{
    ifstream FileIn(FileName, ios::binary);
    char *TempFileName = new char[strlen(FileName) + 1];
    char Sign[6];

    FileIn.read(Sign, 5);
    Sign[5] = '\0';
    //cout << Sign << endl;
    if (strcmp(Sign, "LEGAL") != 0)
    {
        cout << "此压缩文件不合法！" << endl;
        return;
    }

    FileIn.read(TempFileName, strlen(FileName) + 1);

    ofstream FileOut(TempFileName, ios::binary);

    int n;
    FileIn.read((char *)&n, sizeof(n));
    //cout << "n:" << n << endl;
    //system("pause");
    StatisticsNode TimeNode[256];

    FileIn.read((char *)&TimeNode, sizeof(StatisticsNode) * n);

    long FileLength;
    FileIn.read((char *)&FileLength, sizeof(FileLength));
    //cout << "FileLength:" << FileLength << endl;

    HuffmanTree NewHT = CreatHuffmanTree(TimeNode, n);

    //ShowHuffmanTree(NewHT);
    //system("pause");
    queue<char> MyQueue;
    stack<char> TempStack;

    char ch;
    //cout << FileIn.get() << endl;
    while (!FileIn.eof())
    {
        ch = FileIn.get();

        int k;
        //cout << ch << endl;
        k = (unsigned char)ch;
        //cout << k << endl;

        int xi;
        while (k != 0)
        {
            char tempc;
            tempc = k % 2 + '0';
            TempStack.push(tempc);
            k = k / 2;
        }

    
        while (TempStack.size() < 8)
        {
            TempStack.push('0');
        }

        while (!TempStack.empty())
        {
            //cout << TempStack.top() << " ";
            MyQueue.push(TempStack.top());
            TempStack.pop();
        }
        //cout << endl;

        while (MyQueue.size() >= 32)
        {
            int tempch;
            tempch = ChangeCodetoChar(NewHT, MyQueue);
            //cout << "size:" << MyQueue.size() << endl;
            //cout << "tempch:" << (int)(unsigned char)tempch << endl;
            FileOut << (char)tempch;
        }
    }

    //cout << "tail:" << endl;
    while (MyQueue.size() >= 8)
    {
        int tempch;
        tempch = ChangeCodetoChar(NewHT, MyQueue);
        //cout << "tempch:" << (int)(unsigned char)tempch << endl;
        if (tempch == 256)
        {
            break;
        }
        FileOut << (char)tempch;
        if (FileOut.tellp() == FileLength)
        {
            break;
        }
    }

    delete TempFileName;
    FileIn.close();
    FileOut.close();
    cout << "解压完成！" << endl;
    //cout << TempFileName << endl;
    //ofstream FileOut(TempFileName,ios::binary | ios::trunc);
}