.global kbd_interrupt_impl
.global kbd_wrapper
.global tempor_impl
.align 4
.type kbd_wrapper, @function
kbd_wrapper:
    pushal
    cld
    call kbd_interrupt_impl
    popal
    iret


.global syscall_wrapper
.align 4
.type syscall_wrapper, @function
syscall_wrapper:
	# load arguments from registers
	pushal
	cld
	# push them as arguments to syscall_impl
	call syscall_impl
	popal
	iret

.global empty_wrapper
.align 4
.type empty_wrapper, @function
empty_wrapper:
	pushal
	cld
	pushl $0
	call pic_send_eoi
	popl %eax # remove "0" from stack, so it will return to the prev size
	popal
	iret