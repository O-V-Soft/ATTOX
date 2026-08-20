[bits 32]

[global timer_wrapper]
[global keyboard_wrapper]
[extern timer_handler]
[extern keyboard_handler]

timer_wrapper:
    pusha
    call timer_handler
    popa
    iret

keyboard_wrapper:
    pusha
    call keyboard_handler
    popa
    iret