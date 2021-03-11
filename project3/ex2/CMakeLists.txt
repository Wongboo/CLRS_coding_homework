add_executable(ex2 src/main.cpp)
set_target_properties(ex2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
target_compile_definitions(ex2 PUBLIC TREE_TYPE=2)