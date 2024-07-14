#include "block.h"

#include <stdio.h>

#include <string.h>

#include<stdlib.h>

void init_block(Block* block) {
    block->n_items = 0;
    block->head_ptr = (short)(block->data - (char*)block);
    block->tail_ptr = (short)sizeof(Block);
    for (int i = 0; i < PAGE_SIZE - 3 * sizeof(short); i++) {
        block->data[i] = 0;
    }
}

ItemPtr get_item(Block* block, short idx) {
    if (idx < 0 || idx >= block->n_items) {
        printf("get item error: idx is out of range\n");
        return NULL;
    }
    ItemID item_id = get_item_id(block, idx);
    if (get_item_id_availability(item_id)) {
        printf("get item error: item_id is not used\n");
        return NULL;
    }
    short offset = get_item_id_offset(item_id);
    return (char*)block + offset;
}

short new_item(Block* block, ItemPtr item, short item_size) {
    ItemID item_id = 0;
    if (block->tail_ptr - block->head_ptr < item_size) {//space less than item_size
        return -1;
    }
    for (short idx = 0; idx < block->n_items; idx++) {//free item_id and space enough
        item_id = get_item_id(block, idx);
        if (get_item_id_availability(item_id) &&
            get_item_id_size(item_id) >= item_size) {
            block->tail_ptr -= item_size;
            short offset = get_item_id_offset(item_id);
            short size = get_item_id_size(item_id);
            short availability = get_item_id_availability(item_id);
            /*memcpy((char*)block + offset, item, item_size);*/
            for (int i = 0; i < item_size; i++) {
                block->data[offset - 3 * sizeof(short) + i] = item[i];
            }
            get_item_id(block, idx) = compose_item_id(0, block->tail_ptr, item_size);
            return idx;
        }
    }
    //no free item_id
    if (block->tail_ptr - block->head_ptr < item_size + sizeof(ItemID)) {
        //space is not enough for ItemID+Item
        return -1;
    }
    //no free item_id but space enough
    short idx = block->n_items;
    block->n_items++;
    block->tail_ptr -= item_size;

    get_item_id(block, idx) = compose_item_id(0, block->tail_ptr, item_size);
    item_id = get_item_id(block, idx);
    block->head_ptr += sizeof(ItemID);
    short offset = get_item_id_offset(item_id);
    /*memcpy((char*)block + offset, item, item_size);*/
    for (int i = 0; i < item_size; i++) {
        block->data[offset - 3 * sizeof(short) + i] = item[i];
    }
    return idx;
}

void delete_item(Block* block, short idx) {
    if (idx >= 0 && idx <= block->n_items) {
        ItemID item_id = get_item_id(block, idx);
        short size = get_item_id_size(item_id);
        short offset = get_item_id_offset(item_id);
        short availability = get_item_id_availability(item_id);

        if (availability == 0 && size != 0) {//only when not availability
            ItemID other_item_id;
            for (int i = 0; i < block->n_items; i++) {
                other_item_id = get_item_id(block, i);
                if (get_item_id_availability(other_item_id) == 0 &&
                    get_item_id_offset(other_item_id) < offset) {//only when not availability and before target
                    short tmp_offset = get_item_id_offset(other_item_id) + size;
                    short tmp_size = get_item_id_size(other_item_id);
                    get_item_id(block, i) = compose_item_id(0, tmp_offset, tmp_size);//modify other item_id
                }
            }

            ItemPtr item = get_item(block, idx);//get itemptr
            short str_length = (short)item - (short)block - block->tail_ptr;
            char* tmp_str = (char*)malloc(sizeof(char) * str_length);
            //memcpy(tmp_str, (char*)block + block->tail_ptr, str_length);//store data
            for (int i = 0; i < str_length; i++) {
                tmp_str[i] = block->data[block->tail_ptr - 3 * sizeof(short) + i];
            }

            for (int i = 0; i < size; i++) {//initial sapce
                block->data[block->tail_ptr - 3 * sizeof(short) + i] = 0;
            }
            block->tail_ptr += size;//change tail_ptr
            //memcpy((char*)block + block->tail_ptr, tmp_str, str_length);//switch item
            for (int i = 0; i < str_length; i++) {
                block->data[block->tail_ptr - 3 * sizeof(short) + i] = tmp_str[i];
            }
            if (idx + 1 == block->n_items) {//delete last available item_id
                for (int i = 1; i <= sizeof(ItemID); i++) {//from back to front 
                    block->data[block->head_ptr - 3 * sizeof(short) - i] = 0;
                }
                block->head_ptr -= sizeof(ItemID);
                block->n_items--;
            }
            else
                get_item_id(block, idx) = compose_item_id(1, 0, 0);//modify itme_id indexed by idx
        }
        else if (availability == 0 && size == 0) {
            for (int i = 1; i <= sizeof(ItemID); i++) {//from back to front 
                block->data[block->head_ptr - 3 * sizeof(short) - i] = 0;
            }
            block->head_ptr -= sizeof(ItemID);
            block->n_items--;
        }
    }
}

/* ----------------------  DO NOT MODIFY ------------------------ */

void str_printer(ItemPtr item, short item_size) {
    if (item == NULL) {
        printf("NULL");
        return;
    }
    short i;
    printf("\"");
    for (i = 0; i < item_size; ++i) {
        printf("%c", item[i]);
    }
    printf("\"");
}

void print_block(Block* block, printer_t printer) {
    short i, availability, offset, size;
    ItemID item_id;
    ItemPtr item;
    printf("----------BLOCK----------\n");
    printf("total = %d\n", block->n_items);
    printf("head = %d\n", block->head_ptr);
    printf("tail = %d\n", block->tail_ptr);
    for (i = 0; i < block->n_items; ++i) {
        item_id = get_item_id(block, i);
        availability = get_item_id_availability(item_id);
        offset = get_item_id_offset(item_id);
        size = get_item_id_size(item_id);
        if (!availability) {
            item = get_item(block, i);
        }
        else {
            item = NULL;
        }
        printf("%10d%5d%10d%10d\t", i, availability, offset, size);
        printer(item, size);
        printf("\n");
    }
    printf("-------------------------\n");
}

void analyze_block(Block* block, block_stat_t* stat) {
    short i;
    stat->empty_item_ids = 0;
    stat->total_item_ids = block->n_items;
    for (i = 0; i < block->n_items; ++i) {
        if (get_item_id_availability(get_item_id(block, i))) {
            ++stat->empty_item_ids;
        }
    }
    stat->available_space = block->tail_ptr - block->head_ptr
        + stat->empty_item_ids * sizeof(ItemID);
}

void accumulate_stat_info(block_stat_t* stat, const block_stat_t* stat2) {
    stat->empty_item_ids += stat2->empty_item_ids;
    stat->total_item_ids += stat2->total_item_ids;
    stat->available_space += stat2->available_space;
}

//void print_stat_info(const block_stat_t* stat) {
//    printf("==========STAT==========\n");
//    printf("empty_item_ids: " FORMAT_SIZE_T "\n", stat->empty_item_ids);
//    printf("total_item_ids: " FORMAT_SIZE_T "\n", stat->total_item_ids);
//    printf("available_space: " FORMAT_SIZE_T "\n", stat->available_space);
//    printf("========================\n");
//}