add_executable(ex1 src/main.cpp)
set_target_properties(ex1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
target_compile_definitions(ex1 PUBLIC TREE_TYPE=1)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")