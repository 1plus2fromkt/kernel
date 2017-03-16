cmake_minimum_required(VERSION 2.8)

project(boot)

enable_language(ASM-ATT)

set(CMAKE_ASM-ATT_COMPILER i686-elf-as)

add_executable(boot boot.s)
