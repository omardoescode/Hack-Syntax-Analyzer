cmake_minimum_required(VERSION 3.8)
project(analyzer)

set(SOURCES tokenizer.cpp hack_map.cpp)
set(HEADERS tokenizer.h enums.h error.h hack_map.h)
add_executable(analyzer main.cpp ${HEADERS} ${SOURCES})
add_executable(tester test.cpp ${HEADERS} ${SOURCES})


target_precompile_headers(analyzer PRIVATE pch.h)
target_precompile_headers(tester PRIVATE pch.h)
