project(sltl_lib)

set(SRC src/element.cpp
        src/type.cpp
        src/scope.cpp
        src/variable.cpp
        src/core/semantic.cpp
        src/syntax/block.cpp
        src/syntax/block_base.cpp
        src/syntax/block_manager.cpp
        src/syntax/component_accessor.cpp
        src/syntax/elide.cpp
        src/syntax/expression_statement.cpp
        src/syntax/intrinsic_declaration.cpp
        src/syntax/io_block.cpp
        src/syntax/io_block_manager.cpp
        src/syntax/operator.cpp
        src/syntax/parameter_declaration.cpp
        src/syntax/reference.cpp
        src/syntax/variable_declaration.cpp
        src/syntax/variable_info.cpp
        src/output/language.cpp
        src/output/output.cpp
        src/output/output_introspector.cpp
        src/output/output_matrix_order.cpp
        src/output/glsl/glsl_convention.cpp
        src/output/glsl/glsl_language.cpp
        src/output/glsl/output_glsl.cpp
        src/output/glsl/output_introspector_glsl.cpp
        src/output/hlsl/output_hlsl.cpp)

set(INC src)

add_library(sltl_lib ${SRC})

target_include_directories(sltl_lib PUBLIC ${INC})
