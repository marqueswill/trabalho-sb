section .text
    global div_int32
    global divisao_int16
    extern ler_int32
    extern ler_int16

div_int32:
	push    ebp
	mov     ebp, esp

	call    ler_int32
	push    eax

	call    ler_int32
	mov     ebx, eax

	pop     eax
	cdq     									; estende sinal EAX -> EDX:EAX (necessario p/ idiv)
	idiv    ebx									; instrucao IA-32 de divisao (exigida)

	mov     esp, ebp
	pop     ebp
	ret

divisao_int16:
    push    ebp
    mov     ebp, esp

    call    ler_int16
    push    eax

    call    ler_int16
    mov     ebx, eax

    pop     eax
    cdq
    idiv    ebx

    mov     esp, ebp
    pop     ebp
    ret