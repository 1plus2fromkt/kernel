cmake_minimum_required(VERSION 2.8)

project(memory)

function(compile_i686 name)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o
		COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c -o ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h
	)
endfunction(compile_i686)

compile_i686("memory")

add_custom_target(
	libc_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/memory.o
)
