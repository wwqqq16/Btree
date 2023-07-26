#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

void LeafNode::pushin(int start, int end, int value) {
    int pos;
    int i;

    for ( pos=start; pos<=end; pos++ ) {
        if ( value<values[pos] )
            break;
    }
    if ( start>0 ) {
        pos--;
        for ( i=0; i<pos; i++ ) {
            values[i] = values[i+1];
        }
    } else {
        for ( i=end+1; i>pos; i-- ) {
            values[i]=values[i-1];
        }
    }
    values[pos] = value;
}

LeafNode* LeafNode::splitNode(int value) {
    LeafNode* newNode;
    int max;
    int i;

    newNode = new LeafNode(leafSize, parent, this, rightSibling);
    if (rightSibling)
        rightSibling->setLeftSibling(newNode);
    rightSibling = newNode;

    if (value<values[leafSize-1]) {
        max = values[leafSize-1];
        pushin(0,leafSize-2,value);
    } else {
        max = value;
    }
    count = (leafSize+1)/2;
    for (i=count;i<leafSize;i++) {
        newNode->insert(values[i]);
    }
    newNode->insert(max);
    return newNode;
}

LeafNode* LeafNode::insert(int value) {
    LeafNode* newNode=NULL;
    if ( count<leafSize ) {
        if (count > 0)
            pushin(0,count-1,value);
        else
            values[0] = value;
        count++;
    } else {
        if (leftSibling&&(leftSibling->getCount()<leafSize)) {
            if (value>values[0]) {
                leftSibling->insert(values[0]);
                pushin(1,count-1,value);
            } else
                leftSibling->insert(value);
        } else {
            if (rightSibling&&(rightSibling->getCount()<leafSize)) {
                if (value<values[count-1]) {
                    rightSibling->insert(values[count-1]);
                    pushin(0,count-2,value);
                } else
                    rightSibling->insert(value);
            } else
                newNode = splitNode(value);
        }
    }
    return newNode;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

