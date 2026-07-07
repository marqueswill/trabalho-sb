section .text
    global exponenciacao_int32
    global exponenciacao_int16

; TODO: adicionar mensagem overflow
exponenciacao_int32:
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

exponenciacao_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret