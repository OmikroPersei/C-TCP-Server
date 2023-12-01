#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FREE(ptr) free(ptr)
#define MALLOC_INT(count) malloc(count * sizeof(unsigned int))
#define MEMCPY(dst, src) memcpy(dst, src, sizeof(src))

void swap(unsigned int* ptr_1, unsigned int* ptr_2)
{
	unsigned int temp = *ptr_1;
	*ptr_1 = *ptr_2;
	*ptr_2 = temp;
}

void bubble(unsigned int arr[], unsigned int how_many)
{
	int i, j;
	for (i = 0; i < how_many; i++)
	{
		for (j = how_many - 1; j > i; j--)
		{
			if (arr[j - 1] < arr[j])
			{
				swap(&arr[j], &arr[j - 1]);
			}
		}

	}
}