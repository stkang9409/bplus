#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MinDeg 3
struct Node {
    struct Node* C[2 * MinDeg + 1];
    bool isLeaf;
    int key[2 * MinDeg];
    int lenKey;
    int t; // 트리의 차수 정보
};
struct BTREE {
    int t;
    struct Node* root;
};
struct Node* BtreeNodeCreate() {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->isLeaf = true;
    newNode->lenKey = 0;
    return newNode;
}
void DeleteBTree(struct BTREE* tree, struct Node* curNode, int dVal);
void DeleteInternalNode(struct BTREE* tree, struct Node* curNode, int dVal, int targetIdx);
int DeleteSucc(struct BTREE* tree, struct Node* node);
int DeletePred(struct BTREE* tree, struct Node* node);
void DeleteMerge(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteSibling(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx);
void BtreeCreate(struct BTREE* T);
void BtreeInsertNode(struct BTREE* btree, int inputValue);
void BtreeInsertNonFull(struct Node* node, int inputValue);
void BtreeSplitChild(struct Node* ParentNode, int idx);
void BtreeSearch(struct Node* node, int searchValue, int level);
int main() {
    struct BTREE* BTree = malloc(sizeof(struct BTREE));
    BtreeCreate(BTree);
    for (int i = 1; i < 20; i++)
    {
        BtreeInsertNode(BTree, i);
    }

    DeleteBTree(BTree, BTree->root, 15);
    for (int j = 1; j < 20; j++)
    {

        BtreeSearch(BTree->root, j, 0);
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
        printf("NULL in %d\n", level);
        return;
    }
    else {
        //printf("search child node\n");
        BtreeSearch(node->C[i], searchValue, level + 1);
    }
}
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
void BtreeCreate(struct BTREE* T) {
    struct Node* x = BtreeNodeCreate();
    T->root = x;
}
void DeleteSibling(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx) {

    // 오른쪽 형제 일때 ( targetCidx < siblingCidx)
    // 오른쪽 형제의 첫번째 키를 부모의 targetCidx의 키로 가지고 올라온다.
    // 오른쪽 형제의 왼쪽 자식은 C[targetCidx](타겟 자식)의 오른쪽 자식으로 붙여야함.
    // 오른쪽 형제 첫번째 키와 첫번째 자식 삭제
    struct Node* targetCNode = parentNode->C[targetCidx];
    struct Node* siblingNode;
    if (targetCidx < siblingCidx) {
        siblingNode = parentNode->C[siblingCidx];
        // 타깃 노드 키 마지막에 부모 노드 키 추가하기
        // 타깃 노드 lenKey 더하기 일
        targetCNode->key[targetCNode->lenKey + 1] = parentNode->key[targetCidx];
        targetCNode->lenKey++;
        // 형제 노드 첫 키 빼고 하나씩 당기기
        // 형제 노드 키 값 -1
        // 부모노드 키 형제 노드 키로 바꾸기
        parentNode->key[targetCidx] = siblingNode->key[0];
        for (int i = 1; i <= siblingNode->lenKey - 1; i++) {

            siblingNode->key[i] = siblingNode->key[i + 1];
        }
        // 형제 노드 첫 자식 가져오기
        // 형제 노드 첫 자식 타깃 노드를 마지막 자식으로 추가하기
        // 형제 노드 자식들 앞으로 떙기기
        targetCNode->C[targetCNode->lenKey + 2] = siblingNode->C[1];
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            siblingNode->C[i] = siblingNode->C[i + 1];
        }
        siblingNode->lenKey--;
    }
        //형제가 왼쪽일 때
    else {
        siblingNode = parentNode->C[siblingCidx];
        for (int i = targetCNode->lenKey; i >= 1; i--) {
            targetCNode->key[i+1] = targetCNode->key[i];
        }
        targetCNode->key[1] = parentNode->key[targetCidx-1];
        targetCNode->lenKey++;
        parentNode->key[targetCidx-1] = siblingNode->key[siblingNode->lenKey];
        for (int i = targetCNode->lenKey+1; i >= 1; i--) {
            targetCNode->C[i + 1] = targetCNode->C[i];
        }
        targetCNode->C[1] = siblingNode->C[siblingNode->lenKey+1];
        siblingNode->lenKey--;
    }
    return;
}
void DeleteMerge(struct BTREE* tree, struct Node* parentNode, int targetCidx, int siblingCidx) {

    // 부모 키 어디다가 저장
    // 부모 키 뒤에 있는 애들 앞으로 당기고
    // lenKey -1
    // target이랑 sibling 합쳐서 target에 다시 넣어,
    // sibling부터 해서 앞에서 하나씩 당기기
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
        for (int i = targetCidx+1; i <= parentNode->lenKey; i++) {
            parentNode->C[i] = parentNode->C[i + 1];
        }
        parentNode->lenKey--;
        targetNode->key[targetNode->lenKey+1] = tmp;
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            targetNode->key[i + targetNode->lenKey + 1] = siblingNode->key[i];
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
            targetNode->C[i + targetNode->lenKey + 1] = siblingNode->C[i];
        }
        targetNode->lenKey += siblingNode->lenKey + 1;
        parentNode->C[targetCidx] = targetNode;
    }
    if (tree->root->lenKey == 0) {
        tree->root = targetNode;
    }
    return;
}
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
    // node, dVal, targetIdx
    DeleteBTree(tree, node, pred); //고장나면 노드로 바꾸기
    return pred;
}
int DeleteSucc(struct BTREE* tree, struct Node* node) {

    int succ;
    struct Node* curr = node;
    if (node->isLeaf == true) {
        succ = node->key[1];
        for (int i = 1; i < node->lenKey ; i++) {
            node->key[i] = node->key[i + 1];
        } ///////////////////////////////////////18 18 19
        node->lenKey--;
        return succ;
    }
    while (curr->C[1]->isLeaf != true) {
        curr = curr->C[1]; //이건 맞음
    }
    succ = curr->C[1]->key[1];
    // node, dVal, targetIdx
    DeleteBTree(tree, node, succ); //고장나면 노드로 바꾸기
    return succ;
}
// 삭제 연산 구현
void DeleteInternalNode(struct BTREE* tree, struct Node* curNode, int dVal, int targetIdx) {
    int i = targetIdx;
    if (curNode->isLeaf) {
        while (i < curNode->lenKey) {
            curNode->key[i] = curNode->key[i + 1];
            i++;
        }
        //curNode->key[targetIdx] = 0;
        curNode->lenKey--;
        return;
    }
    else {
        // 일단 갖고 오고나서 삭제하기
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
        // 못 가지고 오면 합체 시키고 다시 삭제
    }
    return;
}
void DeleteBTree(struct BTREE* tree, struct Node* curNode, int dVal) {

    int i = curNode->lenKey;
    int j = 0;
    // dVal을 curNode->key에서 찾는다.
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
            //curNode->key[i] = 0;
            curNode->lenKey--;
            return;
        }
        printf("값 %d을 찾을 수가 없음", dVal);
        return;
    }
    else {

        if (curNode->key[i] == dVal) {
            DeleteInternalNode(tree, curNode, dVal, i);
            return;
        }
        else if (curNode->C[i]->lenKey >= MinDeg) {
            DeleteBTree(tree, curNode->C[i], dVal);
        }
        else {
            if (i != 1 && i < curNode->lenKey + 1) {
                // 우리가 찾으려는 값은 C[i+1]에 있다.
                // 왼쪽 형제는 C[i] 오른쪽 형제는 C[i+2]
                if (curNode->C[i]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i-1); //왼쪽에서 빌려오기
                }
                else if (curNode->C[i + 1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else if (i == 1) {
                if (curNode->C[i+1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else {
                if (curNode->C[i-1]->lenKey > MinDeg - 1) {
                    DeleteSibling(tree, curNode, i, i-1); //왼쪽에서 빌려오기
                }
                else {
                    DeleteMerge(tree, curNode, i, i-1); //왼쪽이랑 합체시키기
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