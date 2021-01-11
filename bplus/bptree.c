#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ORD 2 //order
struct Node {
    struct Node* C[2 * ORD + 1];
    struct Node* nextNode;
    int key[2 * ORD];
    int lenKey;
    bool isLeaf;
};
struct BTREE {
    struct Node* root;
};
void BtreeSearch(struct Node* node, int searchValue, int level);
void insertData(struct Node* node, int inputVal);
void insertToTree(struct BTREE* tree, int inputVal);
void split(struct Node* node, int idx);
struct BTREE* initTree();
struct Node* createNode();
int main() {
    struct BTREE*  BTree = initTree();
    struct Node* rootNode = createNode();
    BTree->root = rootNode;
    insertToTree(BTree, 1);
    insertToTree(BTree, 2);
    insertToTree(BTree, 4);
    insertToTree(BTree, 3);
    BtreeSearch(BTree->root, 4, 0);
    return 0;
}
void BtreeSearch(struct Node* node, int searchValue, int level) {
    int i = 1;
    while (i <= node->lenKey && searchValue > node->key[i]) {
        i += 1;
    }
    if (i <= node->lenKey && searchValue == node->key[i]) {
        printf("found %d in %d\n", node->key[i], level);
        if (node->isLeaf != true) {
            BtreeSearch(node->C[i+1], searchValue, level + 1);
        }
        return;
    }
    else if (node->isLeaf == true) {
        printf("NULL searchVal: %d\n", searchValue);
        return;
    }
    else {
        //printf("search child node\n");
        BtreeSearch(node->C[i], searchValue, level + 1);
    }
}
struct BTREE* initTree() {
    struct BTREE* tree = malloc(sizeof(struct BTREE));
    tree->root = NULL;
    return tree;
}
struct Node* createNode() {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode -> isLeaf = true;
    newNode->lenKey = 0;
    return newNode;
}
void split(struct Node* node, int idx) {
    struct Node* rightNode = createNode();
    struct Node* leftNode = node->C[idx];
    int T = ORD;
    if (leftNode->isLeaf == false){
        rightNode->isLeaf = leftNode->isLeaf;
        rightNode->lenKey = T - 1;
        for (int j = 1; j <= T - 1; j++) {
            rightNode->key[j] = leftNode->key[j + T];
        }
        for (int j = 1; j <= T; j++) {
            rightNode->C[j] = leftNode->C[j + T];
        }
        leftNode->lenKey = T - 1;
        for (int j = (node->lenKey) + 1; j >= idx + 1; j--) {
            node->C[j + 1] = node->C[j];
        }
        node->C[idx + 1] = rightNode;
        for (int j = node->lenKey; j >= idx; j--) {
            node->key[j + 1] = node->key[j];
        }
        node->key[idx] = leftNode->key[T];
        node->lenKey += 1;
    }
    else {
        rightNode->isLeaf = leftNode->isLeaf;
        rightNode->lenKey = T-1;
        for (int j = 1; j <= T-1; j++) {
            rightNode->key[j] = leftNode->key[j + T];
        }
        leftNode->lenKey = T;
        for (int j = (node->lenKey) + 1; j >= idx + 1; j--) {
            node->C[j + 1] = node->C[j];
        }
        node->C[idx + 1] = rightNode;
        for (int j = node->lenKey; j >= idx; j--) {
            node->key[j + 1] = node->key[j];
        }
        leftNode->nextNode = rightNode;
        if (rightNode->key[1] < node->key[idx]) {
            node->key[idx] = rightNode->key[1];
        }
        node->lenKey += 1;
    }
    // 리프인 경우
    // 아닌 경우
    return;
}
void insertData(struct Node* node, int inputVal) {
    // 인덱스 찾기
    int i;
    for (i = node->lenKey; i >= 1; i--) {
        if (node->key[i] < inputVal) {
            break;
        }
    }
    i++;
    //인덱스에 삽입
    if (node->isLeaf == true) {
        for (int j = node->lenKey; j >= i; j--) {
            node->key[j + 1] = node->key[j];
        }
        node->key[i] = inputVal;
        node->lenKey++;
    }
    else {
        if (node->C[i]->lenKey == 2 * ORD-1) {
            split(node, i);
            if (inputVal > node->key[i]) {
                i++;
            }
        }
        insertData(node->C[i], inputVal);
    }
}
void insertToTree(struct BTREE* tree, int inputVal) {
    struct Node* r = createNode();
    r = tree->root;
    if (r->lenKey == 2 * ORD - 1) {
        struct Node* s = createNode();
        tree->root = s;
        s->key[1] = 999999999;
        s->C[1] = r;
        s->isLeaf = false;
        s->lenKey = 0;
        split(s, 1);
        insertData(s, inputVal);
    }
    else {
        insertData(r, inputVal);
    }
}