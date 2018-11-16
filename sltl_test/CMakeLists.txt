project(sltl_test)

set(SRC src/block_test.cpp
        src/call_test.cpp
        src/comparison_test.cpp
        src/elide_test.cpp
        src/if_test.cpp
        src/intrinsic_test.cpp
        src/io_block_test.cpp
        src/io_test.cpp
        src/matrix_test.cpp
        src/scalar_test.cpp
        src/scoped_singleton_test.cpp
        src/semantic_test.cpp
        src/shader_test.cpp
        src/swizzle_test.cpp
        src/vector_test.cpp)

add_executable(sltl_test ${SRC})

add_subdirectory(ext/googletest)

target_link_libraries(sltl_test sltl_lib gtest_main)
