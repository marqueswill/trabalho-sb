section .text
    global multiplicacao_int32
    global multiplicacao_int16

multiplicacao_int32:
    push ebp
    mov ebp, esp
    xor edx, edx              ; edx = flag overflow (0 = ok)
    mov eax, [ebp+8]
    imul eax, [ebp+12]        ; instrucao IA-32 de multiplicacao (exigida)
    jno .fim
    mov edx, 1
.fim:
    mov esp, ebp
    pop ebp
    ret


multiplicacao_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret