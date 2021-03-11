add_executable(reshuffle reshuffle.cpp)
add_executable(sort_no_insert sort.cpp)
add_executable(sort_omp sort.cpp)
add_executable(sort sort.cpp)
#this five is for debug
#[[
add_executable(insert_sort sort.cpp)
add_executable(merge_sort sort.cpp)
add_executable(quick_sort sort.cpp)
add_executable(count_sort sort.cpp)
add_executable(heap_sort sort.cpp)
]]

target_compile_definitions(sort_no_insert PRIVATE USE_MERGE USE_QUICK USE_COUNT USE_HEAP)
target_compile_definitions(sort_omp PRIVATE USE_MERGE USE_QUICK USE_COUNT USE_HEAP USE_OMP)
target_compile_definitions(sort PRIVATE USE_INSERT USE_MERGE USE_QUICK USE_COUNT USE_HEAP)
#[[
target_compile_definitions(insert_sort PRIVATE USE_INSERT)
target_compile_definitions(merge_sort PRIVATE USE_MERGE)
target_compile_definitions(quick_sort PRIVATE USE_QUICK)
target_compile_definitions(count_sort PRIVATE USE_COUNT)
target_compile_definitions(heap_sort PRIVATE USE_HEAP)
]]

#using openmp
if (MSVC)
    #fix some issue by using experimental
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp:experimental")
else ()
    find_package(OpenMP REQUIRED)
    target_link_libraries(sort_omp PUBLIC OpenMP::OpenMP_CXX)
endif ()