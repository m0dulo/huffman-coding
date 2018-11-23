#pragma once

typedef struct HTNode
{
  long weight;
  char code;
  struct HTNode *parent;
  struct HTNode *lchild;
  struct HTNode *rchild;
}HTNode, *HuffmanTree;



