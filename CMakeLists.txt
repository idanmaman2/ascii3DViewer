cmake_minimum_required(VERSION 3.23)
project(ascii3DViewer C)

set(CMAKE_C_STANDARD 99)

add_executable(ascii3DViewer main.c objReader.h vectorM.h face.h)
