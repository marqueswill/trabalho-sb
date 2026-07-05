section .text
    global subtracao

subtracao:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    sub eax, [ebp+12]
    mov esp, ebp
    pop ebp
    ret