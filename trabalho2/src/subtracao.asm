section .text
    global subtracao_int32
    global subtracao_int16

subtracao_int32:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    sub eax, [ebp+12]
    mov esp, ebp
    pop ebp
    ret



subtracao_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret