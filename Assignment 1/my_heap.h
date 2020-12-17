#define MAX_CAPACITY 512
struct memory_block
{
	memory_block() // my default constructor
	{right = nullptr;left = nullptr;used = true;size = 0;starting_address = 0;}
	memory_block(int number, int usedbytes) // my constructor
	{
		right=nullptr,left=nullptr;
		used=true;
		size=number;
		starting_address=usedbytes;
	}
	memory_block* right; // node to the right
	memory_block* left; // node to the left
	bool used; // if this memory block is reserved or not
	int size; // the number of bytes reserved in Img_heap
	int starting_address; // the starting address in Img_heap of the memory reserved by this block
};

class My_heap
{
private:
    memory_block * blk;
    memory_block* heap_begin;
    int used_bytes;
public:
    My_heap();
    ~My_heap();
    memory_block* bump_allocate(int num_bytes);
    void deallocate(memory_block* block_address);
    float get_fragmentation();
    void print_heap();
};