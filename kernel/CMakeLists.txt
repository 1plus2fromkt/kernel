cmake_minimum_required(VERSION 2.8)

project(kernel)

function(compile_i686 name)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o
		COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c -o ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o  -I ${CMAKE_BINARY_DIR}/../ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h
	)
endfunction(compile_i686)

compile_i686("kernel")
compile_i686("paging")
compile_i686("tty")
compile_i686("io")
compile_i686("idt")
compile_i686("pic")
compile_i686("handlers")

add_custom_target(
	kernel_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o ${CMAKE_CURRENT_SOURCE_DIR}/paging.o ${CMAKE_CURRENT_SOURCE_DIR}/tty.o ${CMAKE_CURRENT_SOURCE_DIR}/io.o ${CMAKE_CURRENT_SOURCE_DIR}/idt.o ${CMAKE_CURRENT_SOURCE_DIR}/pic.o ${CMAKE_CURRENT_SOURCE_DIR}/handlers.o
) 
#I know how it looks like, I know it can be replaced, I am too lazy to replace it. If you want to replace it, do it, please.
