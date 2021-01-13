#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MinDeg 3
struct Node {
    struct Node* C[2 * MinDeg + 1];
    bool isLeaf;
    int key[2 * MinDeg];
    int lenKey;
};
struct BTREE {
    struct Node* root;
};
//초기화
struct Node* BtreeNodeCreate();
void BtreeCreate(struct BTREE* T);
//탐색
void BtreeSearch(struct Node* node, int searchValue, int level);
void PrintByDFS(struct Node* node, int cnt);
//삽입
void BtreeInsertNode(struct BTREE* btree, int inputValue);
void BtreeInsertNonFull(struct Node* node, int inputValue);
void BtreeSplitChild(struct Node* ParentNode, int idx);
//삭제
void DeleteBTree(struct BTREE* tree, struct Node* curNode, int dVal);
void DeleteInternalNode(struct BTREE* tree, struct Node* curNode, int dVal, int targetIdx);
int DeleteSucc(struct BTREE* tree, struct Node* node);
int DeletePred(struct BTREE* tree, struct Node* node);
void DeleteMerge(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteSibling(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx);
int main() {
    struct BTREE* BTree = malloc(sizeof(struct BTREE));
    BtreeCreate(BTree);
    for (int i = 1; i < 101; i++) {
        BtreeInsertNode(BTree, i);
    }
    PrintByDFS(BTree->root,0);
}
//초기화
struct Node* BtreeNodeCreate() {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->isLeaf = true;
    newNode->lenKey = 0;
    return newNode;
}
void BtreeCreate(struct BTREE* T) {
    struct Node* x = BtreeNodeCreate();
    T->root = x;
}
//탐색
void PrintByDFS(struct Node* node, int cnt) {
    if (node->isLeaf) {
        for (int i = 1; i <= cnt; i++) {
            printf("-------|");
        }
        for (int i = 1; i <= node->lenKey; i++) {
            printf("%4d", node->key[i]);
        }
        printf("\n");
    }
    else {
        for (int i = 1; i <= node->lenKey; i++) {
            PrintByDFS(node->C[i], cnt + 1);
            for (int i = 1; i <= cnt; i++) {
                printf("-------|");
            }
            printf("%d            \n", node->key[i]);
        }
        PrintByDFS(node->C[node->lenKey + 1], cnt + 1);
    }
}
void BtreeSearch(struct Node* node, int searchValue, int level) {
    int i = 1;
    while (i <= node->lenKey && searchValue > node->key[i]) {
        i += 1;
    }
    if (i <= node->lenKey && searchValue == node->key[i]) {
        printf("found %d in %d\n", node->key[i], level);
        return;
    }
    else if (node->isLeaf == true) {
        printf("%d NULL\n", searchValue);
        return;
    }
    else {
        BtreeSearch(node->C[i], searchValue, level + 1);
    }
}
//삽입
void BtreeSplitChild(struct Node* ParentNode, int idx) {
    struct Node* rightNode = BtreeNodeCreate();
    struct Node* leftNode = ParentNode->C[idx];
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
void BtreeInsertNonFull(struct Node* node, int inputValue) {
    int i = node->lenKey;
    if (node->isLeaf == true) {
        while (i >= 1 && inputValue < node->key[i]) {
            node->key[i + 1] = node->key[i];
            i -= 1;
        }
        node->key[i + 1] = inputValue;
        node->lenKey += 1;
    }
    else {
        while (i >= 1 && inputValue < node->key[i]) {
            i -= 1;
        }
        i += 1;
        if (node->C[i]->lenKey == 2 * MinDeg - 1) {
            BtreeSplitChild(node, i);
            if (inputValue > node->key[i]) {
                i += 1;
            }
        }
        BtreeInsertNonFull(node->C[i], inputValue);
    }
}
void BtreeInsertNode(struct BTREE* btree, int inputValue) {
    struct Node* r = BtreeNodeCreate();
    r = btree->root;
    if (r->lenKey == 2 * (MinDeg)-1) {
        struct Node* s = BtreeNodeCreate();
        btree->root = s;
        s->isLeaf = false;
        s->lenKey = 0;
        s->C[1] = r;
        BtreeSplitChild(s, 1);
        BtreeInsertNonFull(s, inputValue);
    }
    else {
        BtreeInsertNonFull(r, inputValue);
    }
}
// 삭제
int DeletePred(struct BTREE* tree, struct Node* node) {
    int pred;
    struct Node* curr = node;
    if (node->isLeaf == true) {
        pred = node->key[node->lenKey];
        node->lenKey--;
        return pred;
    }
    while (curr->C[curr->lenKey + 1]->isLeaf != true) {
        curr = curr->C[curr->lenKey + 1];
    }
    pred = curr->C[curr->lenKey + 1]->key[curr->lenKey];
    DeleteBTree(tree, node, pred);
    return pred;
}
int DeleteSucc(struct BTREE* tree, struct Node* node) {
    int succ;
    struct Node* curr = node;
    if (node->isLeaf == true) {
        succ = node->key[1];
        for (int i = 1; i < node->lenKey; i++) {
            node->key[i] = node->key[i + 1];
        }
        node->lenKey--;
        return succ;
    }
    while (curr->C[1]->isLeaf != true) {
        curr = curr->C[1];
    }
    succ = curr->C[1]->key[1];
    DeleteBTree(tree, node, succ);
    return succ;
}
void DeleteSibling(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx) {
    struct Node* targetCNode = parentNode->C[targetCidx];
    struct Node* siblingNode;
    if (targetCidx < siblingCidx) {
        siblingNode = parentNode->C[siblingCidx];
        targetCNode->key[targetCNode->lenKey + 1] = parentNode->key[targetCidx];
        targetCNode->lenKey++;
        parentNode->key[targetCidx] = siblingNode->key[1];
        for (int i = 1; i <= siblingNode->lenKey - 1; i++) {
            siblingNode->key[i] = siblingNode->key[i + 1];
        }
        targetCNode->C[targetCNode->lenKey + 1] = siblingNode->C[1];
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            siblingNode->C[i] = siblingNode->C[i + 1];
        }
        siblingNode->lenKey--;
    }
    else {
        siblingNode = parentNode->C[siblingCidx];
        for (int i = targetCNode->lenKey; i >= 1; i--) {
            targetCNode->key[i + 1] = targetCNode->key[i];
        }
        targetCNode->key[1] = parentNode->key[targetCidx - 1];
        targetCNode->lenKey++;
        parentNode->key[targetCidx - 1] = siblingNode->key[siblingNode->lenKey];
        for (int i = targetCNode->lenKey + 1; i >= 1; i--) {
            targetCNode->C[i + 1] = targetCNode->C[i];
        }
        targetCNode->C[1] = siblingNode->C[siblingNode->lenKey + 1];
        siblingNode->lenKey--;
    }
    return;
}
void DeleteMerge(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx) {
    struct Node* targetNode;
    struct Node* siblingNode;
    int tmp;
    if (targetCidx < siblingCidx) {
        targetNode = parentNode->C[targetCidx];
        siblingNode = parentNode->C[siblingCidx];
        tmp = parentNode->key[targetCidx];
        for (int i = targetCidx; i <= parentNode->lenKey - 1; i++) {
            parentNode->key[i] = parentNode->key[i + 1];
        }
        for (int i = targetCidx + 1; i <= parentNode->lenKey; i++) {
            parentNode->C[i] = parentNode->C[i + 1];
        }
        parentNode->lenKey--;
        targetNode->key[targetNode->lenKey + 1] = tmp;
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            targetNode->key[i + targetNode->lenKey + 1] = siblingNode->key[i];
        }
        for (int i = 1; i <= siblingNode->lenKey + 1; i++) {
            targetNode->C[i + targetNode->lenKey + 1] = siblingNode->C[i];
        }
        targetNode->lenKey += siblingNode->lenKey + 1;
        parentNode->C[targetCidx] = targetNode;
    }
    else {
        siblingNode = parentNode->C[targetCidx];
        targetNode = parentNode->C[siblingCidx];
        tmp = parentNode->key[siblingCidx];
        for (int i = siblingCidx; i <= parentNode->lenKey - 1; i++) {
            parentNode->key[i] = parentNode->key[i + 1];
        }
        for (int i = siblingCidx + 1; i <= parentNode->lenKey; i++) {
            parentNode->C[i] = parentNode->C[i + 1];
        }
        parentNode->lenKey--;
        targetNode->key[targetNode->lenKey + 1] = tmp;
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            targetNode->key[i + targetNode->lenKey + 1] = siblingNode->key[i];
        }
        for (int i = 1; i <= siblingNode->lenKey + 1; i++) {
            targetNode->C[i + targetNode->lenKey + 1] = siblingNode->C[i];
        }
        targetNode->lenKey += siblingNode->lenKey + 1;
        parentNode->C[targetCidx] = targetNode;
    }
    free(siblingNode);
    if (tree->root->lenKey == 0) {
        tree->root = targetNode;
    }
    return;
}
void DeleteInternalNode(struct BTREE* tree, struct Node* curNode, int dVal, int targetIdx) {
    int i = targetIdx;
    if (curNode->isLeaf) {
        while (i < curNode->lenKey) {
            curNode->key[i] = curNode->key[i + 1];
            i++;
        }
        curNode->lenKey--;
        return;
    }
    else {
        if (curNode->C[targetIdx]->lenKey > MinDeg - 1) {
            curNode->key[targetIdx] = DeletePred(tree, curNode->C[targetIdx]);
        }
        else if (curNode->C[targetIdx + 1]->lenKey > MinDeg - 1) {
            curNode->key[targetIdx] = DeleteSucc(tree, curNode->C[targetIdx + 1]);
        }
        else {
            DeleteMerge(tree, curNode, targetIdx, targetIdx + 1);
            DeleteInternalNode(tree, curNode->C[targetIdx], dVal, MinDeg);
        }
    }
    return;
}
void DeleteBTree(struct BTREE* tree, struct Node* curNode, int dVal) {
    int i = curNode->lenKey;
    int j = 0;
    while (i <= curNode->lenKey && dVal <= curNode->key[i]) {
        i--;
    }
    i++;
    if (curNode->isLeaf) {
        if (curNode->key[i] == dVal) {
            j = i;
            while (j < curNode->lenKey) {
                curNode->key[j] = curNode->key[j + 1];
                j++;
            }
            curNode->lenKey--;
            return;
        }
        printf("값 %d을 찾을 수가 없음\n", dVal);
        return;
    }
    else {
        if (curNode->key[i] == dVal && i <= curNode->lenKey) {
            DeleteInternalNode(tree, curNode, dVal, i);
            return;
        }
        else if (curNode->C[i]->lenKey >= MinDeg) {
            DeleteBTree(tree, curNode->C[i], dVal);
        }
        else {
            if (i != 1 && i < curNode->lenKey + 1) {
                if (curNode->C[i]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else if (curNode->C[i + 1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else if (i == 1) {
                if (curNode->C[i + 1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else {
                if (curNode->C[i - 1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i - 1); //왼쪽이랑 합체시키기
                    DeleteBTree(tree, curNode->C[i-1], dVal);
                    if (curNode->lenKey == 0) {
                        free(curNode);
                    }
                    return;
                }
            }
            DeleteBTree(tree, curNode->C[i], dVal);
            if (curNode->lenKey == 0) {
                free(curNode);
            }
        }
    }
    return;
}
