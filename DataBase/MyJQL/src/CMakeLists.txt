add_library(myjql STATIC
    file_io.c
    buffer_pool.c
    block.c
    hash_map.c
    table.c
    str.c
    b_tree.c
    myjql.c
)
target_include_directories(myjql PUBLIC ../include)