add_executable(ex2 src/main.cpp)
add_executable(ex2_omp src/main.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(ex2_omp OpenMP::OpenMP_CXX)
target_compile_definitions(ex2_omp PUBLIC USE_OMP)

set_target_properties(ex2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(ex2_omp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

