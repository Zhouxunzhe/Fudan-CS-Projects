#define _CRT_SECURE_NO_WARNINGS

#include "file_io.h"

#include <stdio.h>

FileIOResult open_file(FileInfo *file, const char *filename) {
    if (file->fp = fopen(filename, "rb+")) {
        /* the file exists and is successfully opened */
    } else if (file->fp = fopen(filename, "wb+")) {
        /* the file does not exist and is successfully created */
    } else {
        return FILE_IO_FAILED;
    }
    if (fseek(file->fp, 0, SEEK_END)) {
        return FILE_IO_FAILED;
    }
    file->length = ftell(file->fp);
    if (file->length & PAGE_MASK) {
        close_file(file);
        return INVALID_LEN;
    }
    return FILE_IO_SUCCESS;
}

FileIOResult close_file(FileInfo *file) {
    fclose(file->fp);
    file->fp = NULL;
    file->length = 0;
    return FILE_IO_SUCCESS;
}

FileIOResult read_page(Page *page, const FileInfo *file, off_t addr) {
    if (addr & PAGE_MASK) {
        return INVALID_ADDR;
    }
    if (addr < 0 || addr >= file->length) {
        return ADDR_OUT_OF_RANGE;
    }
    if (fseek(file->fp, (long)addr, SEEK_SET)) {
        return FILE_IO_FAILED;
    }
    size_t bytes_read = fread(page, PAGE_SIZE, 1, file->fp);
    if (bytes_read != 1) {
        return FILE_IO_FAILED;
    }
    return FILE_IO_SUCCESS;
}

FileIOResult write_page(const Page *page, FileInfo *file, off_t addr) {
    if (addr & PAGE_MASK) {
        return INVALID_ADDR;
    }
    if (addr < 0 || addr > file->length) {
        return ADDR_OUT_OF_RANGE;
    }
    if (fseek(file->fp, (long)addr, SEEK_SET)) {
        return FILE_IO_FAILED;
    }
    size_t bytes_written = fwrite(page, PAGE_SIZE, 1, file->fp);
    if (bytes_written != 1) {
        return FILE_IO_FAILED;
    }
    if (addr == file->length) {  /* append new page in the end*/
        file->length += PAGE_SIZE;
    }
    return FILE_IO_SUCCESS;
}
