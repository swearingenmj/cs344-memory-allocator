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

struct block {
        struct block *next;
        int size;     // Bytes
        int in_use;   // Boolean
} block;

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

void *myalloc(int size) {

    // use sbrk to allocate a chunk of data space
    // node information:
        // in-use(true or false)?
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

    // I used this website to guide the construction of the myalloc method body:
        //http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html

    struct block *current, *previous;

    while (((current->size) < size) || (((current->in_use) == 1) && (current->next != NULL))) {
        previous = current;
        current = current->next;
    }

    if ((current->size) == size) {
        current->in_use = 1;
    } else if ((current->size) > size) {
        current->in_use = 1;
    } else {
        // if there was no room
        // return NULL
        return NULL;
    }

    // what we are returning:
    // malloc will return a pointer to the data.. NOT a pointer to the struct block
    // the data start at n bytes past the struct, where n is PADDED_SIZE(sizeof(struct block))
    // padded_struct_block_size = PADDED_SIZE(sizeof(struct block));

    // ... All the machinations to allocate go here ...
    int padded_block_size = PADDED_SIZE(sizeof(struct block));
    return PTR_OFFSET(current, padded_block_size);
}

int main(void){ 
    void *p;

    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    printf("%p\n", p);
}