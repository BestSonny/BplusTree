#ifndef _BPLUSTREE_HPP
#define _BPLUSTREE_HPP

#include <iostream>
#include <stack>
#include <map>
#include <string>
using namespace std;
template <typename Key, typename Value>


#define MAX_CHILD_NUM  50
#define MAX_KEY_NUM MAX_CHILD_NUM-1
#define MAX_LEAF_SLOT MAX_CHILD_NUM-1
class BPlusTree
{
private:
    int minimumDegree;
    int minKeyNum;
    int maxKeyNum;
    int minChildNum;
    int maxChildNum;
    int minLeafSlot;
    int maxLeafSlot;
    class Node
    {
    public:
        bool isLeaf;
        Key keyNum;

        Node() { keyNum = 0;}

        virtual int getLower(Key k) = 0;
    };

    class innerNode : public Node
    {
        friend class BPlusTree;
    private:
        Node* child[MAX_CHILD_NUM];
        Key key[MAX_KEY_NUM];
        int minKeyNum;
        int maxChildNum;
    public:
    		innerNode(int maxChildNum, int minKeyNum) {this->maxChildNum = maxChildNum; this->minKeyNum = minKeyNum;
                                                   this->isLeaf = false;}
        virtual int getLower(Key k);
        void insert(Key key, Node *p);
        int split(Node* newNode, Key k);
    };

    class leafNode : public Node
    {
        friend class BPlusTree;
    private:
        Value value[MAX_LEAF_SLOT];
        Key key[MAX_LEAF_SLOT];
        leafNode *left;
        leafNode *right;
        int minKeyNum;
        int maxLeafSlot;
    public:
        leafNode(int maxLeafSlot, int minKeyNum) {this->minKeyNum = minKeyNum; this->maxLeafSlot = maxLeafSlot;
                                                  this->isLeaf = true; this->left = NULL; this->right = NULL;}
        virtual int getLower(Key k);
        void insert(Key key, Value value);
        int split(leafNode* newNode);
    };

private:
    Node *root;
    leafNode* leftHead;

private:
    bool get(Node* node, Key key, Value& value);
    void insertNode(Node* node, Key key, Value value, Node* p, stack<Node *>* parent);

public:
    BPlusTree(int N) {
        root = NULL;
        leftHead = NULL;
        minimumDegree = 2;
        minKeyNum = minimumDegree - 1;
        maxKeyNum = N - 1;
        minChildNum = (N+1)/2;
        maxChildNum = N;
        minLeafSlot = minKeyNum;
        maxLeafSlot = maxKeyNum;
    }

    bool get(Key key, Value& value);
    void put(Key key, Value value);
    multimap<Key, Value> getrange(Key key1, Key key2);
};

#endif
