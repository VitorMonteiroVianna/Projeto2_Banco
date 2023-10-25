cmake_minimum_required(VERSION 3.25.2)
project(Projeto2_banco C)

set(CMAKE_C_STANDARD 11)

add_executable(Projeto2_banco main.c
        biblioteca.c
        biblioteca.h)
