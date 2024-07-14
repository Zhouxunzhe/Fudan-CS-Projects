#include "table.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
typedef long long my_off_t;
#else
typedef long my_off_t;
#endif

typedef struct {
    my_off_t addr;
    short idx;
} my_RID;

void init();
void insert(my_RID rid, char* s);
void erase(my_RID rid);
size_t get_total();
my_RID get_rid(int idx);
int equal(my_RID rid, char* s);

#define N 1024

char buf[N + 1];

char random_char() {
    int op = rand() % 3;
    if (op == 0) {
        return 'a' + rand() % 26;
    }
    else if (op == 1) {
        return 'A' + rand() % 26;
    }
    else {
        return '0' + rand() % 10;
    }
}

int generate_string(int n) {
    int len = rand() % n;
    int i;
    for (i = 0; i < len; ++i) {
        buf[i] = random_char();
    }
    buf[len] = 0;
    return len;
}

int test(int max_str_len, int num_op, int out)
{
    int flag = 0;
    int i, op, n;
    RID rid;
    my_RID m_rid;
    StringRecord rec;

    Table table;
    table_init(&table, "zztest-str.data", "zztest-str.fsm");

    for (i = 0; i < num_op; ++i) {
        op = rand() % 3;
        if (op == 0 && get_total() != 0) {  /* read */
            m_rid = get_rid(rand() % (int)get_total());
            get_rid_block_addr(rid) = m_rid.addr;
            get_rid_idx(rid) = m_rid.idx;
            if (out) {
                printf("read: ");
                print_rid(rid);
                printf("\n");
            }
            read_string(&table, rid, &rec);
            n = (int)load_string(&table, &rec, buf, N);
            buf[n] = 0;
            if (equal(m_rid, buf)) {
                if (out) {
                    printf("OK\n");
                }
            }
            else {
                printf("* error\n");
                flag = 1;
                break;  /* for */
            }
        } if (op == 1 && get_total() != 0) {  /* delete */
            m_rid = get_rid(rand() % (int)get_total());
            get_rid_block_addr(rid) = m_rid.addr;
            get_rid_idx(rid) = m_rid.idx;
            if (out) {
                printf("delete: ");
                print_rid(rid);
                printf("\n");
            }
            delete_string(&table, rid);
            erase(m_rid);
        }
        else {  /* write */
            n = generate_string(max_str_len);
            rid = write_string(&table, buf, n);
            m_rid.addr = get_rid_block_addr(rid);
            m_rid.idx = get_rid_idx(rid);
            insert(m_rid, buf);
            if (out) {
                printf("insert: %s, len = %d @ ", buf, n);
                print_rid(rid);
                printf("\n");
            }
        }
    }

    /* validate_buffer_pool(&table.data_pool); */
    /* validate_buffer_pool(&table.fsm_pool); */
    table_close(&table);

    if (remove("zztest-str.data")) {
        printf("error deleting: zztest-str.data\n");
    }
    if (remove("zztest-str.fsm")) {
        printf("error deleting: zztest-str.fsm\n");
    }

    return flag;
}

int main()
{
    srand(0);

    // if (test(10, 30, 1)) {
    //     return 1;
    // }
    if (test(512, 10000, 1)) {
        return 1;
    }
    printf("success!");
    return 0;
}