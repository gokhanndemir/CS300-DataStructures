// CS300's homework 1 that belongs to Gökhan Demir 26562 in 2020 Fall.
#include "my_heap.h"
#include <iostream>
#include <string>

My_heap::My_heap() // Implementing constructor.
{
	heap_begin=nullptr;
	blk=nullptr;
	used_bytes=0;
}

My_heap::~My_heap() // Implementing destructor.
{
	std::cout << "At destruction, the heap had a memory leak of " << used_bytes << " bytes." << std::endl;
	memory_block* temp;
	while(heap_begin!=nullptr)
	{
		temp=heap_begin->right;
		delete heap_begin;
		heap_begin=temp;
	}
}

memory_block* My_heap::bump_allocate(int num_bytes) // Implementing bump_allocate member function.
{
	if(MAX_CAPACITY-used_bytes>=num_bytes)
	{
		if(heap_begin!=nullptr) // If it is not the first to add.
		{
			blk->right=new memory_block(num_bytes, used_bytes);
			blk->right->left=blk;
			blk=blk->right;
			used_bytes+=num_bytes;
			return blk;
		}
		else // If it is the first to add.
		{
			heap_begin=new memory_block(num_bytes, used_bytes);
			heap_begin->left=nullptr;
			blk=heap_begin;
			used_bytes+=num_bytes;
			return blk;
		}
	}
	else
	{
		return nullptr; // Since no new allocations can be made in Img_heap.
	}
}

void My_heap::deallocate(memory_block* block_address)
{
	if(block_address->left!=nullptr && block_address->right!=nullptr)
	{
		if(block_address->left->used==true && block_address->right->used==false) // Merging
		{
			memory_block* temp4 = new memory_block();
			if(block_address->right->right==nullptr)
			{
				temp4->right=nullptr;
				blk=temp4;
			}
			else
			{
				temp4->right=block_address->right->right;
				block_address->right->right->left=temp4;
			}
			block_address->left->right=temp4;
			temp4->left=block_address->left;
			temp4->size=block_address->size + block_address->right->size;
			temp4->starting_address=block_address->starting_address;
			temp4->used=false;
			used_bytes-=block_address->size;
			delete block_address->right;
			delete block_address;
	    }
	    else if(block_address->left->used==false && block_address->right->used==true) // Merging
	    {
			memory_block* temp6 = new memory_block();
			temp6->right=block_address->right;
			if(block_address->left->left==nullptr)
			{
				heap_begin=temp6;
				temp6->left=nullptr;
			}
			else
			{
				block_address->left->left->right=temp6;
				temp6->left=block_address->left->left;
			}
			block_address->right->left=temp6;
			temp6->size=block_address->size + block_address->left->size;
			temp6->starting_address=block_address->left->starting_address;
			temp6->used=false;
			used_bytes-=block_address->size;
			delete block_address->left;
			delete block_address;
	    }
		else if(block_address->left->used==false && block_address->right->used==false) // Merging
	    {
			memory_block* temp9= new memory_block();
			if(block_address->right->right==nullptr)
			{
				temp9->right=nullptr;
				blk=temp9;
			}
			else
			{
				temp9->right=block_address->right->right;
				block_address->right->right->left=temp9;
			}
			if(block_address->left->left==nullptr)
			{
				heap_begin=temp9;
				temp9->left=nullptr;
			}
			else
			{
				block_address->left->left->right=temp9;
				temp9->left=block_address->left->left;
			}
			used_bytes-=block_address->size;
			temp9->used=false;
			temp9->starting_address=block_address->left->starting_address;
			temp9->size=block_address->size+block_address->right->size+block_address->left->size;
			delete block_address->right;
			delete block_address->left;
			delete block_address;
	    }
	    else // It means there won't be any merging.
	    {
			used_bytes-=block_address->size;
			block_address->used=false;
	    }
	}
	else if(block_address->left==nullptr && block_address->right!=nullptr) // Namely the first memory block.
	{
		if(block_address->right->used==false) // There will be merging.
		{
			memory_block* temp7= new memory_block();
			if(block_address->right->right==nullptr)
			{
				temp7->right=nullptr;
				blk=temp7;
			}
			else
			{
				temp7->right=block_address->right->right;
				block_address->right->right->left=temp7;
			}
			heap_begin=temp7;
			temp7->left=nullptr;
			used_bytes-=block_address->size;
			temp7->used=false;
			temp7->starting_address=0;
			temp7->size=block_address->size+block_address->right->size;
			delete block_address->right;
			delete block_address;
		}
		else // No merging.
		{
			used_bytes-=block_address->size;
			block_address->used=false;
		}
	}
	else if(block_address->left!=nullptr && block_address->right==nullptr) // Namely the last memory block.
	{
		if(block_address->left->used==false) // There will be merging.
		{
			memory_block* temp8= new memory_block();
			temp8->right=nullptr;
			if(block_address->left->left==nullptr)
			{
				heap_begin=temp8;
				temp8->left=nullptr;
			}
			else
			{
				block_address->left->left->right=temp8;
				temp8->left=block_address->left->left;
			}
			used_bytes-=block_address->size;
			temp8->used=false;
			temp8->starting_address=block_address->left->starting_address;
			temp8->size=block_address->size+block_address->left->size;
			blk=temp8;
			delete block_address->left;
			delete block_address;
		}
		else // No merging.
		{
			used_bytes-=block_address->size;
			block_address->used=false;
		}
	}
	else // If both block_adress's left and right are NULL. Namely heap with only one memory block.
	{
		used_bytes-=block_address->size;
		block_address->used=false;
	}
}

float My_heap::get_fragmentation()
{
	float fragmentation=0.0;
	int free_memory=MAX_CAPACITY-used_bytes;
	int biggest_free_block=0;
	if(heap_begin!=nullptr)
	{
		int not_used_count=0; // Number of blocks.
		memory_block* live=heap_begin; // To count number of blocks.
		while(live!=nullptr)
		{
			if(live->used==false)
			{
				not_used_count++;
		    }
			live=live->right;
		}
		if(not_used_count!=0)
		{
			memory_block* livee=heap_begin;
			while(livee!=blk)
			{
				if(livee->used==false&&livee->size>biggest_free_block)
				{
					biggest_free_block=livee->size;
				}
				livee=livee->right;
			}
			if(blk->used==false)
			{
				if(MAX_CAPACITY-(blk->size+blk->starting_address)+blk->size>biggest_free_block)
				{
					biggest_free_block=MAX_CAPACITY-(blk->size+blk->starting_address)+blk->size;
				}
			}
			else
			{
				if(MAX_CAPACITY-(blk->size+blk->starting_address)>biggest_free_block)
				{
					biggest_free_block=MAX_CAPACITY-(blk->size+blk->starting_address);
				}
			}
			fragmentation=float(free_memory - biggest_free_block)/free_memory*100;
		}
	}
	return fragmentation;
}

void My_heap::print_heap()
{
	int count=0, used_count=0; // Number of blocks.
	memory_block* temp5=heap_begin; // To count number of blocks.
	while(temp5!=nullptr)
	{
		if(temp5->used==true)
		{
			used_count++;
		}
		count++;
		temp5=temp5->right;
	}
	std::cout << "Maximum capacity of heap: " << MAX_CAPACITY << "B" << std::endl;
	std::cout << "Currently used memory (B): " << used_bytes << std::endl;
	std::cout << "Total memory blocks: " << count << std::endl;
	std::cout << "Total used memory blocks: " << used_count << std::endl;
	std::cout << "Total free memory blocks: " << count-used_count << std::endl;
	std::cout << "Fragmentation: " << get_fragmentation() << "%" << std::endl;
	std::cout << "------------------------------" << std::endl;
	if(count!=0)
	{
		memory_block* temp2=heap_begin;
		for (int i=0;i<count;i++)
		{
			std::string word;
			if(temp2->used==true)
			{
				word="True";
            }
	        else
	        {
				word="False";
	        }
			std::cout << "Block " << i << " \t\tUsed: " << word << " \tSize (B): " << temp2->size << " \tStarting Address: 0x" << std::hex << temp2->starting_address << std::dec << " \n";
			temp2=temp2->right;
		}
	}
	std::cout << "------------------------------" << std::endl;
	std::cout << "------------------------------" << std::endl;
}