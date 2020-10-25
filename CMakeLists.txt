cmake_minimum_required(VERSION 2.6)

project(EGA_LABS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -std=c++17")
set(INCLUDE_DIR include)

function(build_lab CPP_NAME)
    set(TARGET_DIR "cpp/${CPP_NAME}.cpp")
    add_executable(${CPP_NAME} ${TARGET_DIR} "dependencies/menu/menu.cpp")
    target_include_directories(${CPP_NAME} PUBLIC ${INCLUDE_DIR} "dependencies/menu")
endfunction(build_lab)

build_lab(lab1_monte_carlo)
build_lab(lab2_dfs)
build_lab(lab3_bfs)
