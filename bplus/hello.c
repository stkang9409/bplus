#include <stdio.h>

int main() {
	int arr[5] = { 1,2,3,4,5 };
	int arr2[2][5] = { { 1,2,3,4,5 },{ 1,2,3,4,5 } };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%d ", arr2[i][j]);
		}
		printf("\n");
	}
	return 0;
}