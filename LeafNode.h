#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;  // called by parent
  void  pushin(int start, int end, int value);
  LeafNode*  splitNode(int value);
  LeafNode* insert(int value); // called by parent, sibling, or BTree::insert()
    // Returns pointer to new Leaf if it splits else NULL
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
