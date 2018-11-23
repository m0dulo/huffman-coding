#include "CreateHuffmanCode.h"
#include "CreatHuffmanTree.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>

using namespace std;

bool CompareTimeNode(StatisticsNode a, StatisticsNode b)
{
    return a.times > b.times;
}

void HuffmanCode( char *FileName)
{
    std::ifstream FileIn(FileName, ios::binary);
    char *TempFileName = new char[strlen(FileName) + 1];
    strcpy(TempFileName, FileName);
    strcpy(TempFileName + strlen(TempFileName) - 3, "cod");
    //cout << TempFileName << endl;
    std::ofstream FileOut(TempFileName,ios::binary | ios::trunc);
    
    if(FileIn.bad() || FileOut.bad())
    {
        cout << "文件打开失败！" << endl;
        return;
    }
    /*
    char ch;
    while((ch = FileIn.get()) != EOF)
    {
        cout << ch;
    }
    */
   StatisticsNode TimeNode[256];

   for(int i = 0; i < 256; i++)
   {
       TimeNode[i].code = i;
       TimeNode[i].times = 0;
   }

   char ch;

   while(!FileIn.eof())
   {
       ch = FileIn.get();
       TimeNode[(int)((unsigned char)ch)].times ++;
   }
   /*
    for (int i = 0;i < 256;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/

   sort(TimeNode, TimeNode + 256, CompareTimeNode);
   /*
    for (int i = 0;i < 256;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/

    int n = 256;
    for (int i = 0 ; i < 256 ; i++ )
    {
        if(TimeNode[i].times == 0)
        {
            n = i;
            break;
        }
    }
    /*
    for (int i = 0;i < n;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/
    HuffmanTree NewHT = CreatHuffmanTree(TimeNode,n);

    //ShowHuffmanTree(NewHT);

    char *CodeN[256];
    for (int i = 0; i < 256; i++)
    {
        CodeN[i] = NULL;
    }

    CreateHuffmanCode(CodeN,NewHT,"");

    /*
    for (int i = 0;i < 256;i ++){
        cout << i << ":" << (CodeN[i] == NULL ? "NULL" : CodeN[i]) << endl;
    }
    */

    FileOut.write("LEGAL",5);
    FileOut.write(FileName,strlen(FileName) + 1);
    FileOut.write((char *)&n,sizeof(n));
    FileOut.write((char *)TimeNode,sizeof(StatisticsNode) * n);

    HANDLE fhadle = CreateFile(FileName, 0, 0, 0, OPEN_EXISTING, 0, 0);
    long FileLength = GetFileSize(fhadle, 0);
    //cout << FileLength << endl;

    FileOut.write((char *)&FileLength,sizeof(FileLength));
    FileIn.clear();
    FileIn.seekg(0);

    queue<char> MyQueue;
    char TempStr[300];
    char TempChar;
    while(!FileIn.eof())
    {
        ch = FileIn.get();
        strcpy(TempStr,CodeN[int((unsigned char)(ch))]);
        //cout << TempStr << endl;
        for (int i = 0; TempStr[i]; i++)
        {
            MyQueue.push(TempStr[i]);
        }
        while(MyQueue.size() >= 8)
        {
            int k;
            k = 0;
            for (int i = 7; i >= 0; i--)
            {
                k += pow(2,i) * (MyQueue.front() - '0');
                MyQueue.pop();
            }
            //cout << k << endl;
            TempChar = (char)k;
            FileOut.write(&TempChar,1); 
        }
    }

    int kk = 0 ,ii = 7;
    while(MyQueue.size() != 0)
    {
        kk += pow(2,ii) * (MyQueue.front() - '0');
        MyQueue.pop();
        ii--;
    }

    TempChar = (char)kk;

    //cout << "kk:" << kk << endl;
    FileOut.write(&TempChar,1);

    FileIn.close();
    FileOut.close();

    HANDLE newfhadle = CreateFile(TempFileName, 0, 0, 0, OPEN_EXISTING, 0, 0);
    long newFileLength = GetFileSize(newfhadle,0);

    delete TempFileName;

    cout << "压缩成功！"<< endl << "压缩比为" << newFileLength * 100 /FileLength << "%" << endl;
}