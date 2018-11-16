project(sltl_cmd)

set(SRC src/sltl_cmd.cpp)

add_executable(sltl_cmd ${SRC})

target_link_libraries(sltl_cmd sltl_lib)
