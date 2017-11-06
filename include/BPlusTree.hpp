#ifndef _BPLUSTREE_HPP
#define _BPLUSTREE_HPP

#include <iostream>
#include <stack>
#include <map>
#include <string>
using namespace std;
template <typename Key, typename Value>
class BPlusTree
{
private:
    static const int minimumDegree = 2;
    static const int minKeyNum = minimumDegree - 1;
    static const int maxKeyNum = 2*minimumDegree - 1;
    static const int minChildNum = minimumDegree;
    static const int maxChildNum = 2*minimumDegree;
    static const int minLeafSlot = minKeyNum;
    static const int maxLeafSlot = maxKeyNum;
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
        Node* child[maxChildNum];
        Key key[maxKeyNum];
    public:
    		innerNode() {this->isLeaf = false;}
        virtual int getLower(Key k);
        void insert(Key key, Node *p);
        int split(Node* newNode, Key k);
    };

    class leafNode : public Node
    {
        friend class BPlusTree;
    private:
        Value value[maxLeafSlot];
        Key key[maxLeafSlot];
        leafNode *left;
        leafNode *right;
    public:
        leafNode() {this->isLeaf = true; this->left = NULL; this->right = NULL;}
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
    BPlusTree() {
        root = NULL;
        leftHead = NULL;
    }

    bool get(Key key, Value& value);
    void put(Key key, Value value);
    multimap<Key, Value> getrange(Key key1, Key key2);
};

#endif
