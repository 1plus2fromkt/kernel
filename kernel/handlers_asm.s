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
	# pushal
	# cld
	# call tempor_impl
	# popal
	iret
	