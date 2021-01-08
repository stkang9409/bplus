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

void insert(int inputVal, struct BTree *btree) {
	if (btree->root == NULL) {
		struct Node* node=malloc(sizeof(struct Node));
		node->key[0] = inputVal;
		printf("%d ", node->key);
		node->lenKey = 1;
		node->t = btree->t;
		node->isLeaf = true;
		btree->root = node;
	}
}

int main() {
	struct BTree* btree=malloc(sizeof(struct BTree));
	btree->root = NULL;
	btree->t = 3;
	insert(1, btree);
	printf("%d", btree->root->key);
}
