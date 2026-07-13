section .text
    global sub_int32
    global subtracao_int16
    extern ler_int32
    extern ler_int16

sub_int32:
	push    ebp
	mov     ebp, esp

	call    ler_int32
	push    eax

	call    ler_int32

	mov     ebx, eax
	pop     eax
	sub     eax, ebx							; int1 - int2


	mov     esp, ebp
	pop     ebp
	ret



subtracao_int16:
    push    ebp
    mov     ebp, esp

    call    ler_int16
    push    eax

    call    ler_int16
    mov     ebx, eax
    pop     eax
    sub     eax, ebx                    ; int1 - int2

    mov     esp, ebp
    pop     ebp
    ret