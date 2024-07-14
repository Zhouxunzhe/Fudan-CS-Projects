#pragma once
#ifndef _BLOCK_H
#define _BLOCK_H

#include "file_io.h"

// BEGIN: --------------------------------- DO NOT MODIFY! --------------------------------- 

typedef struct {
    // header section 
    short n_items;  //number of allocated ItemPtr's 
    short head_ptr;  //free space begin 
    short tail_ptr;  //free space end 

    // ItemID section 

    // Item section 

    char data[PAGE_SIZE - 3 * sizeof(short)];  //placeholder only 
} Block;


//  * ItemID: 32-bit unsigned int
//  * 31-th bit: not used, always be 0
//  * 30-th bit: availability (0: unavailable (because it has been used); 1: available (for future use))
//  * 29~15 bit: offset
//  * 14~ 0 bit: size

typedef unsigned int ItemID;
typedef char* ItemPtr;

#define get_item_id_availability(item_id) (((item_id) >> 30) & 1)
#define get_item_id_offset(item_id) (((item_id) >> 15) & ((1 << 15) - 1))
#define get_item_id_size(item_id) ((item_id) & ((1 << 15) - 1))

#define compose_item_id(availability, offset, size) ((((availability) & 1) << 30) | (((offset) & ((1 << 15) - 1)) << 15) | ((size) & ((1 << 15) - 1)))

#define get_item_id(block, idx) (*(ItemID*)((block)->data + sizeof(ItemID) * (idx)))

void init_block(Block* block);

// idx should not be out of range 
ItemPtr get_item(Block* block, short idx);

// there should be enough space 
short new_item(Block* block, ItemPtr item, short item_size);

// idx should not be out of range 
void delete_item(Block* block, short idx);

// END:   --------------------------------- DO NOT MODIFY! --------------------------------- 



// if item is NULL, print NULL 
typedef void (*printer_t)(ItemPtr item, short item_size);

void str_printer(ItemPtr item, short item_size);

void print_block(Block* block, printer_t printer);



typedef struct {
    size_t empty_item_ids;
    size_t total_item_ids;
    // tail_ptr - head_ptr + empty_item_ids * sizeof(ItemID) 
    size_t available_space;
} block_stat_t;

void analyze_block(Block* block, block_stat_t* stat);

void accumulate_stat_info(block_stat_t* stat, const block_stat_t* stat2);

void print_stat_info(const block_stat_t* stat);

#endif  _BLOCK_H 