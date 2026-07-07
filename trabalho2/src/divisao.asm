section .text
    global divisao_int32
    global divisao_int16

divisao_int32:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    cdq                        ; estende sinal EAX -> EDX:EAX (necessario p/ idiv)
    idiv dword [ebp+12]        ; instrucao IA-32 de divisao (exigida)
    mov esp, ebp
    pop ebp
    ret

divisao_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret