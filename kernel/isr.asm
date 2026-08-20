[bits 32]

[global timer_wrapper]
[global keyboard_wrapper]
[global fault_wrapper]
[extern timer_handler]
[extern keyboard_handler]
[extern fault_handler]

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

fault_wrapper:
    cli                       
    pusha                     
    
    call fault_handler   
    
    popa
    add esp, 4                 
    iret