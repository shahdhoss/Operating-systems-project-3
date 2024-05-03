#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
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

struct head* after(struct head *block) {
    return (struct head*) (char*) (block + block->size + HEAD);
}
struct head* before (struct head *block){    
	return (struct head*) (char*) block - (block->size + HEAD);
}
struct head* split (struct head* block, int size){
	int rsize = block->size - (size+ HEAD);
	block->size =rsize;
	struct head *splt = (char*) block->size+HEAD; 
	splt->bsize =rsize;
	splt->bfree= FALSE;
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
	struct head *new =mmap(NULL, ARENA, PROT_READ| PROT_WRITE,MAP_PRIVATE| MAP_ANONYMOUS,-1,0);
	if(new==MAP_FAILED){
		printf("mmap failed: error %d\n",errno);
		return NULL;
	}
	uint size= ARENA - 2*HEAD;
	new->bfree= FALSE;
	new->bsize= 0;
	new->free= TRUE;
	new->size=size;
	struct head* sentinel = after(new);
	sentinel->bfree = FALSE;
	sentinel->bsize= size;
	sentinel->free=FALSE;
	sentinel->size= 0;
	arena = (struct head*)new;
	return arena;
}
struct head* flist;
void detach(struct head *block) {
    if (block->next != NULL) {
    	if(block->prev!=NULL){
        	block->prev->next = block->prev;
    	}
    	else{
    		flist=block->next;
    	}
    }
}
void insert (struct head* block) {
	if(flist!=NULL){
		block->next=flist;
	}
	flist=block;
}
void *dalloc(size_t request){
	if(request<=0){
		return NULL;
	}
	int size=adjust(request);
	struct head *taken=find(size);
	if(taken==NULL)
		return NULL;
	else
		return taken;
}

int main() {
    /*struct head *block1 = malloc(sizeof(struct head));
    struct head *block2 = malloc(sizeof(struct head));
    insert(block1);
    insert(block2);
    detach(block1);
    free(block1);
    free(block2);
    
    return 0;*/
}

