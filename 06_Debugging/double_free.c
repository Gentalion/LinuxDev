#include <stdio.h>
#include <stdlib.h>

const int arraySize = 20;

int main () {
	int *array = malloc(sizeof(int) * arraySize);

	for (int i = 0; i++ ; i < arraySize) {
		array[i] = i;
	}

	free(array);
	printf("here\n");

	free(array);
	printf("still here\n");

	return 0;
}