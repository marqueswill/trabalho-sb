section .text
    global adicao

adicao:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    add eax, [ebp+12]
    mov esp, ebp
    pop ebp
    ret