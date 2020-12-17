#include "my_heap.h"
#include <iostream>
using namespace std;

int main()
{
	My_heap heap;

	heap.print_heap();

	memory_block* block = heap.bump_allocate(8);
	memory_block* block1 = heap.bump_allocate(12);
	memory_block* block2 = heap.bump_allocate(9);
	memory_block* block3 = heap.bump_allocate(23);

	heap.print_heap();

	heap.deallocate(block1);
	heap.print_heap();

	heap.deallocate(block2);
	heap.print_heap();

	return 0;
}