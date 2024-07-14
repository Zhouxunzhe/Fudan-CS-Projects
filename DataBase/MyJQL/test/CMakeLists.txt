link_libraries(myjql)

add_executable(test_hash_map
    test_hash_map.c
    test_hash_map_wrapper.cpp)

add_executable(test_str
    test_str.c
    test_str_wrapper.cpp)

add_executable(test_b_tree
    test_b_tree.c
    test_b_tree_wrapper.cpp)

enable_testing()

add_test(
   NAME test_hash_map
   COMMAND $<TARGET_FILE:test_hash_map>
)

add_test(
   NAME test_str
   COMMAND $<TARGET_FILE:test_str>
)

add_test(
    NAME test_b_tree
    COMMAND $<TARGET_FILE:test_b_tree>
)