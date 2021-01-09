#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 노드들
struct Node {
    struct Node** C;
    bool isLeaf;
    int* key;
    int lenKey;
    int t; // 트리의 차수 정보
};
// B-tree
struct BTree {
    // t = Minimun Degree, 최소 차수. 노드에 들어있는 키의 개수와 노드가 가질수 있는 자식의 수를 결정한다.
    // 조건, t>=2
    //  mindeg  | key의 개수   | 자식의 개수
    //  ================================
    //          | min t-1    | min t
    //    t     | max 2t-1   | max 2t
    int t;
    struct Node* root;
};


void splitNode(struct Node* parent, int index) {
    // 노드의 키가 max:2t-1 까지 꽉 차게 되면 노드는 분할을 시작한다.
    // 노드는 t-1개의 키를 가지는 자식 노드 2개를 생성하며
    // 본인노드는 1개의 키를 가진다.
    // 키의 개수 1
    //        / \
    //      t-1 t-1    분할된 노드의 모든 키의 개수는  2*(t-1)+1 = 2t-1로 분할전과 같다.
    // 왼쪽 자식노드가 가지는 인덱스는 0 to t-2 (t-1개)까지
    //         본인 노드의 인덱스는 t-1 (1개)
    //    오른쪽 자식 노드의 인덱스는 t to 2t-1(t-1개)이다.
    printf("split\n");

    struct Node* z = malloc(sizeof(struct Node));//분할에 필요한 새로운 노드를 만든다.
    z->key = malloc(sizeof(int));
    z->t = parent->t;

    if (parent->C==NULL){ //처음 노드를 분할 할 때 노드가 없으면 노드를 생성한다
        struct Node* y = malloc(sizeof(struct Node));
        y->key = malloc(sizeof(int));
        y->isLeaf = parent->isLeaf; // 처음에는 부모의 리프 정보를 넘겨준다.
        y->t = parent->t;
        y->lenKey=0; //지금까지 새로운 노드를 초기화 하는 단계

        z->isLeaf = parent->isLeaf;

        int *childarray[2*(parent->t)];
        parent->C = &childarray;
        childarray[0] = &y;
        childarray[1] = &z;

        z->isLeaf = parent->isLeaf;
        for (int j = 0; j <= (parent->t) - 1; j++) {
            y->key[j] = parent->key[j];
            z->key[j] = parent->key[j + parent->t];

            y->lenKey+=1;
            z->lenKey+=1;
            parent->lenKey-=1;
        }
        parent->key[0] = parent->key[(parent->t)-1];
        parent->isLeaf=false;



    }
//    else {
//        struct Node *y = (parent->C);
//        z->isLeaf = y->isLeaf;
//        z->lenKey = (parent->t) - 1;
//        for (int j = 1; j < (parent->t) - 1; j++) {
//            z->key[j] = parent->key[j + parent->t - 1];
//        }
//
//        if (y->isLeaf == false) {
//            for (int j = 1; j < parent->t; j++) {
//                z->C[j] = y->C[j + (parent->t)];
//            }
//        }
//        y->lenKey = (parent->t) - 1;
//
//        for (int j = parent->lenKey; j > index + 1; j--) {
//            parent->C[j + 1] = parent->C[j];
//        }
//        parent->C[index + 1] = z;
//        for (int j = parent->lenKey; j > index; j--) {
//            parent->key[j + 1] = parent->key[j];
//        }
//        parent->key[index] = y->key[parent->t];
//        parent->lenKey = parent->lenKey + 1;
//    }


    return;
}

void printTree(struct Node* node, int level){
    if (node->isLeaf==true){
        //조회만 하면 된다.
        //k = 배열의 개수
        printf("level: %d // ",level);
        for(int i=0; i<node->lenKey; i++) {
            printf("%d ", node->key[i]);
        }
    }
    if (node->isLeaf !=true){
        for (int i=0; i<node->lenKey+1;i++){
            printTree((node->C)[i], level+1);
        }

    }

}

//void insertNotFull(struct Node* node, int inputval){
//    int i = node->lenKey;
//    if (node->isLeaf == true){
//        while(i>=1 && inputval<(node->key)[i]) {
//            node->key[i+1] = node->key[i];
//            i-=1;
//        }
//        node->key[i+1]=k;
//        node->lenKey+=1;
//    }
//    else{
//        while(i>=1 && inputval< (node->key)[i]){
//            i-=1;
//        }
//        i+=1;
//        if((node->C[i])->lenKey == 2*(root->t)-1){
//
//        }
//    }
//
//}
void insertKey(int inputVal, struct BTree* btree, struct Node* node) {
    //키가 삽입되는 경우를 3가지로 나눈다.
    // 노드가 없을때, (처음의 경우로 노드를 생성하고, 그 노드에 키를 삽입한다.)
    // 노드가 있는데, 루트
    printf("%d\n", inputVal);
    // insert not full
    if (node == NULL) {
        struct Node* newNode = malloc(sizeof(struct Node));
//        printf("size of btree->t : %d\n", sizeof(btree->t));
        newNode->key = malloc(sizeof(int));
        newNode->key[0] = inputVal;
        newNode->lenKey = 1;
        newNode->t = btree->t;
        newNode->isLeaf = true;
        btree->root = newNode;
    }// 노드가 꽉차있지 않을
    else if (node->lenKey < 2 * (btree->t) - 2) {
//        printf("btree->t : %d\n", btree->t);
//        printf("lenKey : %d\n", node->lenKey);
        for (int i = node->lenKey-1; i >= 0; i--) {
            // inputVal 보다 큰 값들 한 칸씩 밀기
            if (node->key[i] >= inputVal) {
                node->key[i + 1] = node->key[i];
            }
            else {
                node->key[i + 1] = inputVal;
                break;
            }
            if (i == 0) {
                node->key[i] = inputVal;
            }
        }
        node->lenKey++;
//        printf("%d", btree->t);
//        printf("node -> lenKey : %d\n", node->lenKey);
    }
    else {
        //꽉차있으면 스플릿하고 노드 삽입
        splitNode(node, 0);
    }
}
int main() {
    struct BTree* btree = malloc(sizeof(struct BTree));
    btree->root = NULL;
    btree->t = 3; //최대 키 갯수를 구하기 위한 미지수, 2*t-2 == MAX
    insertKey(9, btree, btree->root);
    insertKey(8, btree, btree->root);
    insertKey(7, btree, btree->root);
    insertKey(6, btree, btree->root);
    insertKey(5, btree, btree->root);
    insertKey(4, btree, btree->root);
    insertKey(3, btree, btree->root);

    printTree(btree->root,0);

    printf("\n");
    printf("%d\n", btree->root->key[0]);
    printf("%d\n", btree->root->key[1]);
    printf("%d\n", btree->root->key[2]);
    printf("%d\n", btree->root->key[3]);
}