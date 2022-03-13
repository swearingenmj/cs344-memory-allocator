#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;  // Head of the list, empty

struct block {
        struct block *next;
        size_t size;     // Bytes
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
        printf("[%ld,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

void split(struct block *b, size_t size){

    struct block *new = PTR_OFFSET(b, size + PADDED_SIZE(sizeof(struct block)));
    new->size = (b->size) - size - sizeof(struct block);
    new->in_use = 1;
    new->next = b->next;
    b->size = size;
    b->in_use = 0;
    b->next = new;

}

void myfree(void *p) {

    // start cur at head
    struct block *current = head;

    // while cur->next isn't NULL:
    while (current->next != NULL) {

        // if cur is not in_use and next node not in use:
        if ((current->in_use == 0) && (current->next->in_use == 0)) {

            // add the next node's region's size to cur's
            current->size += (current->next->size) + sizeof(struct block);

            // make cur's next pointer skip the next node
            current->next = current->next->next;

        } else {
            // mov cur to next node
            current = current->next;
        }   
    }
}

void *myalloc(int size) {

    if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->in_use = 0;
    }

    struct block *current = head;
    int padded_size = PADDED_SIZE(size);

    // Walk the list looking for a free node
    while (current != NULL) {

        // If n's data big enough for user request
        if (current->in_use == 0 && current->size >= padded_size) {
            
            // Mark n as used
            current->in_use = 1;

            // Return address of n's data plus padding
            int padded_block_size = PADDED_SIZE(sizeof(struct block));
            return PTR_OFFSET(current, padded_block_size);
        }
    }

    // If we get here, there was no room
    // Return NULL
    return NULL;
}

int main(void){ 
    void *p;

    myalloc(10);     print_data();
    p = myalloc(20); print_data();
    myalloc(30);     print_data();
    myfree(p);       print_data();
    myalloc(40);     print_data();
    myalloc(10);     print_data();
}