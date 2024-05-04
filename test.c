#include "dlmall.c"
int main() {
    flist=new();
    printf("Allocating memory...\n");
    void* memory = dalloc(4*sizeof(int));
    //void* memory2 = dalloc(4*sizeof(int));
    if (memory == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int* int_memory = (int*)memory;
    //int* int_memory2 = (int*)memory2;
    for (int i = 0; i < 4; i++) {
        *(int_memory + i) = i + 1;
        //*(int_memory2 + i) = i + 5;
    }

    printf("Values stored in allocated memory:\n");
    for (int i = 0; i < 4; i++) {
        printf("at memory location: %p ,value at index %d: %d\n" ,(int_memory+i), i, *(int_memory + i)); 
        //printf("at memory location: %p ,value at index %d: %d\n" ,(int_memory2+i), i, *(int_memory2 + i)); 
    }
    printf("Freeing memory...\n");
    dfree(memory); 
    //dfree(memory2);
    printf("Memory freed\n");

    return 0;
}
