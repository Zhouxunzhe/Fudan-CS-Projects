#include "hash_map.h"

#include <stdio.h>

void hash_table_init(const char* filename, BufferPool* pool, off_t n_directory_blocks) {
    init_buffer_pool(filename, pool);
    /* TODO: add code here */
    if (pool->file.length == 0) {
        for (int i = 0; i < n_directory_blocks + 2; i++) {
            Page* init = get_page(pool, i * PAGE_SIZE);
            release(pool, i * PAGE_SIZE);
        }
        HashMapControlBlock* ctrl = (HashMapControlBlock*)get_page(pool, 0);
        ctrl->free_block_head = (n_directory_blocks + 1) * PAGE_SIZE;
        ctrl->n_directory_blocks = n_directory_blocks;
        ctrl->max_size = n_directory_blocks * HASH_MAP_DIR_BLOCK_SIZE;//0~127,maxsize=128
        ctrl->n_block = 2 + n_directory_blocks; //ctrl+dir+block

        for (int i = 1; i <= n_directory_blocks; i++) {
            HashMapDirectoryBlock* dir_block = (HashMapDirectoryBlock*)get_page(pool, i * PAGE_SIZE);
            for (int j = 0; j < HASH_MAP_DIR_BLOCK_SIZE; j++) {
                dir_block->directory[j] = 0;
            }
            release(pool, i * PAGE_SIZE);
        }

        //HashMapBlock initialed
        off_t free_addr = ctrl->free_block_head;
        HashMapBlock* block = (HashMapBlock*)get_page(pool, free_addr);
        block->next = 0;
        block->n_items = 0;
        for (int i = 0; i < HASH_MAP_BLOCK_SIZE; i++) {
            block->table[i] = -1;
        }
        release(pool, free_addr);
        release(pool, 0);
    }
    return;
}

void hash_table_close(BufferPool* pool) {
    close_buffer_pool(pool);
}

void hash_table_insert(BufferPool* pool, short size, off_t addr) {
    HashMapControlBlock* ctrl = (HashMapControlBlock*)get_page(pool, 0);
    if (size < ctrl->max_size) {
        short dir_id = size / HASH_MAP_DIR_BLOCK_SIZE; //id of dir_block
        short block_id = size % HASH_MAP_DIR_BLOCK_SIZE; //id in dir_block
        HashMapDirectoryBlock* target_dir = (HashMapDirectoryBlock*)get_page(pool, (dir_id + 1) * PAGE_SIZE);
        HashMapBlock* target_block;
        HashMapBlock* free_block;
        HashMapBlock* target_block_next;
        if (target_dir->directory[block_id] != 0) {
            off_t target_block_addr = target_dir->directory[block_id];

            short is_duplicate = 0;
            target_block = (HashMapBlock*)get_page(pool, target_block_addr);
            while (target_block->n_items >= HASH_MAP_BLOCK_SIZE
                && target_block->next != 0) {
                for (int i = 0; i < HASH_MAP_BLOCK_SIZE; i++) {
                    if (addr == target_block->table[i]) {//duplicate addr
                        is_duplicate = 1;
                    }
                }
                release(pool, target_block_addr);
                target_block_addr = target_block->next;
                target_block = (HashMapBlock*)get_page(pool, target_block_addr);
            }
            if (is_duplicate == 1) {
                release(pool, target_block_addr);
                release(pool, (dir_id + 1) * PAGE_SIZE);
                release(pool, 0);
                return;
            }
            if (target_block->n_items < HASH_MAP_BLOCK_SIZE) {
                for (int i = 0; i < HASH_MAP_BLOCK_SIZE; i++) {
                    if (target_block->table[i] == -1) {
                        target_block->table[i] = addr;
                        target_block->n_items++;
                        break;
                    }
                }
            }
            else {//all full
                off_t free_addr = ctrl->free_block_head;
                if (free_addr != 0) {//find first free_block
                    free_block = (HashMapBlock*)get_page(pool, free_addr);
                    ctrl->free_block_head = free_block->next;
                    target_block->next = free_addr;
                    free_block->n_items++;
                    free_block->next = 0;
                    free_block->table[0] = addr;
                    release(pool, free_addr);
                }
                else {//new block
                    off_t hash_end = ctrl->n_block * PAGE_SIZE;
                    target_block->next = hash_end;
                    target_block_next = (HashMapBlock*)get_page(pool, hash_end);
                    ctrl->n_block++;
                    target_block_next->n_items = 1;
                    target_block_next->next = 0;
                    for (int i = 0; i < HASH_MAP_BLOCK_SIZE; i++) {
                        target_block_next->table[i] = -1;
                    }
                    target_block_next->table[0] = addr;
                    release(pool, hash_end);
                }
            }
            release(pool, target_block_addr);
        }
        else {//block no exist
            off_t free_addr = ctrl->free_block_head;
            if (free_addr != 0) {//find first free_block
                free_block = (HashMapBlock*)get_page(pool, free_addr);
                target_dir->directory[block_id] = free_addr;
                ctrl->free_block_head = free_block->next;
                free_block->n_items++;
                free_block->next = 0;
                free_block->table[0] = addr;
                release(pool, free_addr);
            }
            else {//new block
                off_t hash_end = ctrl->n_block * PAGE_SIZE;
                target_dir->directory[block_id] = hash_end;
                target_block = (HashMapBlock*)get_page(pool, hash_end);
                ctrl->n_block++;
                target_block->n_items = 1;
                target_block->next = 0;
                for (int i = 0; i < HASH_MAP_BLOCK_SIZE; i++) {
                    target_block->table[i] = -1;
                }
                target_block->table[0] = addr;
                release(pool, hash_end);
            }
        }
        release(pool, (dir_id + 1) * PAGE_SIZE);
    }
    release(pool, 0);
    return;
}

off_t hash_table_pop_lower_bound(BufferPool* pool, short size) {
    HashMapControlBlock* ctrl = (HashMapControlBlock*)get_page(pool, 0);
    short tmp_size = size;
    while (tmp_size < ctrl->max_size) {
        short dir_id = tmp_size / HASH_MAP_DIR_BLOCK_SIZE; //id of dir_block
        short block_id = tmp_size % HASH_MAP_DIR_BLOCK_SIZE; //id in dir_block
        HashMapDirectoryBlock* target_dir = (HashMapDirectoryBlock*)get_page(pool, (dir_id + 1) * PAGE_SIZE);
        if (target_dir->directory[block_id] != 0) {
            off_t block_addr = target_dir->directory[block_id];
            off_t addr; off_t next_addr;
            HashMapBlock* block = get_page(pool, block_addr);
            release(pool, block_addr);
            while (block_addr != 0) {
                block = (HashMapBlock*)get_page(pool, block_addr);
                next_addr = block->next;
                release(pool, block_addr);
                block_addr = next_addr;
            }
            if (block->n_items == HASH_MAP_BLOCK_SIZE) {//block full
                addr = block->table[HASH_MAP_BLOCK_SIZE - 1];
                hash_table_pop(pool, tmp_size, addr);
                release(pool, (dir_id + 1) * PAGE_SIZE);
                release(pool, 0);
                return addr;
            }
            else {
                for (int i = 0; i < block->n_items + 1; i++) {
                    if (block->table[i] == -1) {
                        addr = block->table[i - 1];
                        hash_table_pop(pool, tmp_size, addr);
                        release(pool, (dir_id + 1) * PAGE_SIZE);
                        release(pool, 0);
                        return addr;
                    }
                }
            }
        }
        release(pool, (dir_id + 1) * PAGE_SIZE);
        tmp_size++;
    }
    release(pool, 0);
    return -1;
}

void hash_table_pop(BufferPool* pool, short size, off_t addr) {
    HashMapControlBlock* ctrl = (HashMapControlBlock*)get_page(pool, 0);
    HashMapDirectoryBlock* dir_block;
    off_t block_addr, next_addr, last_addr;
    HashMapBlock* block;
    int j;
    dir_block = (HashMapDirectoryBlock*)get_page(pool, (size / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
    if (dir_block->directory[size % HASH_MAP_DIR_BLOCK_SIZE] != 0) {//make sure dir not null
        block_addr = dir_block->directory[size % HASH_MAP_DIR_BLOCK_SIZE];
        block = (HashMapBlock*)get_page(pool, block_addr);
        if (block->n_items == 1) {//size = 1 and directly dir
            block->table[0] = -1;
            block->n_items--;
            //delete from block
            dir_block->directory[size % HASH_MAP_DIR_BLOCK_SIZE] = 0;
            //insert into free_block
            off_t free_block_head = ctrl->free_block_head;
            block->next = free_block_head;
            ctrl->free_block_head = block_addr;
            release(pool, block_addr);
            release(pool, (size / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
            release(pool, 0);
            return;
        }
        else {//size > 1 or not directly dir
            do {
                block = (HashMapBlock*)get_page(pool, block_addr);
                next_addr = block->next;
                for (j = 0; j < block->n_items; ++j) {
                    if (block->table[j] == addr) {
                        block->table[j] = -1;
                        if (j != block->n_items - 1) {
                            block->table[j] = block->table[block->n_items - 1];
                            block->table[block->n_items - 1] = -1;
                        }
                        block->n_items--;
                        if (block->n_items == 0) {//get free block
                            //delete from block
                            HashMapBlock* last_block = (HashMapBlock*)get_page(pool, last_addr);
                            last_block->next = next_addr;
                            //insert into free_block
                            off_t free_block_head = ctrl->free_block_head;
                            block->next = free_block_head;
                            ctrl->free_block_head = block_addr;
                            release(pool, last_addr);
                        }
                        release(pool, block_addr);
                        release(pool, (size / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
                        release(pool, 0);
                        return;
                    }
                }
                release(pool, block_addr);
                last_addr = block_addr;
                block_addr = next_addr;
            } while (block->next != 0);
        }
        release(pool, block_addr);
    }
    //not find target
    release(pool, (size / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
    release(pool, 0);
    return;
}

void print_hash_table(BufferPool* pool) {
    HashMapControlBlock* ctrl = (HashMapControlBlock*)get_page(pool, 0);
    HashMapDirectoryBlock* dir_block;
    off_t block_addr, next_addr;
    HashMapBlock* block;
    int i, j;
    printf("----------HASH TABLE----------\n");
    for (i = 0; i < ctrl->max_size; ++i) {
        dir_block = (HashMapDirectoryBlock*)get_page(pool, (i / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
        if (dir_block->directory[i % HASH_MAP_DIR_BLOCK_SIZE] != 0) {
            printf("%d:", i);
            block_addr = dir_block->directory[i % HASH_MAP_DIR_BLOCK_SIZE];
            while (block_addr != 0) {
                block = (HashMapBlock*)get_page(pool, block_addr);
                printf("  [" FORMAT_OFF_T "]", block_addr);
                printf("{");
                for (j = 0; j < HASH_MAP_BLOCK_SIZE; ++j) {
                    if (j != 0) {
                        printf(", ");
                    }
                    printf(FORMAT_OFF_T, block->table[j]);
                }
                printf("}");
                next_addr = block->next;
                release(pool, block_addr);
                block_addr = next_addr;
            }
            printf("\n");
        }
        release(pool, (i / HASH_MAP_DIR_BLOCK_SIZE + 1) * PAGE_SIZE);
    }
    release(pool, 0);
    printf("------------------------------\n");
}