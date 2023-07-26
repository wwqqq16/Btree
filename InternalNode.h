#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p, BTreeNode *left, 
    BTreeNode *right);
  int getMinimum()const;
  // void pushin(int start, int end, BTreeNode *BTreeNodePtr);
  void moveLeft();
  InternalNode* splitNode(BTreeNode *BTreeNodePtr);
  void updateMinimums(); // called by child with new minimum
  InternalNode* SiblingOrSplit(BTreeNode *BTreeNodePtr);
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *newNode); // from a sibling or BTree::insert()
  void print(Queue <BTreeNode*> &queue);
}; // InternalNode class

#endif
