#include "dlmall.h"
int adjust(size_t request){
	double blocks=request / ALIGN;
	return (int)blocks+1;
}
struct head* find(int size){
	int blocks=adjust(size);
	if(flist==NULL){
		printf("flist is null\n");
		return NULL;
	}
	struct head* ptr=flist;
	struct head* block=ptr;
	flist=split(block,size);
	detach(block);
	return block;
}
void *dalloc(size_t request){
	if(request<=0){
		return NULL;
	}
	int size = adjust(request);
	struct head *taken = find(size);
	if(taken==NULL){
		return NULL;
	}
	else
		return taken;
}
void dfree(void *memory){
	if(memory!=NULL){
		struct head* block=(struct head*)((char*)memory - sizeof(struct head));
		struct head *aft=(struct head*)((char*)memory + block->size);
		block->free=TRUE;
		aft->bfree=TRUE;
		insert(memory);
	}
	return;
}
