[bits 32]

[global timer_wrapper]
[global keyboard_wrapper]
[global fault_wrapper]
[global syscall_wrapper]
[extern timer_handler]
[extern keyboard_handler]
[extern fault_handler]
[extern schedule]
[extern syscall_handler]

timer_wrapper:
    pusha                 
    
    push esp           
    call schedule        
    mov esp, eax          

    mov al, 0x20
    out 0x20, al        

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

syscall_wrapper:
    pusha   

    push esp   
    call syscall_handler

    add esp, 4
   
    popa         
    iret
