cmake_minimum_required(VERSION 2.8)

project(os)

add_subdirectory(boot)
add_subdirectory(libc)
add_subdirectory(kernel)
add_subdirectory(memory)

set (object_files 
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/linker.ld"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.o" 
		"${CMAKE_CURRENT_SOURCE_DIR}/kernel/kernel.o" 
		"${CMAKE_CURRENT_SOURCE_DIR}/kernel/paging.o"
		"${CMAKE_CURRENT_SOURCE_DIR}/libc/itoa.o"
		"${CMAKE_CURRENT_SOURCE_DIR}/libc/reverse.o"
		"${CMAKE_CURRENT_SOURCE_DIR}/memory/memory.o"
		"${CMAKE_CURRENT_SOURCE_DIR}/kernel/tty.o"
	)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin
	COMMAND i686-elf-gcc -T ${CMAKE_CURRENT_SOURCE_DIR}/boot/linker.ld -o ${CMAKE_CURRENT_SOURCE_DIR}/myos.bin 
						-ffreestanding -O2 -nostdlib ${object_files} -lgcc
	DEPENDS memory_run kernel_run boot_run libc_run ${object_files} 
)


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
