project(sltl_cmd)

set(SRC src/sltl_cmd.cpp)

add_executable(exe_sltl_cmd ${SRC})

target_link_libraries(exe_sltl_cmd lib_sltl_lib)
