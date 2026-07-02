section .data
; Bem-vindo. Digite seu nome:
; Hola, <nome do usuario>, bem-vindo ao programa de CALC IA-32
; Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32):

;ESCOLHA UMA OPÇÃO: 
;- 1: SOMA 
;- 2: SUBTRACAO 
;- 3: MULTIPLICACAO 
;- 4: DIVISAO 
;- 5: EXPONENCIACAO 
;- 6: MOD 
;- 7: SAIR

section .text
    global _start

	; Funções para operações
	extern adicao
	extern subtracao
	extern multiplicacao
	extern divisao
	extern modulo
	extern exponenciacao

	; Funções de IO
    extern print_string
    extern ler_string
    extern ler_int32
    extern ler_int16

_start:
    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80