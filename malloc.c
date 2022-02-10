#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - ((x) - 1) & (ALIGNMENT - 1))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

int main(void){ // int argc, char *argv[]) {
    void *p;

    print_data();
    p = myalloc(64);
    print_data();

    // print_data();
    // p = myalloc(16);
    // print_data();
    // p = myalloc(16);
    // printf("%p\n", p);
}

void myalloc() {
    void *heap = sbrk(1024);
    struct block *head = NULL;  // Head of the list, empty
    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->in_use = 0;
    }

}

struct block {
        struct block *next;
        int size;     // Bytes
        int in_use;   // Boolean
};

void print_data(void) {
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

#if 0
    In myalloc() allocate a certain number of bytes and return a pointer
        a. call sbrk() to get space
        b. build a linked-list node inside the space to indicate the
            size and 'in-use' status
        c. walk the linked list in a loop to look for the first node that is:
            i. not in use
            ii. big enough to hold the requested amount+padding
        d. if the block is found:
            i. mark it 'in-use'
            ii. return a pointer to the user data just after the linked
                list node+padding
        e. if no block is found:
            i. return NULL
#endif