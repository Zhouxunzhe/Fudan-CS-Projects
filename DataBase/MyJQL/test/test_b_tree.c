#include "b_tree.h"
#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
typedef long long my_off_t;
#else
typedef long my_off_t;
#endif

void init();
void insert(my_off_t addr, short idx);
void erase(my_off_t addr);
short get_idx(my_off_t addr);
int contain(my_off_t addr);
size_t get_total();
my_off_t get_addr(int idx);

RID get_rand_rid() {
    RID rid;
    get_rid_block_addr(rid) = rand();
    get_rid_idx(rid) = (short)rand();
    return rid;
}

int op(off_t a, off_t b) {
    if (a > b) return 1;
    else if (a < b) return -1;
    else return 0;
}

int rid_row_row_cmp(RID a, RID b) {
    return op(get_rid_block_addr(a), get_rid_block_addr(b));
}

int rid_ptr_row_cmp(void *p, size_t size, RID b) {
    off_t a = *(off_t*)p;
    return op(a, get_rid_block_addr(b));
}

RID insert_handler(RID rid) {
    RID new_rid;
    get_rid_block_addr(new_rid) = get_rid_block_addr(rid);
    get_rid_idx(new_rid) = -1;
    return new_rid;
}

void delete_handler(RID rid) {
}

int test(int num_op, int out)
{
    int flag = 0;
    int i, op;
    RID rid, rid1, rid2;
    BufferPool pool;
    b_tree_init("zztest-b-tree", &pool);

    init();

    for (i = 0; i < num_op; ++i) {
        op = rand() % 3;
        if (op == 0) {  /* insert */
            do {
                rid = get_rand_rid();
            } while (contain(get_rid_block_addr(rid)));
            if (out) {
                printf("insert: ");
                print_rid(rid);
                printf("\n");
            }
            b_tree_insert(&pool, rid, &rid_row_row_cmp, &insert_handler);
            insert(get_rid_block_addr(rid), get_rid_idx(rid));
        } else if (op == 1 && get_total() != 0) {  /* erase */
            get_rid_block_addr(rid) = get_addr(rand() % (int)get_total());
            get_rid_idx(rid) = 0;
            if (out) {
                printf("erase: ");
                print_rid(rid);
                printf("\n");
            }
            b_tree_delete(&pool, rid, &rid_row_row_cmp, &insert_handler, &delete_handler);
            erase(get_rid_block_addr(rid));
        } else {  /* find */
            if (rand() % 2 && get_total() != 0) {
                get_rid_block_addr(rid) = get_addr(rand() % (int)get_total());
            } else {
                rid = get_rand_rid();
            }
            get_rid_idx(rid) = 0;
            if (out) {
                printf("search: ");
                print_rid(rid);
                printf("\n");
            }
            rid1 = b_tree_search(&pool, &rid, sizeof(rid), &rid_ptr_row_cmp);
            if (contain(get_rid_block_addr(rid))) {
                get_rid_block_addr(rid2) = get_rid_block_addr(rid);
                get_rid_idx(rid2) = get_idx(get_rid_block_addr(rid));
            } else {
                get_rid_block_addr(rid2) = -1;
                get_rid_idx(rid2) = 0;
            }
            if (out) {
                printf("expected: ");
                print_rid(rid2);
                printf(", got: ");
                print_rid(rid1);
                printf("\n");
            }
            if (get_rid_block_addr(rid1) == get_rid_block_addr(rid2)
                    && get_rid_idx(rid1) == get_rid_idx(rid2)) {
                if (out) {
                    printf("OK\n");
                }
            } else {
                printf("* error: \n");
                printf("expected: ");
                print_rid(rid2);
                printf(", got: ");
                print_rid(rid1);
                printf("\n");
                flag = 1;
                break;  /* for */
            }
        }
    }

    /* b_tree_traverse(&pool); */

    /* validate_buffer_pool(&pool); */
    b_tree_close(&pool);

    if (remove("zztest-b-tree")) {
        printf("error deleting: zztest-b-tree\n");
    }
    return flag;
}

int main()
{
    printf("PAGE_SIZE = %d\n", PAGE_SIZE);
    printf("DEGREE = " FORMAT_SIZE_T "\n", DEGREE);
    printf("BNode size: " FORMAT_SIZE_T "\n", sizeof(BNode));
    if (DEGREE < 2) {
        printf("error: DEGREE < 2\n");
        return 1;
    }
    if (sizeof(BNode) > PAGE_SIZE) {
        printf("error: BNode size is too large\n");
        return 1;
    }
    
    /* fixed random seed */
    srand(0);
    // srand((unsigned int)time(NULL));

    // if (test(50, 1)) {
    //     return 1;
    // }
    if (test(2000000, 0)) {
        return 1;
    }

    /* prevent using exit(0) to pass the test */
    printf("END OF TEST\n");
    return 0;
}