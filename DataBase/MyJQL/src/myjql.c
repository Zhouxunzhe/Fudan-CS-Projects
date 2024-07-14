#include "myjql.h"

#include "buffer_pool.h"
#include "b_tree.h"
#include "table.h"
#include "str.h"

#include<stdlib.h>

typedef struct {
    RID key;
    RID value;
} Record;

void read_record(Table* table, RID rid, Record* record) {
    table_read(table, rid, (ItemPtr)record);
}

RID write_record(Table* table, const Record* record) {
    return table_insert(table, (ItemPtr)record, sizeof(Record));
}

void delete_record(Table* table, RID rid) {
    table_delete(table, rid);
}

BufferPool bp_idx;  // b_tree
Table tbl_rec;      // record_table 
Table tbl_str;      // string_table

void myjql_init() {
    b_tree_init("rec.idx", &bp_idx);
    table_init(&tbl_rec, "rec.data", "rec.fsm");
    table_init(&tbl_str, "str.data", "str.fsm");
}

void myjql_close() {
    /* validate_buffer_pool(&bp_idx);
    validate_buffer_pool(&tbl_rec.data_pool);
    validate_buffer_pool(&tbl_rec.fsm_pool);
    validate_buffer_pool(&tbl_str.data_pool);
    validate_buffer_pool(&tbl_str.fsm_pool); */
    b_tree_close(&bp_idx);
    table_close(&tbl_rec);
    table_close(&tbl_str);
}

int row_row_cmp(RID rid1, RID rid2) {
    Record* rec1 = (Record*)malloc(sizeof(Record)), * rec2 = (Record*)malloc(sizeof(Record));
    StringRecord* key_rec1 = (StringRecord*)malloc(sizeof(StringRecord)), * key_rec2 = (StringRecord*)malloc(sizeof(StringRecord));
    int cmp = 2;
    
    if (get_rid_block_addr(rid1) == -1 && get_rid_idx(rid1) == 0)
        return;

    if (get_rid_block_addr(rid2) == -1 && get_rid_idx(rid2) == 0)
        return;

    read_record(&tbl_rec, rid1, rec1);
    read_record(&tbl_rec, rid2, rec2);

    if (rec1 != NULL && rec2 != NULL) {
        read_string(&tbl_str, rec1->key, key_rec1);
        read_string(&tbl_str, rec2->key, key_rec2);

        cmp = compare_string_record(&tbl_str, key_rec1, key_rec2);
    }
    if (rec1)
        free(rec1);
    if (rec2)
        free(rec2);
    if (key_rec1)
        free(key_rec1);
    if (key_rec2)
        free(key_rec2);
    if (cmp != 2)
        return cmp;
    else return;
}
int ptr_row_cmp(char* key, size_t size, RID rid) {
    Record* rec = (Record*)malloc(sizeof(Record));
    StringRecord* str_rec = (StringRecord*)malloc(sizeof(StringRecord));

    if (get_rid_block_addr(rid) == -1 && get_rid_idx(rid) == 0) {
        return;
    }
    read_record(&tbl_rec, rid, rec);
    read_string(&tbl_str, rec->key, str_rec);

    StringRecord str_rec_cmp = *str_rec;
    str_rec_cmp.idx = 0;
    char* str_cmp = get_str_chunk_data_ptr(&str_rec_cmp.chunk);
    if (key[0] > str_cmp[0]) {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return 1;
    }
    else if (key[0] < str_cmp[0]) {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return -1;
    }
    size_t tmp_size = 1;
    while (tmp_size < size) {
        if (has_next_char(&str_rec_cmp) == 1 && key[tmp_size] != 0) {
            str_cmp[0] = next_char(&tbl_str, &str_rec_cmp);
            if (key[tmp_size] > str_cmp[0]) {
                if (rec)
                    free(rec);
                if (str_rec)
                    free(str_rec);
                return 1;
            }
            else if (key[tmp_size] < str_cmp[0]) {
                if (rec)
                    free(rec);
                if (str_rec)
                    free(str_rec);
                return -1;
            }
        }
        else if (key[tmp_size] == 0 || has_next_char(&str_rec_cmp) == 0) {
            if (key[tmp_size] != 0 && tmp_size != size - 1) {
                if (rec)
                    free(rec);
                if (str_rec)
                    free(str_rec);
                return 1;
            }
            /* else if (has_next_char(&str_rec_cmp))
                return -1;
            else
                return 0; */
        }
        tmp_size++;
    }
    if (has_next_char(&str_rec_cmp)) {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return -1;
    }
    else {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return 0;
    }
}
RID insert_handler(RID rid) {}
void delete_handler(RID rid) {}

size_t myjql_get(const char* key, size_t key_len, char* value, size_t max_size) {
    Record* rec = (Record*)malloc(sizeof(Record));
    StringRecord* str_rec = (StringRecord*)malloc(sizeof(StringRecord));
    size_t size;

    RID rec_rid = b_tree_search(&bp_idx, key, key_len, ptr_row_cmp);
    if (get_rid_block_addr(rec_rid) == -1 && get_rid_idx(rec_rid) == 0) {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return -1;
    }
    read_record(&tbl_rec, rec_rid, rec);
    read_string(&tbl_str, rec->value, str_rec);
    size = load_string(&tbl_str, str_rec, value, max_size);
    if (rec)
        free(rec);
    if (str_rec)
        free(str_rec);
    return size;
}

void myjql_set(const char* key, size_t key_len, const char* value, size_t value_len) {
    Record* rec = (Record*)malloc(sizeof(Record));
    StringRecord* str_rec = (StringRecord*)malloc(sizeof(StringRecord));//for test
    RID key_rid, val_rid, rec_rid;

    rec_rid = b_tree_search(&bp_idx, key, key_len, ptr_row_cmp);

    if (get_rid_block_addr(rec_rid) == -1 && get_rid_idx(rec_rid) == 0) {//insert   
        //write string
        key_rid = write_string(&tbl_str, key, key_len);
        val_rid = write_string(&tbl_str, value, value_len);
        //write record
        rec->key = key_rid;
        rec->value = val_rid;
        rec_rid = write_record(&tbl_rec, rec);
        //write b_tree
        b_tree_insert(&bp_idx, rec_rid, row_row_cmp, insert_handler);

        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return;
    }
    else {//modify
        //delete old data
        // b_tree delete
        b_tree_delete(&bp_idx, rec_rid, row_row_cmp, insert_handler, delete_handler);
        // string delete
        read_record(&tbl_rec, rec_rid, rec);
        delete_string(&tbl_str, rec->key);
        delete_string(&tbl_str, rec->value);
        // record delete
        delete_record(&tbl_rec, rec_rid);

        //write new data
        //write string
        key_rid = write_string(&tbl_str, key, key_len);
        val_rid = write_string(&tbl_str, value, value_len);
        //write record
        Record* newrec = (Record*)malloc(sizeof(Record));
        newrec->key = key_rid;
        newrec->value = val_rid;
        RID newrec_rid = write_record(&tbl_rec, newrec);
        //write b_tree
        b_tree_insert(&bp_idx, newrec_rid, row_row_cmp, insert_handler);
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        if (newrec)
            free(newrec);
        return;
    }
}

void myjql_del(const char* key, size_t key_len) {
    Record* rec = (Record*)malloc(sizeof(Record));
    StringRecord* str_rec = (StringRecord*)malloc(sizeof(StringRecord));

    RID rec_rid = b_tree_search(&bp_idx, key, key_len, ptr_row_cmp);
    if (get_rid_block_addr(rec_rid) == -1 && get_rid_idx(rec_rid) == 0) {
        if (rec)
            free(rec);
        if (str_rec)
            free(str_rec);
        return;
    }
    // b_tree delete
    b_tree_delete(&bp_idx, rec_rid, row_row_cmp, insert_handler, delete_handler);
    // string delete
    read_record(&tbl_rec, rec_rid, rec);
    delete_string(&tbl_str, rec->key);
    delete_string(&tbl_str, rec->value);
    // record delete
    delete_record(&tbl_rec, rec_rid);
    if (rec)
        free(rec);
    if (str_rec)
        free(str_rec);
}

void myjql_analyze() {
    printf("Record Table:\n");
    analyze_table(&tbl_rec);
    printf("String Table:\n");
    analyze_table(&tbl_str);
}