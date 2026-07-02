section .data

section .text
    global _start
	extern adicao
	extern subtracao
	extern multiplicacao
	extern divisao
	extern modulo
	extern exponenciacao

_start:
    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80