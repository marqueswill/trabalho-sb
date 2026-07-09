section .text
    global adicao_int32
    global adicao_int16

    extern ler_int32
    extern ler_int16

; pede dois inteiros, soma eles e retorna o valor em eax
adicao_int32:
	push    ebp
	mov     ebp, esp

	call    ler_int32
	push    eax

	call    ler_int32
	pop     ebx

	add     eax, ebx

	mov     esp, ebp
	pop     ebp
	ret

adicao_int16:
	push    ebp
	mov     ebp, esp

	mov     esp, ebp
	pop     ebp
	ret