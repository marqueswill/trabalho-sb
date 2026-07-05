section .text
    global exponenciacao

exponenciacao:
    push ebp
    mov ebp, esp
    mov eax, 1
    xor edx, edx               ; edx = flag overflow
    mov ecx, [ebp+12]          ; expoente
.loop:
    cmp ecx, 0
    jle .fim
    imul eax, [ebp+8]
    jno .semOverflow
    mov edx, 1
    jmp .fim
.semOverflow:
    dec ecx
    jmp .loop
.fim:
    mov esp, ebp
    pop ebp
    ret