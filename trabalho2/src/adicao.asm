section .text
    global adicao_int32
    global adicao_int16

adicao_int32:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    add eax, [ebp+12]
    mov esp, ebp
    pop ebp
    ret

adicao_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret