cmake_minimum_required(VERSION 3.0)
project(cadexchanger_interview VERSION 0.2 LANGUAGES CXX)


add_library(mycadlib SHARED mycadlib.cpp)
set_target_properties(mycadlib PROPERTIES CXX_STANDARD 20)
target_include_directories(mycadlib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_compile_options(mycadlib PUBLIC
        -O0
        -g3
        -ggdb
        -Werror
        )

add_executable(mcl_test application.cc)
set_target_properties(mcl_test PROPERTIES CXX_STANDARD 20)
target_link_libraries(mcl_test PUBLIC
        mycadlib
        )
target_compile_options(mcl_test PUBLIC
        -O0
        -g3
        -ggdb
        -Werror
        )
