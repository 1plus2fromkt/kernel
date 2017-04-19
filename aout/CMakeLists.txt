cmake_minimum_required(VERSION 2.8)

project(aout)

function(compile_i686 name)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o
		COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c -o ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o  -I ${CMAKE_BINARY_DIR}/../ -std=gnu99 -ffreestanding -Wall -Wextra
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h
	)
endfunction(compile_i686)

compile_i686(loader)

add_custom_target(
	aout_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/loader.o 
)
