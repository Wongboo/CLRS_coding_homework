add_executable(ex2 src/main.cpp)
add_executable(ex2_fast src/main.cpp)
add_executable(ex2_map src/main.cpp)
target_compile_definitions(ex2_map PUBLIC USE_SET_IMPLEMENT)
set_target_properties(ex2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(ex2_map PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(ex2_fast PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#using openmp
if (MSVC)
    #fix some issue by using experimental
    target_compile_options(ex2_fast PUBLIC -openmp:experimental)
else ()
    find_package(OpenMP REQUIRED)
    if(${OpenMP_FOUND})
        target_link_libraries(ex2_fast PUBLIC OpenMP::OpenMP_CXX)
    else()
        message("without openmp, ex2_fast is useless!")
    endif()
endif ()

target_compile_definitions(ex2_fast PUBLIC USE_OMP)
