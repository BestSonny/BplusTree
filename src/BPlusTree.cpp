#include "BPlusTree.hpp"

template <class Key, class Value>
int BPlusTree<Key, Value>::innerNode::getLower(Key k)
{
    if(this->keyNum == 0) return 0;
    int lo = 0, hi = this->keyNum;

    while(lo < hi) {
        int mid = (lo + hi) / 2;
        if(key[mid] > k) hi = mid;
        else lo = mid + 1;
    }

    return hi;
}

template <class Key, class Value>
void BPlusTree<Key, Value>::innerNode::insert(Key k, Node *p)
{
    int i = 0;
    for(i = this->keyNum-1; i >= 0 && key[i] > k; i--) {
        key[i+1] = key[i];
        child[i+2] = child[i+1];
    }
    key[i+1] = k;
    child[i+2] = p;
    this->keyNum++;
}

template <class Key, class Value>
Key BPlusTree<Key, Value>::innerNode::split(Node*  newNode, Key k)
{
    if(k > key[minKeyNum] && k < key[minKeyNum+1]) {
        for(int i = 0; i < minKeyNum; i++) {
            ((innerNode *)newNode)->key[i] = key[i + maxKeyNum - minKeyNum];
            ((innerNode *)newNode)->child[i+1] = child[i + maxKeyNum - minKeyNum + 1];
        }
        newNode->keyNum = minKeyNum;
        this->keyNum = maxKeyNum - minKeyNum;
        return k;
    }
    else {
        Key retkey = 0;
        if(k > key[minKeyNum + 1]) {
            retkey = key[minKeyNum + 1];
            for(int i = 0; i < minKeyNum - 1; i++) {
                ((innerNode *)newNode)->key[i] = key[i + maxKeyNum - minKeyNum + 1];
                ((innerNode *)newNode)->child[i+1] = child[i + maxKeyNum - minKeyNum + 2];
            }
            ((innerNode *)newNode)->child[0] = child[maxKeyNum - minKeyNum + 1];
            newNode->keyNum = minKeyNum - 1;
            this->keyNum = maxKeyNum - minKeyNum;
            return retkey;
        }
        else {
            retkey = key[minKeyNum];
            for(int i = 0; i < minKeyNum; i++) {
                ((innerNode *)newNode)->key[i] = key[i + maxKeyNum - minKeyNum];
                ((innerNode *)newNode)->child[i+1] = child[i + maxKeyNum - minKeyNum + 1];
            }
            ((innerNode *)newNode)->child[0] = child[maxKeyNum - minKeyNum];
            newNode->keyNum = minKeyNum;
            this->keyNum = maxKeyNum - minKeyNum - 1;
            return retkey;
        }
    }
}

//<P0 K0 P1 K1 ... Pn Kn Pn+1>
//for  Ki <= key < Ki+1 ,key store in suBPlusTree pointed by Pi+1
//find first Ki that is greater than key, return i;
template <class Key, class Value>
int BPlusTree<Key, Value>::leafNode::getLower(Key k)
{
    if(this->keyNum == 0) return 0;
    int lo = 0, hi = this->keyNum;

    while(lo < hi) {
        int mid = (lo + hi) / 2;
        if(key[mid] > k) hi = mid;
        else lo = mid + 1;
    }

    //auto low=std::lower_bound(key, key+hi-1, k);
    //return low-key;
    return hi;
}

template <class Key, class Value>
void BPlusTree<Key, Value>::leafNode::insert(Key k, Value v)
{
    int i = 0;
    for(i = this->keyNum-1; i >= 0 && key[i] > k; i--) {
        key[i+1] = key[i];
        value[i+1] = value[i];
    }
    key[i+1] = k;
    value[i+1] = v;
    this->keyNum++;
}

template <class Key, class Value>
Key BPlusTree<Key, Value>::leafNode::split(leafNode* newNode)
{
    for(int i = 0; i < minKeyNum; i++) {
        newNode->key[i] = key[i + maxKeyNum - minKeyNum];
        newNode->value[i] = value[i + maxKeyNum - minKeyNum];
    }
    this->keyNum = maxKeyNum - minKeyNum;
    newNode->keyNum = minKeyNum;

    newNode->right = right;
    newNode->left = this;
    if(right != NULL) {
        right->left = newNode;
    }
    right = newNode;

    return newNode->key[0];
}

template <class Key, class Value>
bool BPlusTree<Key, Value>::get(Node* node, Key key, Value & v)
{
    if(node == NULL) return false;

    if(node->isLeaf) {
        int slot = node->getLower(key) - 1;
        if(((leafNode*)node)->key[slot] == key) {
            v = ((leafNode*)node)->value[slot];
            return true;
        }
    }
    else {
        int slot = node->getLower(key);
        return get(((innerNode*)node)->child[slot], key, v);
    }
}

template <class Key, class Value>
bool BPlusTree<Key, Value>::get(Key key, Value& result)
{
    return get(root, key, result);
}

template <class Key, class Value>
void BPlusTree<Key, Value>::insertNode(Node* node, Key key, Value v, Node *p, stack<Node*>* parent)
{
    Node* newNode;
    Key newKey;
    if(node->keyNum < maxKeyNum) {
        if(node->isLeaf) ((leafNode*)node)->insert(key, v);
        else ((innerNode *)node)->insert(key, p);
    }
    else {
        if(node->isLeaf) {
            newNode = new leafNode(maxLeafSlot, minKeyNum, maxKeyNum);
            newKey = ((leafNode*)node)->split((leafNode *)newNode);

            if(key < newKey) {
                ((leafNode*)node)->insert(key, v);
            }
            else
                ((leafNode*)newNode)->insert(key, v);
        }
        else {
            newNode = new innerNode(maxChildNum, minKeyNum, maxKeyNum);
            newKey = ((innerNode*)node)->split(newNode, key);
            if(newKey == key) {
                ((innerNode *)newNode)->child[0] = p;
            }
            else if(newKey > key) {
                ((innerNode*)node)->insert(key, p);
            }
            else {
                ((innerNode*)newNode)->insert(key, p);
            }
        }

        if(node == root) {
            innerNode* newRoot = new innerNode(maxChildNum, minKeyNum, maxKeyNum);
            newRoot->keyNum = 1;
            newRoot->key[0] = newKey;
            newRoot->child[0] = node;
            newRoot->child[1] = newNode;
            root = newRoot;
        }
        else {
            Node* p = parent->top();
            parent->pop();
            insertNode(p, newKey, v, newNode, parent);
        }
    }
}

template <class Key, class Value>
void BPlusTree<Key, Value>::put(Key key, Value value)
{
    stack<Node *> parent;
    Node *n = root;
    if(root == NULL) {
        root = new leafNode(maxLeafSlot, minKeyNum, maxKeyNum);
        insertNode(root , key, value, NULL , &parent);
        return;
    }

    while(n->isLeaf == false) {
        innerNode *inner = (innerNode*) n;
        int slot = inner->getLower(key);
        parent.push(n);
        n = inner->child[slot];
    }
    leafNode *leaf = (leafNode *)n;
    int slot = leaf->getLower(key);
    if(slot > 0) slot -= 1;
    //if(leaf->key[slot] == key) leaf->value[slot] = value;
    insertNode(leaf , key, value, NULL , &parent);

}

template <class Key, class Value>
multimap<Key, Value> BPlusTree<Key, Value>::getrange(Key key1, Key key2)
{
    Node *n = root;
    int i = 0;
    multimap<Key, Value> res;
    if(root == NULL) return res;
    while(n->isLeaf == false) {
        innerNode *inner = (innerNode*) n;
        int slot = inner->getLower(key1);
        n = inner->child[slot];
    }

    leafNode *leaf = (leafNode *)n;

    int slot = leaf->getLower(key1)-1;

    if(leaf->key[slot] != key1) slot++;

    i = slot;

    while(true) {
        bool flag = false;
        cout << i << "   "<< leaf->key[i] << endl;
        for(; i >= 0 && leaf->key[i] >= key1 && leaf->key[i] <= key2; i--) {
          if(i == 0){
            flag = true;
            break;
          }
        }
        //all node are in the range
        if(flag) {
            if (leaf->left == NULL)
              break;
            leaf = leaf -> left;
            i = leaf->keyNum - 1;
        }else{
            if(i==leaf->keyNum-1){
              if (leaf->right == NULL)
                break;
              leaf = leaf -> right;
              i = 0;
            } else{
              i++;
            }
            break;
        }
    }
    while(true) {

        for(; i < leaf->keyNum && leaf->key[i] >= key1 && leaf->key[i] <= key2; i++) {
            res.insert(pair <Key, Value> (leaf->key[i], leaf->value[i]));
        }
        if(i >= leaf->keyNum) {
            i = 0;
            if (leaf->right == NULL)
              break;
            leaf = leaf -> right;
        }
        else break;
    }
    return res;
}

template class BPlusTree<float, string>;
