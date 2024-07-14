#ifndef _TABLE_H
#define _TABLE_H

#include "file_io.h"
#include "buffer_pool.h"
#include "block.h"

typedef struct {
    /* you can ADD anything in this struct */
    BufferPool data_pool;
    BufferPool fsm_pool;
} Table;

/* BEGIN: --------------------------------- DO NOT MODIFY! --------------------------------- */

typedef struct {
    /* off_t block_addr */
    /* short idx */
    char data[sizeof(off_t) + sizeof(short)];  /* 10 bytes */
} RID;  /* record id */

#define get_rid_block_addr(rid) (*(off_t*)(&(rid)))
#define get_rid_idx(rid) (*(short*)(((char*)(&(rid))) + sizeof(off_t)))

/* tables do not handle rows/records which are larger than one page */

void table_init(Table *table, const char *data_filename, const char *fsm_filename);

void table_close(Table *table);

off_t table_get_total_blocks(Table *table);

short table_block_get_total_items(Table *table, off_t block_addr);

void table_read(Table *table, RID rid, ItemPtr dest);

RID table_insert(Table *table, ItemPtr src, short size);

void table_delete(Table *table, RID rid);

void print_table(Table *table, printer_t printer);

void print_rid(RID rid);

/* END:   --------------------------------- DO NOT MODIFY! --------------------------------- */

void analyze_table(Table *table);

#endif  /* _TABLE_H */