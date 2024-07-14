//#include "buffer_pool.h"
//#include "hash_map.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//
//#ifdef _WIN32
//typedef long long my_off_t;
//#else
//typedef long my_off_t;
//#endif
//void init(size_t size);
//void add(short size, my_off_t id);
//void erase(short size, my_off_t id);
//int contain(short size, my_off_t id);
//int empty(short size);
//short get_size(my_off_t id);
//size_t get_total();
//my_off_t _get_item(int idx);
//
//int test(int num_rep, int num_pos,
//    int num_ins1, int num_pop_lb1,
//    int num_ins2, int num_pop, int num_pop_lb2, int out)
//{
//    BufferPool pool;
//    hash_table_init("zztest-hashmap", &pool, 8);
//
//    int max_val = 8 * HASH_MAP_DIR_BLOCK_SIZE;
//    int counter = 1;
//    int rep;
//    int i, p, r, a;
//    int* pos = (int*)malloc(num_pos * sizeof(int));
//
//    int flag = 0;
//
//    init(max_val);
//
//    printf("max_val = %d\n", max_val);
//
//    for (rep = 0; rep < num_rep && !flag; ++rep) {
//        /* step 1: select a number of positions to insert a number of addrs */
//        for (i = 0; i < num_pos; ++i) {
//            pos[i] = rand() % max_val;
//        }
//        for (i = 0; i < num_ins1; ++i) {
//            p = rand() % num_pos;
//            hash_table_insert(&pool, (short)pos[p], counter);
//            add((short)pos[p], counter);
//            if (out) printf("insert: %d %d\n", pos[p], counter);
//            ++counter;
//        }
//        /* step 2: pop lower bound */
//        for (i = 0; i < num_pop_lb1; ++i) {
//            p = rand() % max_val;
//            r = (int)hash_table_pop_lower_bound(&pool, (short)p);
//            if (out) {
//                if (r != -1) {
//                    printf("pop lower bound: %d %d (size: %d)\n", p, r, get_size(r));
//                }
//                else {
//                    printf("pop lower bound: %d %d\n", p, r);
//                }
//            }
//            a = p;
//            while (a < max_val) {
//                if (!empty((short)a)) {
//                    break;
//                }
//                ++a;
//            }
//            if (out) printf("answer size: %d\n", a);
//            if (a == max_val) {  /* a == -1 */
//                if (r != -1) {
//                    printf("* expected: -1\n");
//                    flag = 1;
//                    break;  /* for */
//                }  /* else r == -1, OK */
//            }
//            else {  /* a != -1 */
//                if (r == -1) {
//                    printf("* expected size: %d\n", a);
//                    flag = 1;
//                    break;  /* for */
//                }
//                /* r != -1 */
//                if (!contain((short)a, r)) {
//                    printf("* expected size: %d\n", a);
//                    flag = 1;
//                    break;  /* for */
//                }
//                /* contain((short)a, r), OK */
//                erase(a, r);
//            }
//        }
//        if (flag) {
//            break;  /* for */
//        }
//        /* step 3: select a number of positions to insert a number of addrs */
//        for (i = 0; i < num_pos; ++i) {
//            pos[i] = rand() % max_val;
//        }
//        for (i = 0; i < num_ins2; ++i) {
//            p = rand() % num_pos;
//            hash_table_insert(&pool, (short)pos[p], counter);
//            add((short)pos[p], counter);
//            if (out) printf("insert: %d %d\n", pos[p], counter);
//            ++counter;
//        }
//        /* step 4: pop */
//        for (i = 0; i < num_pop; ++i) {
//            if (get_total() == 0) {
//                break;
//            }
//            r = (int)_get_item(rand() % (int)get_total());
//            if (out) printf("pop: %d (size: %d)\n", r, get_size(r));
//            hash_table_pop(&pool, get_size(r), r);
//            erase(get_size(r), r);
//        }
//        /* step 5: pop lower bound */
//        for (i = 0; i < num_pop_lb2; ++i) {
//            p = rand() % max_val;
//            r = (int)hash_table_pop_lower_bound(&pool, (short)p);
//            if (out) {
//                if (r != -1) {
//                    printf("pop lower bound: %d %d (size: %d)\n", p, r, get_size(r));
//                }
//                else {
//                    printf("pop lower bound: %d %d\n", p, r);
//                }
//            }
//            a = p;
//            while (a < max_val) {
//                if (!empty((short)a)) {
//                    break;
//                }
//                ++a;
//            }
//            if (out) printf("answer size: %d\n", a);
//            if (a == max_val) {  /* a == -1 */
//                if (r != -1) {
//                    printf("* expected: -1\n");
//                    flag = 1;
//                    break;  /* for */
//                }  /* else r == -1, OK */
//            }
//            else {  /* a != -1 */
//                if (r == -1) {
//                    printf("* expected size: %d\n", a);
//                    flag = 1;
//                    break;  /* for */
//                }
//                /* r != -1 */
//                if (!contain((short)a, r)) {
//                    printf("* expected size: %d\n", a);
//                    flag = 1;
//                    break;  /* for */
//                }
//                /* contain((short)a, r), OK */
//                erase(a, r);
//            }
//        }
//    }
//
//    free(pos);
//
//    /* validation & cleanup */
//    /* validate_buffer_pool(&pool); */
//    hash_table_close(&pool);
//    if (remove("zztest-hashmap") != 0) {
//        printf("error deleting: zztest-hashmap\n");
//    }
//    return flag;
//}
//int main()
//{
//    if (sizeof(HashMapDirectoryBlock) > PAGE_SIZE) {
//        printf("HashMapDirectoryBlock size is too large: " FORMAT_SIZE_T "\n", sizeof(HashMapDirectoryBlock));
//        return 1;
//    }
//    if (sizeof(HashMapBlock) > PAGE_SIZE) {
//        printf("HashMapBlock size is too large: " FORMAT_SIZE_T "\n", sizeof(HashMapBlock));
//        return 1;
//    }
//    srand(0);
//    // if (test(1, 2, 4, 4, 4, 2, 4, 1)) {
//    //     return 1;
//    // }
//    if (test(10000, 10, 40, 40, 40, 20, 20, 0)) {
//        return 1;
//    }
//    if (test(1000, 256, 512, 512, 512, 256, 256, 0)) {
//        return 1;
//    }
//    if (test(100000, 1, 1, 1, 1, 1, 1, 0)) {
//        return 1;
//    }
//    if (test(100000, 2, 2, 1, 2, 1, 1, 0)) {
//        return 1;
//    }
//    return 0;
//}