cmake_minimum_required(VERSION 3.26)
project(Pencil)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pencil
        main.cpp      # main lives here
        pencil.cpp
        pencil.h
)

add_executable(pencil_tests
        tests.cpp     # tests have their own main
        pencil.cpp
        pencil.h
)

#install target.
add_custom_target(pencil_install
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/pencil
        /Users/cobalt/bin/pencil
        DEPENDS pencil
)