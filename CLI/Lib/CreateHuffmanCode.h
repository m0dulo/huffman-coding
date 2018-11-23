#include "HTNode.h"
#include <cstring>
#include "CreatHuffmanTree.h"

void CreateHuffmanCode(char *Code[], HuffmanTree HT, char *LastCode)
{
    if (HT -> lchild == NULL && HT -> rchild == NULL)
    {
        Code[(int)((unsigned char)(HT -> code))] = new char[strlen(LastCode) + 1];
        strcpy(Code[(int)((unsigned char)(HT -> code))],LastCode);
    }

    else
    {
        //cout << "Left" << endl;
        char *TempCode = new char[strlen(LastCode) + 2];
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT -> lchild,strcat(TempCode,"0"));
        //cout << "right" << endl;
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT -> rchild,strcat(TempCode,"1"));    
    }
    
}
