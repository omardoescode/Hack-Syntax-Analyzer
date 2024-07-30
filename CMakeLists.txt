cmake_minimum_required(VERSION 3.8)
project(analyzer)

set(SOURCES tokenizer.cpp)
set(HEADERS tokenizer.h enums.h)
add_executable(analyzer main.cpp ${HEADERS} ${SOURCES})
