#pragma once
#ifndef _BUFFER_POOL_H
#define _BUFFER_POOL_H

#include "file_io.h"

#define CACHE_PAGE 8

typedef struct {
	/* you can modify anything in this struct */
	FileInfo file;
	Page pages[CACHE_PAGE];
	off_t addrs[CACHE_PAGE];
	size_t cnt[CACHE_PAGE];
	size_t ref[CACHE_PAGE];
	size_t avail[CACHE_PAGE];
	/*size_t write[CACHE_PAGE];*/
} BufferPool;

/* BEGIN: --------------------------------- DO NOT MODIFY! --------------------------------- */

void init_buffer_pool(const char* filename, BufferPool* pool);

void close_buffer_pool(BufferPool* pool);

Page* get_page(BufferPool* pool, off_t addr);

void release(BufferPool* pool, off_t addr);

/* END:   --------------------------------- DO NOT MODIFY! --------------------------------- */

/* void print_buffer_pool(BufferPool *pool); */

/* void validate_buffer_pool(BufferPool *pool); */

#endif  /* _BUFFER_POOL_H */