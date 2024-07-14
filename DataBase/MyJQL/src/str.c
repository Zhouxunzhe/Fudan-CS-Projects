#include"str.h"

#include "table.h"

#include<string.h>

#include<stdlib.h>

void read_string(Table* table, RID rid, StringRecord* record) {
    StringChunk* chunk = &record->chunk;
    table_read(table, rid, (ItemPtr)chunk);
    record->chunk = *chunk;
    record->idx = 0;
    return;
}

int has_next_char(StringRecord* record) {
    StringRecord rec = *record;
    StringChunk* chunk = &rec.chunk;
    RID rid;
    short size = get_str_chunk_size(chunk) - sizeof(RID) - sizeof(short);
    if (rec.idx == size - 1) {
        rid = get_str_chunk_rid(chunk);
        if (get_rid_block_addr(rid) == -1)
            return 0;
    }
    return 1;
}

char next_char(Table* table, StringRecord* record) {
    StringChunk* chunk = &record->chunk;
    char* str = get_str_chunk_data_ptr(chunk);
    short idx = record->idx;
    short size = get_str_chunk_size(chunk) - sizeof(RID) - sizeof(short);

    if (idx == size - 1) {
        record->idx = 0;
        RID next_rid = get_str_chunk_rid(chunk);
        off_t rid_addr = get_rid_block_addr(next_rid);
        short rid_idx = get_rid_idx(next_rid);
        if (rid_addr == -1) //can not find more str
            return 0;
        Block* block = (Block*)get_page(&table->data_pool, rid_addr);
        release(&table->data_pool, rid_addr);
        StringChunk* next_chunk = (StringChunk*)get_item(block, rid_idx);
        record->chunk = *next_chunk;
        char* next_str = get_str_chunk_data_ptr(next_chunk);
        return next_str[0];
    }
    else {
        record->idx++;
        return str[++idx];
    }
}

int compare_string_record(Table* table, const StringRecord* a, const StringRecord* b) {
    StringRecord rec_a = *a;
    StringRecord rec_b = *b;
    rec_a.idx = 0;
    rec_b.idx = 0;
    char* str_a = get_str_chunk_data_ptr(&rec_a.chunk);
    char* str_b = get_str_chunk_data_ptr(&rec_b.chunk);
    if (str_a[0] > str_b[0]) {
        return 1;
    }
    else if(str_a[0] < str_b[0]) {
        return -1;
    }
    while (1) {
        if (has_next_char(&rec_a) == 1 && has_next_char(&rec_b) == 1) {
            str_a[0] = next_char(table, &rec_a);
            str_b[0] = next_char(table, &rec_b);
            if(str_a[0]==NULL&&str_b[0]!=NULL)
                return -1;
            if(str_a[0]!=NULL&&str_b[0]==NULL)
                return 1;
            if(str_a[0]==NULL&&str_b[0]==NULL)
                return 0;
            if (str_a[0] > str_b[0]) {
                return 1;
            }
            else if (str_a[0] < str_b[0]) {
                return -1;
            }
        }
        else if (has_next_char(&rec_a) == 0 || has_next_char(&rec_b) == 0) {
            if (has_next_char(&rec_a))
                return 1;
            else if (has_next_char(&rec_b))
                return -1;
            else
                return 0;
        }
    }
}

RID write_string(Table* table, const char* data, off_t size) {
    short max_str_size = STR_CHUNK_MAX_SIZE - sizeof(RID) - sizeof(short);
    RID rid;
    //split string
    short cnt = size / max_str_size;
    short endsize = size % max_str_size;
    if (size % 20 == 0) {
        endsize = 20;
        cnt = cnt - 1;
    }
    //initial chunk
    StringChunk* chunk = (StringChunk*)malloc(sizeof(StringChunk));

    get_rid_block_addr(get_str_chunk_rid(chunk)) = -1;
    get_rid_idx(get_str_chunk_rid(chunk)) = 0;
    get_str_chunk_size(chunk) = calc_str_chunk_size(endsize);
    /*memcpy(chunk->data + sizeof(RID) + sizeof(short), data + cnt * max_str_size, max_str_size);*/
    for (int i = 0; i < endsize; i++) {
        chunk->data[sizeof(RID) + sizeof(short) + i] = data[cnt * max_str_size + i];
    }
    rid = table_insert(table, (ItemPtr)chunk->data, endsize + sizeof(RID) + sizeof(short));

    for (int i = cnt - 1; i > 0; i--) {
        get_rid_block_addr(get_str_chunk_rid(chunk)) = get_rid_block_addr(rid);
        get_rid_idx(get_str_chunk_rid(chunk)) = get_rid_idx(rid);
        get_str_chunk_size(chunk) = calc_str_chunk_size(max_str_size);
        /*memcpy(chunk->data + sizeof(RID) + sizeof(short), data + i * max_str_size, max_str_size);*/
        for (int j = 0; j < max_str_size; j++) {
            chunk->data[sizeof(RID) + sizeof(short) + j] = data[i * max_str_size + j];
        }
        rid = table_insert(table, chunk->data, STR_CHUNK_MAX_SIZE);
    }
    if (size > max_str_size) {
        get_rid_block_addr(get_str_chunk_rid(chunk)) = get_rid_block_addr(rid);
        get_rid_idx(get_str_chunk_rid(chunk)) = get_rid_idx(rid);
        get_str_chunk_size(chunk) = calc_str_chunk_size(max_str_size);
        /*memcpy(chunk->data + sizeof(RID) + sizeof(short), data, max_str_size);*/
        for (int i = 0; i < max_str_size; i++) {
            chunk->data[sizeof(RID) + sizeof(short) + i] = data[i];
        }
        rid = table_insert(table, chunk->data, STR_CHUNK_MAX_SIZE);
    }
    return rid;
}

void delete_string(Table* table, RID rid) {
    RID next_rid, current_rid;
    off_t addr = get_rid_block_addr(rid);
    short idx = get_rid_idx(rid);
    Block* block = (Block*)get_page(&table->data_pool, addr);
    StringChunk* chunk = (StringChunk*)get_item(block, idx);
    release(&table->data_pool, addr);
    next_rid = get_str_chunk_rid(chunk);
    addr = get_rid_block_addr(next_rid);
    idx = get_rid_idx(next_rid);
    current_rid = next_rid;
    table_delete(table, rid);
    while (addr != -1) {
        block = (Block*)get_page(&table->data_pool, addr);
        chunk = (StringChunk*)get_item(block, idx);
        next_rid = get_str_chunk_rid(chunk);
        if (addr != -1)
            table_delete(table, current_rid);
        addr = get_rid_block_addr(next_rid);
        idx = get_rid_idx(next_rid);
        current_rid = next_rid;
        release(&table->data_pool, addr);
    }
    return;
}

void print_string(Table* table, const StringRecord* record) {
    StringRecord rec = *record;
    printf("\"");
    while (has_next_char(&rec)) {
        printf("%c", next_char(table, &rec));
    }
    printf("\"");
}

size_t load_string(Table* table, const StringRecord* record, char* dest, size_t max_size) {
    StringRecord rec = *record;
    rec.idx = 0;
    char* tmp = get_str_chunk_data_ptr(&rec.chunk);
    dest[0] = tmp[0];
    size_t size = 1;
    while (size < max_size) {
        if (has_next_char(&rec)) {
            dest[size] = next_char(table, &rec);
            size++;
        }
        else {
            return size;
        }
    }
}

void chunk_printer(ItemPtr item, short item_size) {
    if (item == NULL) {
        printf("NULL");
        return;
    }
    StringChunk* chunk = (StringChunk*)item;
    short size = get_str_chunk_size(chunk), i;
    printf("StringChunk(");
    print_rid(get_str_chunk_rid(chunk));
    printf(", %d, \"", size);
    for (i = 0; i < size; i++) {
        printf("%c", get_str_chunk_data_ptr(chunk)[i]);
    }
    printf("\")");
}