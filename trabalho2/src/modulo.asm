; int mod(int a, int b) -> eax = resto de (a / b)
section .text
    global mod_int32
    global mod_int16
    extern ler_int32
    extern ler_int16

mod_int32:
	push    ebp
	mov     ebp, esp

	call    ler_int32
	push    eax

	call    ler_int32
	mov     ebx, eax

	pop     eax
	cdq     									; estende sinal EAX -> EDX:EAX (necessario p/ idiv)
	idiv    ebx									; instrucao IA-32 de divisao (exigida)

	mov     eax, edx

	mov     esp, ebp
	pop     ebp
	ret

mod_int16:
	push    ebp
	mov     ebp, esp

	mov     esp, ebp
	pop     ebp
	ret