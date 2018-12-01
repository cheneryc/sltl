project(sltl_lib)

set(SRC src/element.cpp
        src/language.cpp
        src/output.cpp
        src/scope.cpp
        src/variable.cpp
        src/core/semantic.cpp
        src/syntax/block.cpp
        src/syntax/block_base.cpp
        src/syntax/block_manager.cpp
        src/syntax/component_accessor.cpp
        src/syntax/elide.cpp
        src/syntax/expression_statement.cpp
        src/syntax/io_block.cpp
        src/syntax/io_block_manager.cpp
        src/syntax/operator.cpp
        src/syntax/parameter_declaration.cpp
        src/syntax/reference.cpp
        src/syntax/variable_declaration.cpp
        src/syntax/variable_info.cpp)

set(INC src)

add_library(sltl_lib ${SRC})

target_include_directories(sltl_lib PUBLIC ${INC})
