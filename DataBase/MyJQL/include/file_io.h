/* DO NOT MODIFY THIS FILE IN ANY WAY! */

#ifndef _FILE_IO_H
#define _FILE_IO_H

#include <stdio.h>

#define PAGE_SIZE 128
// #define PAGE_SIZE 64

#define PAGE_MASK (PAGE_SIZE - 1)

#ifdef _WIN32

/* address offset type is defined to be (signed) int64 */
typedef long long off_t;

/* typedef unsigned long long size_t; */

/* in this way, off_t and size_t are always of the same size */

/* FORMAT: printf/scanf format string */
/* off_t */
#define FORMAT_OFF_T "%lld"
/* size_t */
#define FORMAT_SIZE_T "%lld"

#else

/* off_t {aka long int} */
#define FORMAT_OFF_T "%ld"
/* size_t {aka long unsigned int} */
#define FORMAT_SIZE_T "%ld"

#endif

typedef struct {
    FILE *fp;  /* file pointer */
    off_t length;  /* file length */
} FileInfo;

typedef struct {
  char data[PAGE_SIZE];  /* placeholder only */
} Page;

const Page EMPTY_PAGE;

typedef enum {
    FILE_IO_SUCCESS,
    FILE_IO_FAILED,
    INVALID_LEN,  /* file length is not a multiple of PAGE_SIZE */
    INVALID_ADDR,  /* address is not a multiple of PAGE_SIZE */
    ADDR_OUT_OF_RANGE,
} FileIOResult;

FileIOResult open_file(FileInfo *file, const char *filename);

FileIOResult close_file(FileInfo *file);

FileIOResult read_page(Page *page, const FileInfo *file, off_t addr);

FileIOResult write_page(const Page *page, FileInfo *file, off_t addr);

#endif  /* _FILE_IO_H */