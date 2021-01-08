#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 노드들
struct Node{
	struct Node** C;
	bool isLeaf;
	int* key;
	int lenKey;
	int t;
};

// B-tree
struct BTree {
	// 2*t -1 => 부모노드 사이즈
	// ceiling(t-1) => 자식노드 최소 사이즈
	int t;
	struct Node* root;
};

void insert(int inputVal, struct BTree *btree, struct Node *node) {
	
	if (node == NULL) {
		struct Node* newNode=malloc(sizeof(struct Node));
		int array[(btree->t)*2];

		newNode->key = array;
		array[0]=inputVal;

		newNode->lenKey = 1;
		newNode->t = btree->t;
		newNode->isLeaf = true;
		btree->root = newNode;
		

	}else if (node->lenKey <= 2*(btree->t)-1){
		// printf("%d\n", inputVal);

		for (int i=node->lenKey; i>=0;i--){
			
			if (node->key[i] >= inputVal){
				node->key[i+1]=node->key[i];
				
			}else{
				node->key[i+1] = inputVal;

				break;
			}
			if(i==0){
				node->key[i]=inputVal;

			}
		}
		
		node->lenKey += 1;

	    printf("1\n");
		// if(node->lenKey > 2*(node->t)-1){
		// 	printf("need to split\n");
		// }
	}
 

}

int main() {
	struct BTree* btree=malloc(sizeof(struct BTree));
	btree->root = NULL;
	btree->t = 3; // 2t-2 
	insert(8, btree,btree->root);
	insert(7, btree,btree->root);
	insert(6, btree,btree->root);
	insert(5, btree,btree->root);
	insert(4, btree,btree->root);
	insert(3, btree,btree->root);

	printf("%d\n", btree->root->key[0]);
	printf("%d\n", btree->root->key[1]);
	printf("%d\n", btree->root->key[2]);
	printf("%d\n", btree->root->key[3]);
	printf("%d\n", btree->root->key[4]);




}
