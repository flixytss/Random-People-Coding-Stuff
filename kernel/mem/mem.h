// bonk enjoyer (dorito girl)

// Should i write down everything i did? (Pumpkicks)
#ifndef _MEM_H
#define _MEM_H

#include <string.h> // So you can use string.h from here
//idk but they say aligning is important
#define ALIGN8(x) (((x) + 7) & ~7)
//this is block of memory
typedef struct block block ;
struct block{
    int size;
    int free;
    block* next;
    
};

static block* find_free_block(unsigned long size) ;
//helper functions to allocate memory
static block* find_free_block(unsigned long size) ;
static block* create_block(unsigned long size);
static void split_block(block * b,unsigned long size);

void kalloc_init();
void* kmalloc(unsigned long size);

void kfree(void* p);
void combine_blocks();

#endif
