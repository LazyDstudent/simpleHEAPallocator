#include <stdio.h>    
#include <string.h>   
#include "Heaps.h"    

int main() {
    printf("--- test  ---\n");
    char* ptr1 = (char*)myMALLOC(100);
    if (ptr1) {
        strcpy(ptr1, "Hello Heap!");
        printf("ptr1 says: %s (address: %p)\n", ptr1, (void*)ptr1);
    }

    printf("\n---test 2  ---\n");
    int* ptr2 = (int*)myMALLOC(sizeof(int) * 10);
    if (ptr2) {
        printf("ptr2 address: %p\n", (void*)ptr2);
    }

    printf("\n--- test 3  ---\n");
    myFREE(ptr1); 

    char* ptr3 = (char*)myMALLOC(16); 
    if (ptr3) {
        printf("ptr3 (reused space) addres: %p\n", (void*)ptr3);
        if (ptr1 == ptr3) {
            printf("sucxess: space was recycled!\n");
        } else {
            printf("note: ptr3 allocated at a different spot.\n");
        }
    }

    return 0;
}
