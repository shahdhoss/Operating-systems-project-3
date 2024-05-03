#include "memory_block.c"
int adjust(size_t request);
struct head* split (struct head* block, int size);
struct  head* find(int size);
void *dalloc(size_t request);
void dfree(void *memory);
