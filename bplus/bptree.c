#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ORD 2 //order
#define MIN -999999999

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

//삭제 함수
void DeleteMerge(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteSibling(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteMergeForLeaf(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteSiblingForLeaf(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx);
void DeleteIndex(struct BTREE* btree, struct Node* node, int dVal, int targetIdx);
int DeleteFromTree(struct BTREE* btree, struct Node* node, int dVal);

//확인 함수
void BtreeDataSearch(struct Node* node);
void BtreeSearch(struct Node* node, int searchValue, int level);
void PrintByDFS(struct Node* node, int cnt);
void printTree(struct Node* node, int cnt);

//삽입함수
void InsertData(struct Node* node, int inputVal);
void InsertToTree(struct BTREE* tree, int inputVal);
void Split(struct Node* node, int idx);

//구조체 선언
struct BTREE* initTree();
struct Node* createNode();

int main() {
    int JenCase[12] = { 1,4,7,10,17,21,31,25,19,20,28,42 };
    int Test[8] = { 21,31,20,10,7,25,42,4 };
    struct Node* flag = createNode();
    struct BTREE* BTree = initTree();
    struct Node* rootNode = createNode();
    BTree->root = rootNode;
    flag->nextNode = BTree->root;
    for (int i = 0; i < 90; i++) {
        InsertToTree(BTree, i);
    }

    BtreeDataSearch(flag->nextNode);
    printf("\n\n");
    PrintByDFS(BTree->root, 0);
    printf("\n\n");
    DeleteFromTree(BTree, BTree->root, 15);
    BtreeDataSearch(flag->nextNode);
    printf("\n\n");
    PrintByDFS(BTree->root, 0);
    printf("\n\n");
    DeleteFromTree(BTree, BTree->root, 47);
    BtreeDataSearch(flag->nextNode);
    printf("\n\n");
    PrintByDFS(BTree->root, 0);
    printf("\n\n");
    DeleteFromTree(BTree, BTree->root, 31);
    BtreeDataSearch(flag->nextNode);
    printf("\n\n");
    PrintByDFS(BTree->root, 0);
    printf("\n\n");

    return 0;
}
void BtreeDataSearch(struct Node* node) {
    for (int i = 1; i <= node->lenKey; i++) {
        printf("%d ", node->key[i]);
    }
    if (node->nextNode != MIN) {
        printf(" -> ");
        BtreeDataSearch(node->nextNode);
    }
}
void BtreeSearch(struct Node* node, int searchValue, int level) {
    int i = 1;
    while (i <= node->lenKey && searchValue > node->key[i]) {
        i += 1;
    }
    if (i <= node->lenKey && searchValue == node->key[i]) {
        printf("found %d in %d ", node->key[i], level);
        if (node->isLeaf != true) {
            BtreeSearch(node->C[i], searchValue, level + 1);
            // 값에 따라 인덱스가 바뀌어야 하는 느낌
            // i랑 i+1 중 하나로...
        }
        printf("\n");
        return;
    }
    else if (node->isLeaf == true) {
        printf("MIN searchVal: %d\n", searchValue);
        return;
    }
    else {
        //printf("search child node\n");
        BtreeSearch(node->C[i], searchValue, level + 1);
    }
}
struct BTREE* initTree() {
    struct BTREE* tree = malloc(sizeof(struct BTREE));
    tree->root = MIN;
    return tree;
}
struct Node* createNode() {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->isLeaf = true;
    newNode->lenKey = 0;
    newNode->nextNode = MIN;
    return newNode;
}
void Split(struct Node* node, int idx) {
    struct Node* rightNode = createNode();
    struct Node* leftNode = node->C[idx];
    int T = ORD;
    if (leftNode->isLeaf == false) {
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
        rightNode->lenKey = T - 1;
        for (int j = 1; j <= T - 1; j++) {
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
        rightNode->nextNode = leftNode->nextNode;
        leftNode->nextNode = rightNode;
        node->key[idx] = leftNode->key[T];
        node->lenKey += 1;
    }
    // 리프인 경우
    // 아닌 경우
    return;
}

void printTree(struct Node* node, int cnt) {

    int countNode;

    if (node->isLeaf) {
        printf("%d ", node->lenKey);
    }
    else {

        printTree(node->C[node->lenKey + 1], cnt + 1);

    }

}

void PrintByDFS(struct Node* node, int cnt) {
    if (node->isLeaf) {
        for (int i = 1; i <= cnt; i++) {
            printf("--------------------|");
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
                printf("--------------------|");
            }
            printf("%d            \n", node->key[i]);
        }
        PrintByDFS(node->C[node->lenKey + 1], cnt + 1);
    }
}
void InsertData(struct Node* node, int inputVal) {
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
        if (node->C[i]->lenKey == 2 * ORD - 1) {
            Split(node, i);
            if (inputVal > node->key[i]) {
                i++;
            }
        }
        else if (node->key[i] < inputVal && node->C[i]->isLeaf == true) {
            node->key[i] = inputVal;
        }
        InsertData(node->C[i], inputVal);
    }
}
void InsertToTree(struct BTREE* tree, int inputVal) {
    struct Node* r = createNode();
    r = tree->root;
    if (r->lenKey == 2 * ORD - 1) {
        struct Node* s = createNode();
        tree->root = s;
        s->C[1] = r;
        s->isLeaf = false;
        s->lenKey = 0;
        Split(s, 1);
        InsertData(s, inputVal);
    }
    else {
        InsertData(r, inputVal);
    }
}

void DeleteMergeForLeaf(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx) {
    struct Node* leftNode;
    struct Node* rightNode;
    int tmp;
    if (targetCidx < siblingCidx) {
        leftNode = parentNode->C[targetCidx];
        rightNode = parentNode->C[siblingCidx];

        // i 범위 아닐 수도 있음
        for (int i = 1; i <= ORD - 1; i++) {
            leftNode->key[i + ORD - 1] = rightNode->key[i];
            leftNode->lenKey++;
        }

        leftNode->nextNode = rightNode->nextNode;
        free(rightNode);

        for (int i = targetCidx; i <= parentNode->lenKey - 1; i++) {
            parentNode->key[i] = parentNode->key[i + 1];
        }
        for (int i = targetCidx + 1; i <= parentNode->lenKey; i++) {
            parentNode->C[i] = parentNode->C[i + 1];
        }

        parentNode->lenKey--;
    }
    else {
        leftNode = parentNode->C[siblingCidx];
        rightNode = parentNode->C[targetCidx];

        for (int i = 1; i <= ORD - 1; i++) {
            leftNode->key[i + ORD - 1] = rightNode->key[i];
            leftNode->lenKey++;
        }

        leftNode->nextNode = rightNode->nextNode;
        free(rightNode);

        for (int i = targetCidx - 1; i <= parentNode->lenKey - 1; i++) {
            parentNode->key[i] = parentNode->key[i + 1];
        }
        for (int i = targetCidx; i <= parentNode->lenKey; i++) {
            parentNode->C[i] = parentNode->C[i + 1];
        }

        parentNode->lenKey--;
    }

    if (btree->root->lenKey == 0) {
        btree->root = leftNode;
    }
}
void DeleteSiblingForLeaf(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx) {
    // 리프노드일 경우
    struct Node* targetCNode = parentNode->C[targetCidx];
    struct Node* siblingNode = parentNode->C[siblingCidx];

    //형제가 오른쪽일 때
    if (targetCidx < siblingCidx) {
        targetCNode->key[targetCNode->lenKey + 1] = siblingNode->key[1];
        parentNode->key[targetCidx] = siblingNode->key[1];

        for (int i = 1; i <= siblingNode->lenKey - 1; i++) {
            siblingNode->key[i] = siblingNode->key[i + 1];
        }
    }
        //형제가 왼쪽일 때
    else {
        for (int i = targetCNode->lenKey; i >= 1; i--) {
            targetCNode->key[i + 1] = targetCNode->key[i];
        }
        targetCNode->key[1] = siblingNode->key[siblingNode->lenKey];
        parentNode->key[siblingCidx] = siblingNode->key[siblingNode->lenKey - 1];
    }
    siblingNode->lenKey--;
    targetCNode->lenKey++;
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
    if (tree->root->lenKey == 0) {
        tree->root = targetNode;
    }
    return;
}

void DeleteSibling(struct BTREE* btree, struct Node* parentNode, int targetCidx, int siblingCidx) {
    struct Node* targetCNode = parentNode->C[targetCidx];
    struct Node* siblingNode;
    if (targetCidx < siblingCidx) { // 오른쪽 형제에서 가지고옴지 //왼쪽으로 머
        siblingNode = parentNode->C[siblingCidx];
        targetCNode->key[targetCNode->lenKey + 1] = parentNode->key[targetCidx];
        targetCNode->lenKey++;
        // 형제 노드 첫 키 빼고 하나씩 당기기
        // 형제 노드 키 값 -1
        // 부모노드 키 형제 노드 키로 바꾸기
        parentNode->key[targetCidx] = siblingNode->key[1];
        for (int i = 1; i <= siblingNode->lenKey - 1; i++) {

            siblingNode->key[i] = siblingNode->key[i + 1];
        }
        // 형제 노드 첫 자식 가져오기
        // 형제 노드 첫 자식 타깃 노드를 마지막 자식으로 추가하기
        // 형제 노드 자식들 앞으로 떙기기
        targetCNode->C[targetCNode->lenKey + 1] = siblingNode->C[1];
        for (int i = 1; i <= siblingNode->lenKey; i++) {
            siblingNode->C[i] = siblingNode->C[i + 1];
        }
        siblingNode->lenKey--;
    }

    else {//왼쪽 형제에서 오른쪽으로 머지
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

int DeleteFromTree(struct BTREE* btree, struct Node* curNode, int dVal) {
    int retval = MIN;
    int i = 1;
    while (i <= curNode->lenKey && curNode->key[i] < dVal) {
        i++;
    }

    // 리프 노드일 경우
    if (curNode->isLeaf) {
        if (curNode->key[i] == dVal) {
            for (int j = i; j <= curNode->lenKey; j++) {
                curNode->key[j] = curNode->key[j + 1];
            }
            curNode->lenKey--;
        }
    }
    else {
        if (curNode->C[i]->lenKey > ORD - 1) {
            DeleteFromTree(btree, curNode->C[i], dVal);
        }
        else if (curNode->C[1]->isLeaf == false) {
            if (i != 1 && i < curNode->lenKey + 1) {
                if (curNode->C[i - 1]->lenKey > ORD - 1) {
                    DeleteSibling(btree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else if (curNode->C[i + 1]->lenKey > ORD - 1) {
                    DeleteSibling(btree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(btree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else if (i == 1) {
                if (curNode->C[i + 1]->lenKey > ORD - 1) {
                    DeleteSibling(btree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMerge(btree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else {
                if (curNode->C[i - 1]->lenKey > ORD - 1) {
                    DeleteSibling(btree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else {
                    DeleteMerge(btree, curNode, i, i - 1); //왼쪽이랑 합체시키기
                }
            }
            DeleteFromTree(btree, curNode->C[i], dVal);
            if (curNode->lenKey == 0) {
                free(curNode);
            }
        }
        else {

            if (i != 1 && i < curNode->lenKey + 1) {
                if (curNode->C[i - 1]->lenKey > ORD - 1) {
                    DeleteSiblingForLeaf(btree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else if (curNode->C[i + 1]->lenKey > ORD - 1) {
                    DeleteSiblingForLeaf(btree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMergeForLeaf(btree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else if (i == 1) {
                if (curNode->C[i + 1]->lenKey > ORD - 1) {
                    DeleteSiblingForLeaf(btree, curNode, i, i + 1); //오른쪽에서 빌려오기
                }
                else {
                    DeleteMergeForLeaf(btree, curNode, i, i + 1); //오른쪽이랑 합체시키기
                }
            }
            else {
                if (curNode->C[i - 1]->lenKey > ORD - 1) {
                    DeleteSiblingForLeaf(btree, curNode, i, i - 1); //왼쪽에서 빌려오기
                }
                else {
                    DeleteMergeForLeaf(btree, curNode, i, i - 1); //왼쪽이랑 합체시키기
                }
                DeleteFromTree(btree, curNode->C[i-1], dVal);
                if (curNode->lenKey == 0) {
                    free(curNode);
                }
                return 0;
            }
            DeleteFromTree(btree, curNode->C[i], dVal);
            if (curNode->lenKey == 0) {
                free(curNode);
            }
        }
    }
}