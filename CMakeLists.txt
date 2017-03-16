cmake_minimum_required(VERSION 2.8)

project(os)

add_subdirectory(boot)
add_subdirectory(kernel)
add_subdirectory(paging)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin
	COMMAND i686-elf-gcc -T ${CMAKE_CURRENT_SOURCE_DIR}/boot/linker.ld -o ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin -ffreestanding -O2 -nostdlib ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.o ${CMAKE_CURRENT_SOURCE_DIR}/kernel/kernel.o ${CMAKE_CURRENT_SOURCE_DIR}/paging/paging.o -lgcc
	DEPENDS boot kernel paging
)

#add_custom_target(
#	create_bin
#	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin
#)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/myos.iso
	COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/isodir/boot/grub
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin ${CMAKE_CURRENT_SOURCE_DIR}/isodir/boot/myos.bin
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/boot/grub.cfg ${CMAKE_CURRENT_SOURCE_DIR}/isodir/boot/grub/grub.cfg
	COMMAND grub-mkrescue -o ${CMAKE_CURRENT_SOURCE_DIR}/myos.iso ${CMAKE_CURRENT_SOURCE_DIR}/isodir
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin
)

add_custom_target(
	create_iso ALL
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/myos.iso 
)