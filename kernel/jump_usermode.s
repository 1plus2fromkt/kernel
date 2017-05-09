.global jump_usermode
.extern test

jump_usermode:
     movw 0x23, %ax
     movw %ax, %ds
     movw %ax, %es 
     movw %ax, %fs 
     movw %ax, %gs

     mov %esp, %eax
     push 0x23
     push %eax
     pushf
     push 0x1B
     push test
     iret
