cmake_minimum_required(VERSION 3.0)
project(cadexchanger_interview VERSION 1.0.1 DESCRIPTION "After-interview changes" LANGUAGES CXX)

include(properties.cmake)

add_library(mycadlib INTERFACE)
target_include_directories(mycadlib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

add_executable(mcl_test mcl_test.cc)
set_target_properties(mcl_test PROPERTIES
        CXX_STANDARD 20
        PWN_DEBUG 2
        PWN_OPTIMIZATION 3
        PWN_WARNINGS 2
        )
pwn_target_properties(mcl_test)
target_link_libraries(mcl_test PUBLIC
        mycadlib
        )
