
#include <queue>
#include "HTNode.h"
#include <cstdlib>

using namespace std;

int ChangeCodetoChar(HuffmanTree HT, queue<char> &MyQueue )
{
    if(HT -> lchild == NULL && HT -> rchild == NULL)
    {
        return HT -> code;
    }
    
    else
    {
        if (!MyQueue.empty())
        {
            int n;
            n = MyQueue.front();
            MyQueue.pop();
            if(n == '0')
            {
                return ChangeCodetoChar(HT -> lchild,MyQueue);
            }
            else
            {
                return ChangeCodetoChar(HT -> rchild,MyQueue);
            }
        }
    }
    
}
