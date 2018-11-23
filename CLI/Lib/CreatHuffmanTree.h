#pragma once
#include"HTNode.h"
#include <algorithm>
#include "StatisticsNode.h"

bool CompareHTNode(HuffmanTree a, HuffmanTree b)
{
    return a->weight > b->weight;
}

HuffmanTree CreatHuffmanTree(StatisticsNode *TN,int n)
{
    HuffmanTree TreeId[256];

    for (int i = 0; i < n; i++)
    {
        TreeId[i] = new HTNode;
        TreeId[i] -> weight = TN[i].times;
        TreeId[i] -> code = TN[i].code;
        TreeId[i] -> lchild = NULL;
        TreeId[i] -> rchild = NULL;
        TreeId[i] -> parent = NULL;
    }

    for (int j = n; j > 1; j--)
    {
        std::sort(TreeId, TreeId + n, CompareHTNode);
        HuffmanTree TempHT = new HTNode;
        TempHT -> lchild = TreeId[j - 1];
        TempHT -> rchild = TreeId[j - 2];
        TempHT -> weight = TreeId[j - 1] -> weight + TreeId[j - 2] -> weight;
        TempHT -> parent = NULL;
        TreeId[j - 1] -> parent = TempHT;
        TreeId[j - 2] -> parent = TempHT;
        TreeId[j - 2] = TempHT;
    }

    return TreeId[0];
}
