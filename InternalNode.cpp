#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [internalSize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()

void InternalNode::moveLeft( ) {

    for ( int i=0; i<count; i++ ) {
            keys[i] = keys[i+1];
            children[i] = children[i+1];
    }
}

InternalNode* InternalNode::splitNode(BTreeNode *BTreeNodePtr) {
    InternalNode* newNode;
    BTreeNode* maxptr;
    int value;
    int i;

    newNode = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
    if (rightSibling)
        rightSibling->setLeftSibling(newNode);
    rightSibling = newNode;

    value = BTreeNodePtr->getMinimum();
    if (value<keys[internalSize-1]) {
        maxptr = children[internalSize-1];
        count --;
        insert(BTreeNodePtr);
//        pushin(0,internalSize-2,BTreeNodePtr);
    } else {
        maxptr = BTreeNodePtr;
    }
    count = (internalSize+1)/2;
    for (i=count;i<internalSize;i++) {
        newNode->insert(children[i]);
    }
    newNode->insert(maxptr);
    return newNode;
}

InternalNode* InternalNode::SiblingOrSplit(BTreeNode *BTreeNodePtr) { // insert into a sibling or split
    int value;
    InternalNode* sibling;
    InternalNode* newNode=NULL;

    value = BTreeNodePtr->getMinimum();
    sibling = (InternalNode*)leftSibling;
    if (sibling&&(sibling->parent==parent)&&( sibling->getCount()<internalSize)) {
        if (value>keys[0]) {
            sibling->insert(children[0]);
            count --;
            moveLeft();
            insert(BTreeNodePtr); //            pushin(1,count-1,BTreeNodePtr);
        } else
            sibling->insert(BTreeNodePtr);
    } else {
        sibling = (InternalNode*)rightSibling;
        if (sibling&&(sibling->parent==parent)&&( sibling->getCount()<internalSize)) {
            if (value < keys[count-1]) {
                sibling->insert(children[count-1]);
                count --;
                insert(BTreeNodePtr); //                pushin(0,count-2,BTreeNodePtr);
            } else
                sibling->insert(BTreeNodePtr);
        } else
            newNode = splitNode(BTreeNodePtr);
    }
    return newNode;
}

InternalNode* InternalNode::insert(int value) {
    int pos;
    BTreeNode *BTreeNodePtr;
    InternalNode* newNode=NULL;

    for ( pos=1; pos<count; pos++ ) {
        if ( value<keys[pos] )
            break;
    }
    pos--;
    BTreeNodePtr = children[pos]->insert(value);
    updateMinimums();
    if (BTreeNodePtr) {
        if ( count<internalSize ) {
            insert(BTreeNodePtr);
        } else {
            newNode = SiblingOrSplit(BTreeNodePtr);
        }
    }
    return newNode; // to avoid warnings for now.
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode) { // from a sibling or BTree::insert
    int pos=0;
    int i;
    int value;

    if (count<internalSize) {
        value = newNode->getMinimum();
        if (count>0) {
            for ( pos=0; pos<count; pos++ ) {
                if ( value<keys[pos] )
                    break;
            }
            for ( i=count; i>pos; i-- ) {
                keys[i] = keys[i-1];
                children[i] = children[i-1];
            }
        }
        keys[pos] = value;
        children[pos] = newNode;
        newNode->setParent(this);
        count ++;
    }
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


void InternalNode::updateMinimums() { // called by child with a new minimum
    for ( int i=0;i<count;i++ ) {
        keys[i] = children[i]->getMinimum();
    }
}   // InternalNode::updateMinimums

