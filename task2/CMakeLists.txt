cmake_minimum_required(VERSION 3.16)
project(task2)

set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=thread -fno-omit-frame-pointer")

add_executable(main main.c)