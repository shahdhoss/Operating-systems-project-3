#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/types.h>
struct head {
	uint16_t bfree;
	uint16_t bsize;
	uint16_t free;
	uint16_t size;
	struct head *next;
	struct head *prev;
};
#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))
#define MIN(size) (((size) > 8) ? (size) : 8)
#define LIMIT(size) (MIN(size) + HEAD)
#define MAGIC(MEMORY) ((struct head*)(MEMORY) - 1)
#define HIDE(block) ((void*)((struct head*)(block) + 1))
#define ALIGN 8
#define ARENA (64 * 1024)
#define order 64
struct head* after(struct head *block) {
    return (struct head*) (((char*) block) + block->size);
}
struct head* before (struct head *block){    
	return (struct head*) (((char*)block) - block->size);
}
struct head* split (struct head* block, int size){
	int rsize = block->size - (size+HEAD);
	block->size =rsize;
	struct head *splt = (struct head*)((char*)block + HEAD + rsize); //might be an issue here 
	splt->bsize = rsize;
	splt->bfree = FALSE;
	splt->free = TRUE;
	splt->size = size;
	struct head *aft= after(splt);
	aft->bsize = size;
	return splt;
}
struct head* arena = NULL;
struct head *new(){
	if(arena!=NULL){
		printf("one arena already allocated\n");
		return NULL;
	}
	struct head *new = mmap(NULL,ARENA,PROT_READ| PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);
	if(new==MAP_FAILED){
		printf("mmap failed: error %d\n",errno);
		return NULL;
	}
	uint size= ARENA - 2*HEAD;
	new->bfree = FALSE;
	new->bsize = 0;
	new->free = TRUE;
	new->size = size;
	struct head* sentinel = after(new); 
	sentinel->bfree = FALSE;
	sentinel->bsize= size;
	sentinel->free=FALSE;
	sentinel->size= 0;
	arena = (struct head*)new;
	return arena;
}
struct block{
	int size;
	struct block* next;
	struct block* prev;
};
struct head *flist;
void detach(struct head *block) {
    if (block->next != NULL) {
    	if(block->prev!=NULL){
        	block->prev->next = block->next;
    	}
	}
}
void insert (struct head* block) {
	if(flist!=NULL){
		block->next=flist;
	}
	flist=block;
}
struct head* merge(struct head* block) {
	struct head* aft = after(block);
	if (block->bfree) {
		struct head* before = block->prev;
		block->size = block->size + before->size;
		block->bfree = FALSE;
		block->bsize = 0;
	}
	if (aft->free) {
		block->size = block->size + aft->size;
	}
	return block;
}

