cmake_minimum_required(VERSION 2.8)

project(boot)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/boot.o
	COMMAND i686-elf-as ${CMAKE_CURRENT_SOURCE_DIR}/boot.s -o ${CMAKE_CURRENT_SOURCE_DIR}/boot.o
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot.s
)

add_custom_target(
	boot_run ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot.o
)