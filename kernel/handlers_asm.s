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


.global tempor
.align 4
.type tempor, @function
tempor:
	pushal
	pushl %ds
	pushl %es
	pushl %ebp
	
	# call tempor_impl
	# addl $4, %esp

	popl %ebp
	popl %es
	popl %ds
	popal
	iret
