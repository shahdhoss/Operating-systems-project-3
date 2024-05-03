#include "dlmall.c"
int main(){
	printf("Allocating memory...\n");
    void* memory = dalloc(4);
    for(int i=0;i<4;i++){
    	printf("allocated memory no. %d is %p \n",i,((char*)memory + i));
    }
    printf("Freeing memory...\n");
    dfree(memory);
    printf("After freeing\n");
    return 0;
}
