#ifndef _STR_H
#define _STR_H

#include "file_io.h"
#include "table.h"

#define STR_CHUNK_MAX_SIZE ((PAGE_SIZE) / 4)
#define STR_CHUNK_MAX_LEN (STR_CHUNK_MAX_SIZE - sizeof(RID) - sizeof(short))

typedef struct {
    /*
     * strings are split into chunks organized in singly linked list
     * RID rid: next chunk
     * short size: size of the current chunk
     * char data[]: chunk data (of variable length)
     */
    char data[STR_CHUNK_MAX_SIZE];  /* placeholder only */
} StringChunk;

#define get_str_chunk_rid(chunk) (*(RID*)(chunk))
#define get_str_chunk_size(chunk) (*(short*)(((char*)(chunk)) + sizeof(RID)))
#define get_str_chunk_data_ptr(chunk) (((char*)(chunk)) + sizeof(RID) + sizeof(short))
#define calc_str_chunk_size(len) ((len) + sizeof(RID) + sizeof(short))

typedef struct {
    /* you can ADD anything in this struct */
    StringChunk chunk;  /* current chunk */
    short idx;  /* current idx */
} StringRecord;

/* BEGIN: --------------------------------- DO NOT MODIFY! --------------------------------- */

void read_string(Table* table, RID rid, StringRecord* record);

int has_next_char(StringRecord* record);

char next_char(Table* table, StringRecord* record);

int compare_string_record(Table* table, const StringRecord* a, const StringRecord* b);

RID write_string(Table* table, const char* data, off_t size);

void delete_string(Table* table, RID rid);

 void print_string(Table *table, const StringRecord *record); 

/* load up to max_size chars to dest from record, the number of loaded chars are returned.
   note '\0' should be appended manually  */
size_t load_string(Table* table, const StringRecord* record, char* dest, size_t max_size);

 void chunk_printer(ItemPtr item, short item_size); 

/* END:   --------------------------------- DO NOT MODIFY! --------------------------------- */

#endif  /* _STR_H */