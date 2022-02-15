#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

// make linked list inside the above data space
struct block *head = NULL;  // Head of the list, empty

int main(void){ 
    void *p;

    print_data();
    p = myalloc(64);
    print_data();
}

void *myalloc(int size) {
    
    // use sbrk to allocate a chunk of data space
    // node information:
            // in-use?
            // size of allocated memory region
            // pointer to the next memory region

    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->in_use = 0;
    }

    // walk the list looking for a free node
    //     let n = the free node
    //     if n's data is big enough for the user request
    //         mark n as used
    //         return address of n's data

    //cries into code.. 

    // if there was no room
    // return NULL
    return NULL;

    // what we are returning:
    // malloc will return a pointer to the data.. NOT a pointer to the struct block
    // the data start at n bytes past the struct, where n is PADDED_SIZE(sizeof(struct block))
    // padded_struct_block_size = PADDED_SIZE(sizeof(struct block));

    // struct block *cur;
    // ... All the machinations to allocate go here ...
    // padded_block_size = PADDED_SIZE(sizeof(struct block));
    // return PTR_OFFSET(cur, padded_block_size);
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