cmake_minimum_required(VERSION 2.8)

project(kernel)

function(compile_i686 name)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o
		COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c -o ${CMAKE_CURRENT_SOURCE_DIR}/${name}.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h
	)
endfunction(compile_i686)

compile_i686("kernel")
compile_i686("paging")

#add_custom_command( 
#	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o
#	COMMAND i686-elf-gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/kernel.c -o ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra 
#	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.c
#)



add_custom_target(
	kernel_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.o ${CMAKE_CURRENT_SOURCE_DIR}/paging.o
)
