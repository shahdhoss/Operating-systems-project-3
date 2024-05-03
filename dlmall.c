#include "dlmall.h"
int adjust(size_t request){
	int blocks=request % ALIGN;
	if(blocks==round(blocks)){
		return blocks;
	}
	return ceil(blocks);
}
struct  head* find(int size){
	int blocks=adjust(size);
	if(flist==NULL){
		return NULL;
	}
	struct head* ptr=flist;
	struct head* block=flist;
	block->size=0;
	while(ptr->next!=NULL){
		if(blocks!=0){
			blocks--;
			if(ptr->size <= size){
				block->size=block->size+ptr->size;
				size = size - ptr->size;
			}
			else if (ptr->size > size && size!=0){
				struct head* splt=split(ptr,size);
				block->size=block->size+splt->size;
			}
		}
	}
	return block;
}
void *dalloc(size_t request){
	if(request<=0){
		return NULL;
	}
	int size = adjust(request);
	struct head *taken = find(size);
	if(taken==NULL)
		return NULL;
	else
		return taken;
}
void dfree(void *memory){
	if(memory!=NULL){
		struct head* block=(struct head*)((char*)memory - sizeof(struct head));
		struct head *aft=(struct head*)((char*)memory + block->size);
		block->free=TRUE;
		aft->bfree=TRUE;
	}
	return;
}
