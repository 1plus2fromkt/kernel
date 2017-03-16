cmake_minimum_required(VERSION 2.8)

project(kernel)

add_custom_command( 
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o
	COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/kernel.c -o ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra 
)

add_custom_target(
	kernel_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o
)
