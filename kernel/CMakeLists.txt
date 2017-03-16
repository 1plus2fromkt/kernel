cmake_minimum_required(VERSION 2.8)

set(CC i686-elf-gcc)

set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -std=gnu99 -ffreestanding -O2 -Wall -Wextra -o kernel.o")

project(kernel)

add_library(kernel STATIC kernel.c)
