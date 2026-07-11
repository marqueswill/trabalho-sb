section .text
    global mod_int32
    global mod_int16
    extern ler_int32
    extern ler_int16
    extern aviso_div_zero
    extern escolha_op_32bit.fim
    extern escolha_op_16bit.fim

mod_int32:
	push    ebp
	mov     ebp, esp

	call    ler_int32
	push    eax

	call    ler_int32
	mov     ebx, eax

	cmp     ebx, 0
	je      .div_zero

	pop     eax
	cdq

	idiv    ebx


	mov     eax, edx


	mov     esp, ebp
	pop     ebp
	ret

.div_zero:
	call    aviso_div_zero

	mov     esp, ebp
	pop     ebp
	pop     eax

	mov     eax, 0x0
	jmp     escolha_op_32bit.fim


mod_int16:
	push    ebp
	mov     ebp, esp

	call    ler_int16
	push    eax

	call    ler_int16
	mov     ebx, eax

	cmp     bx, 0

	je      .div_zero16

	pop     eax
	cwd

	idiv    bx


	movsx   eax, dx


	mov     esp, ebp
	pop     ebp
	ret

.div_zero16:
	call    aviso_div_zero
	mov     esp, ebp
	pop     ebp
	pop     eax

	jmp     escolha_op_16bit.fim