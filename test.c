#include "dlmall.c"
int main() {
    flist=new();
    printf("Allocating memory...\n");
    void* memory = dalloc(4 * sizeof(int));
    if (memory == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int* int_memory = (int*)memory;
    for (int i = 0; i < 4; i++) {
        *(int_memory + i) = i + 1;
    }

    printf("Values stored in allocated memory:\n");
    for (int i = 0; i < 4; i++) {
        printf("Value at index %d: %d\n", i, *(int_memory + i)); 
    }

    printf("Freeing memory...\n");
    dfree(memory); 
    printf("Memory freed\n");

    return 0;
}
