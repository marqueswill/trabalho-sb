section .text
    global mult_int32
    global multiplicacao_int16
    extern ler_int32
    extern ler_int16
	extern escolha_op_32bit.fim
	extern aviso_overflow

mult_int32:
	push    ebp
	mov     ebp, esp

	xor     edx, edx							; edx = flag overflow (0 = ok)

	call    ler_int32
	push    eax

	call    ler_int32

	pop     ebx
	imul    eax, ebx							; instrucao IA-32 de multiplicacao (exigida)
	jno     .fim

	jmp     .teve_overflow

.fim:
	mov     esp, ebp
	pop     ebp
	ret

.teve_overflow:
	mov     edx, 1								; ocorreu overflow

	call    aviso_overflow
	mov     esp, ebp
	pop     ebp
	pop     eax									; retira o endereço de retorno (ret)
	jmp     escolha_op_32bit.fim				; gambiarra


multiplicacao_int16:
	push    ebp
	mov     ebp, esp

	mov     esp, ebp
	pop     ebp
	ret