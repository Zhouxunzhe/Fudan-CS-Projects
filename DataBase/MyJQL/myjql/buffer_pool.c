#include "buffer_pool.h"
#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>

void init_buffer_pool(const char* filename, BufferPool* pool) {
    open_file(&pool->file, filename);
    for (int i = 0; i < CACHE_PAGE; i++) {
        pool->addrs[i] = -1;
        pool->cnt[i] = 0;
        pool->ref[i] = 0;
        pool->avail[i] = 1;
        read_page(&pool->pages[i], &pool->file, PAGE_SIZE * i);
        /*pool->write[i] = 0;*/
    }
}

void close_buffer_pool(BufferPool* pool) {
    for (int i = 0; i < CACHE_PAGE; i++) {
        write_page(&pool->pages[i], &pool->file, pool->addrs[i]);
    }
    close_file(&pool->file);
}

Page* get_page(BufferPool* pool, off_t addr) {//LRU
    int target = -1;
    size_t is_full = 0;
    for (int i = 0; i < CACHE_PAGE; i++) {//not full and find
        if (pool->addrs[i] == addr) {
            pool->ref[i]++;
            pool->cnt[i] = 0;
            pool->addrs[i] = addr;
            pool->avail[i] = 0;
            target = i;
            break;
        }
        pool->cnt[i]++;
        is_full += pool->avail[i];
    }
    if (target == -1) {//full or not find : LRU
        if (is_full == 0) {
            size_t max_cnt = 0;

            for (int i = 0; i < CACHE_PAGE; i++) {
                if (pool->cnt[i] > max_cnt) {
                    max_cnt = pool->cnt[i];
                    target = i;
                }
            }
            release(pool, pool->addrs[target]);
            pool->addrs[target] = addr;
            pool->ref[target]++;
            pool->cnt[target] = 0;
            pool->avail[target] = 0;
            return &pool->pages[target];
        }
        else {
            for (int i = 0; i < CACHE_PAGE; i++) {
                if (pool->avail[i] == 1) {
                    read_page(&pool->pages[i], &pool->file, addr);
                    target = i;
                    pool->addrs[target] = addr;
                    pool->ref[target]++;
                    pool->cnt[target] = 0;
                    pool->avail[target] = 0;
                    return &pool->pages[target];
                }
            }
        }
    }
    else {
        return &pool->pages[target];
    }
}

void release(BufferPool* pool, off_t addr) {
    for (int i = 0; i < CACHE_PAGE; i++) {
        if (pool->addrs[i] == addr) {
            /*if (pool->write[i] == 1)*/
            write_page(&pool->pages[i], &pool->file, pool->addrs[i]);
            pool->addrs[i] = -1;
            pool->ref[i]++;
            pool->avail[i] = 1;
            return;
        }
    }
}

void print_buffer_pool(BufferPool* pool) {
}

void validate_buffer_pool(BufferPool* pool) {
}
