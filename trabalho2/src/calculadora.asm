section .data
str_ecerrar     db      "Encerrando calculadora", 0xA, 0x0
len_str_ecerrar equ     $ - str_ecerrar

section .bss
tipo_inteiro    resd    1

section .text
    global _start

	; Funções para operações
	extern adicao
	extern subtracao
	extern multiplicacao
	extern divisao
	extern mod
	extern exponenciacao

	; Funções de IO
    extern print_string
    extern ler_string
    extern ler_int16
    extern ler_int32
	extern print_int32
    extern print_int16
	extern saudacao
	extern exibir_menu

_start:
; TODO: fazer loop execucao

	call    saudacao
	mov     dword [tipo_inteiro], eax									; salvo na memória

loop_execucao:
	call    exibir_menu
	push    eax									
	call    ler_inteiro				
	push 	eax
	call    ler_inteiro						
	push 	eax
	; ordem na pilha: operação -> inteiro 1 -> inteiro 2

	call    switch_case_operacao				; recebe a operação e os inteiros
	add     esp, 12 							; reseta pilha pro próximo loop
	
	cmp     eax, 0x1
	je      loop_execucao

end:
    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80

; === FUNÇÕES AUXILIARES ===

ler_inteiro:
; TODO: implementar essa bomba aqui
; retorno tem que ser pela pilha
	push    ebp
	mov     ebp, esp

	mov     [tipo_inteiro], eax
	cmp     eax, 0x1
	je      eh_pra_ler_int32

	call    ler_int16
	jmp     end_ler_inteiro

	eh_pra_ler_int32:
		call    ler_int32

	end_ler_inteiro:
		mov     esp, ebp
		pop     ebp
		ret

switch_case_operacao:
	push    ebp
	mov     ebp, esp

	mov     eax, [ebp+8]						; 1o arg: Inteiro 2
	mov     ebx, [ebp+12]						; 2o arg: Inteiro 1
	mov     ecx, [ebp+16]						; 3o arg: Operacao

	push    ebx									; empilha int2 para chamada da operação
	push    eax									; empilha int1 para chamada da operação

	cmp     ecx, 0x1
	je      fazer_adicao

	cmp     ecx, 0x2
	je      fazer_subtracao

	cmp     ecx, 0x3
	je      fazer_multiplicacao

	cmp     ecx, 0x4
	je      fazer_divisao

	cmp     ecx, 0x5
	je      fazer_exponenciacao

	cmp     ecx, 0x6
	je      fazer_mod

	push    len_str_ecerrar
	push    str_ecerrar
	call    print_string
	add     esp, 8 								; limpa a pilha
	mov     eax, 0x0							; flag em 0 para parar exec
	jmp     end_switch_case_operacao

	fazer_adicao:
		call    adicao
		jmp     limpar_e_mostrar

	fazer_subtracao:
		call    subtracao
		jmp     limpar_e_mostrar

	fazer_multiplicacao:
		call    multiplicacao
		jmp     limpar_e_mostrar

	fazer_divisao:
		call    divisao
		jmp     limpar_e_mostrar

	fazer_exponenciacao:
		call    exponenciacao
		jmp     limpar_e_mostrar

	fazer_mod:
		call    mod
		jmp     limpar_e_mostrar

	limpar_e_mostrar:

		cmp     dword [tipo_inteiro], 0x1
		je      eh_int32

		call    print_int16

		mov     eax, 0x1							; flag em 1 para continuar exec
		jmp     end_switch_case_operacao

		eh_int32:
			call    print_int32

		add     esp, 8 								; limpa pilha por causa da chamada da operação
		mov     eax, 0x1							; flag em 1 para continuar exec

	end_switch_case_operacao:
		mov     esp, ebp
		pop     ebp
		ret