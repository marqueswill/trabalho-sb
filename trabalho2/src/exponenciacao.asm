section .text
    global exp_int32
    global exponenciacao_int16
    extern ler_int32
    extern ler_int16
	extern aviso_overflow
	extern escolha_op_32bit.fim


exp_int32:
	push    ebp
	mov     ebp, esp

	mov     eax, 1
	xor     edx, edx							; edx = flag overflow


	call    ler_int32							; base
	push    eax

	call    ler_int32							; expoente
	mov     ecx, eax

	pop     ebx

	xor     eax, eax							; zera eax
	inc     eax									; coloca 1 em eax

.loop:
	cmp     ecx, 0
	jle     .fim

	imul    eax, ebx
	jno     .sem_overflow

	jmp     .teve_overflow

.sem_overflow:
	dec     ecx
	jmp     .loop

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

	mov     eax, 0x0							; flag pra parar exec
	jmp     escolha_op_32bit.fim				; gambiarra


exponenciacao_int16:

	mov     esp, ebp
	pop     ebp
	ret

