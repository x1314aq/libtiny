include_directories(${CMAKE_SOURCE_DIR}/include)

macro(declare_test  test_name)
    add_executable(${test_name}  ${test_name}.c)
    target_link_libraries(${test_name}  tiny)
    add_test(NAME      ${test_name}
             COMMAND   ${test_name})
endmacro()

declare_test(rbtree_test)
declare_test(avl_test)
declare_test(avl_map_test)
declare_test(hash_test)
declare_test(heap_test)
declare_test(sort_test)
