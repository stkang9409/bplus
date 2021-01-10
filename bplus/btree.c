#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const int MinDeg= 3;
struct Node {
    struct Node *C[2*MinDeg];
    bool isLeaf;
    int key[2*MinDeg-1];
    int lenKey;
    int t; // 트리의 차수 정보
};
struct BTREE {
    int t;
    struct Node *root;
};
struct Node *BtreeNodeCreate() {
    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->isLeaf = true;
    newNode->lenKey = 0;
    return newNode;
}
void BtreeSearch(struct Node *node, int searchValue, int level) {
    int i = 0;
    while (i <= node->lenKey && searchValue > node->key[i]) {
        i += 1;
    }
    if (i <= node->lenKey && searchValue == node->key[i]) {
        printf("%d found in %d level\n", node->key[i],level);
        return;
    } else if (node->isLeaf == true && searchValue == node->key[i]) {
        printf("NULL in %d\n",level);
        return;
    } else {
        //printf("search child node\n");
        BtreeSearch(node->C[i], searchValue,level+1);
    }
}
void BtreeSplitChild(struct Node *ParentNode, int idx) {
    struct Node *rightNode = BtreeNodeCreate();
    struct Node *leftNode = ParentNode->C[idx];
    int T = MinDeg;
    rightNode->isLeaf = leftNode->isLeaf;
    rightNode->lenKey = T - 1;
    for (int j = 1; j <= T - 1; j++) {
        rightNode->key[j] = leftNode->key[j + T];
    }
    if (leftNode->isLeaf == false) {
        for (int j = 1; j <= T; j++) {
            rightNode->C[j] = leftNode->C[j + T];
        }
    }
    leftNode->lenKey = T - 1;
    for (int j = (ParentNode->lenKey) + 1; j >= idx + 1; j--) {
        ParentNode->C[j + 1] = ParentNode->C[j];
    }
    ParentNode->C[idx + 1] = rightNode;
    for (int j = ParentNode->lenKey; j >= idx; j--) {
        ParentNode->key[j + 1] = ParentNode->key[j];
    }
    ParentNode->key[idx] = leftNode->key[T];
    ParentNode->lenKey += 1;
}
void BtreeInsertNonFull(struct Node *node, int inputValue) {
    int i = node->lenKey;
    if (node->isLeaf == true ) {
        while (i >= 1 && inputValue < node->key[i]) {
            node->key[i + 1] = node->key[i];
            i -= 1;
        }
        node->key[i] = inputValue;
        node->lenKey += 1;
    } else {
        while (i >= 1 && inputValue < node->key[i]) {
            i -= 1;
        }
        i += 1;
        if (node->C[i]->lenKey == 2 * (MinDeg)-1) {
            BtreeSplitChild(node, i);
            if (inputValue > node->key[i]) {
                i += 1;
            }
        }
        BtreeInsertNonFull(node->C[i], inputValue);
    }
}
void BtreeInsertNode(struct BTREE *btree, int inputValue) {
    struct Node *r = BtreeNodeCreate();
    r = btree->root;
    if (r->lenKey == 2 * (MinDeg) - 1) {
        struct Node *s = BtreeNodeCreate();
        btree->root = s;
        s->isLeaf = false;
        s->lenKey = 0;
        s->C[1] = r;
        BtreeSplitChild(s, 1);
        BtreeInsertNonFull(s, inputValue);
    } else {
        BtreeInsertNonFull(r, inputValue);
    }
}
void BtreeCreate(struct BTREE *T){
    struct Node* x = BtreeNodeCreate();
    T->root = x;
}
int main() {
    struct BTREE *BTree = malloc(sizeof(struct BTREE));
    BtreeCreate(BTree);
    for(int i =1; i<12; i++)
    {
        BtreeInsertNode(BTree,i);
        for(int j =1 ;j<i; j++)
        {
            BtreeSearch(BTree->root,j,0);
        }
        printf("==================\n");
    }
}