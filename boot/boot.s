# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel. These are magic
# values that are documented in the multiboot standard. The bootloader will
# search for this signature in the first 8 KiB of the kernel file, aligned at a
# 32-bit boundary. The signature is in its own section so the header can be
# forced to be within the first 8 KiB of the kernel file.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# The multiboot standard does not define the value of the stack pointer register
# (esp) and it is up to the kernel to provide a stack. This allocates room for a
# small stack by creating a symbol at the bottom of it, then allocating 16384
# bytes for it, and finally creating a symbol at the top. The stack grows
# downwards on x86. The stack is in its own section so it can be marked nobits,
# which means the kernel file is smaller because it does not contain an
# uninitialized stack. The stack on x86 must be 16-byte aligned according to the
# System V ABI standard and de-facto extensions. The compiler will assume the
# stack is properly aligned and failure to align the stack will result in
# undefined behavior.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text
.global pd
.global dn
.global _start
.type _start, @function
_start:
	# The bootloader has loaded us into 32-bit protected mode on a x86
	# machine. Interrupts are disabled. Paging is disabled. The processor
	# state is as defined in the multiboot standard. The kernel has full
	# control of the CPU. The kernel can only make use of hardware features
	# and any code it provides as part of itself. There's no printf
	# function, unless the kernel provides its own <stdio.h> header and a
	# printf implementation. There are no security restrictions, no
	# safeguards, no debugging mechanisms, only what the kernel provides
	# itself. It has absolute and complete power over the
	# machine.

	# To set up a stack, we set the esp register to point to the top of our
	# stack (as it grows downwards on x86 systems). This is necessarily done
	# in assembly as languages such as C cannot function without a stack.

    movl $stack_top, %esp

	# This is a good place to initialize crucial processor state before the
	# high-level kernel is entered. It's best to minimize the early
	# environment where crucial features are offline. Note that the
	# processor is not fully initialized yet: Features such as floating
	# point instructions and instruction set extensions are not initialized
	# yet. The GDT should be loaded here. Paging should be enabled here.
	# C++ features such as global constructors and exceptions will require
	# runtime support to work as well.

	# call init_paging

	# movl $(pt1 - 0xC0000000 + 0x3), pd - 0xC0000000
	# movl $(pt1 - 0xC0000000 + 0x3), pd - 0xC0000000 + 768 * 4
	# movl $(pt2 - 0xC0000000 + 0x3), pd - 0xC0000000 + 4
	# movl $(pt2 - 0xC0000000 + 0x3), pd - 0xC0000000 + 768 * 4 + 4

	movl $0, %ecx

loop1:
	movl %ecx, %edx
	addl $768, %edx # writing to pt[768]
	imul $0x1000, %edx
	add $(pt), %edx
	add $0x3, %edx
	movl %ecx, %edi
	imul $4, %edi
	add $(pd), %edi
	sub $0xC0000000, %edi
	sub $0xC0000000, %edx
	movl %edx, (%edi) 
	add $3072, %edi #3072 = 768 * 4 -- start of kernel
	movl %edx, (%edi)
	inc %ecx
	cmpl (dn - 0xC0000000), %ecx 
	jne loop1


	movl $0, %esi
loop2:
	movl %esi, %ebp
	addl $768, %ebp
	imul $0x1000, %ebp
	add $(pt), %ebp
	sub $0xC0000000, %ebp

	movl $0, %ecx
	movl %esi, %edi
	imul $0x400000, %edi
loop:
	movl %edi, %edx
	orl $0x3, %edx
	movl %edx, (%ebp)
	add $0x1000, %edi
	add $4, %ebp
	add $1, %ecx
	cmpl $1024, %ecx
	jne loop

	add $1, %esi
	cmpl (dn - 0xC0000000), %esi 
	jne loop2


	mov $pd, %ecx
	subl $0xC0000000, %ecx
    mov %ecx, %cr3

    mov %cr0, %ecx
    orl $0x80000000, %ecx
    movl %ecx, %cr0

    lea 1f, %ecx
    jmp *%ecx 

 1:

 # 	movl $0, pd + 0
	# movl %cr3, %ecx
	# movl %ecx, %cr3 # invalidate first page directory because it is unused. Although I should invalidate all of the first pages, I'll just do it in kernel

	pushl %ebx #passing ebx argiment to kernel_start, there is an information about RAM in ebx
	pushl %eax

	# Enter the high-level kernel. The ABI requires the stack is 16-byte
	# aligned at the time of the call instruction (which afterwards pushes
	# the return pointer of size 4 bytes). The stack was originally 16-byte
	# aligned above and we've since pushed a multiple of 16 bytes to the
	# stack since (pushed 0 bytes so far) and the alignment is thus
	# preserved and the call is well defined.
	call kernel_main

	# If the system has nothing more to do, put the computer into an
	# infinite loop. To do that:
	# 1) Disable interrupts with cli (clear interrupt enable in eflags).
	#    They are already disabled by the bootloader, so this is not needed.
	#    Mind that you might later enable interrupts and return from
	#    kernel_main (which is sort of nonsensical to do).
	# 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	#    Since they are disabled, this will lock up the computer.
	# 3) Jump to the hlt instruction if it ever wakes up due to a
	#    non-maskable interrupt occurring or due to system management mode.
	cli
2:	hlt
	jmp 2b

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start