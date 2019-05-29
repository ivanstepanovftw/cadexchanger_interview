cmake_minimum_required(VERSION 3.0)
project(cadexchanger_interview VERSION 1.0 LANGUAGES CXX)


add_library(mycadlib INTERFACE)
target_include_directories(mycadlib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
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
