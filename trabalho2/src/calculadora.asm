section .data
	str_ecerrar     db      "Encerrando calculadora", 0xA, 0x0
	len_str_ecerrar equ     $ - str_ecerrar

	str_resultado db      "Resultado:", 0xA, 0x0
	len_str_resultado equ     $ - str_resultado

	str_continuar db      0xa,"Pressione enter para continuar", 0x0
	len_str_continuar equ     $ - str_continuar

	str_overflow db      "OCORREU OVERFLOW", 0xA, 0x0
	len_str_overflow equ     $ - str_overflow

section .bss
	; TODO: não pode isso aqui não
	extern buffer_escrita

	; TODO: remover acesso na memória e usar ponteiro
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
    extern ler_inteiro
	extern print_int32
    extern print_int16
	extern saudacao
	extern exibir_menu
	extern int32_to_ascii

_start:
	; TODO: Instanciar os buffers de leitura e escrita dinamicamente na pilha (sub esp, X) para abolir o uso global na .bss.

	call    saudacao
	mov     dword [tipo_inteiro], eax									; salvo na memória

	.loop_execucao:
		call    exibir_menu
		cmp eax, 0x7
		je end

		push    eax ; salva a operação


		; TODO: passar leitura para dentro das operações (fica redundante mas é requerimento)
		mov     [tipo_inteiro], eax
		; TODO: separar em ler_inteiro32 e ler_inteiro16
		call    ler_inteiro
		push 	eax ; salva o inteiro 1

		mov     [tipo_inteiro], eax
		; TODO: separar em ler_inteiro32 e ler_inteiro16
		call    ler_inteiro			
		push 	eax ; salva o inteiro 2

		; ordem na pilha: operação -> inteiro 1 -> inteiro 2
		call    switch_case_operacao				; recebe a operação e os inteiros, devolve o resultado (1 para interromper, 0 para continuar)
		add     esp, 12 							; reseta pilha pro próximo loop
	
		cmp     eax, 0x0
		je end

		push    len_str_continuar
		push    str_continuar
		call    print_string
		add 	esp, 8

		call 	ler_string
		
		jmp      .loop_execucao

end:
	push    len_str_ecerrar
	push    str_ecerrar
	call    print_string
	add 	esp, 8

    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80


; === FUNÇÕES AUXILIARES ===
switch_case_operacao:
	push    ebp
	mov     ebp, esp

	mov     eax, [ebp+8]						; 1o arg: Inteiro 2
	mov     ebx, [ebp+12]						; 2o arg: Inteiro 1
	mov     ecx, [ebp+16]						; 3o arg: Operacao

	; TODO: refatorar operações para elas mesmas lerem os inteiros
	; TODO: fazer um switch case para 16bits e 32bits
	; TODO: implementar 2 versões para cada operação

	push    eax									; empilha int1 para chamada da operação
	push    ebx									; empilha int2 para chamada da operação

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
		add 	esp, 8
		push 	eax
		
		push len_str_resultado
		push str_resultado
		call print_string
		add esp, 8

		cmp     dword [tipo_inteiro], 0x0
		je      eh_int16

		push buffer_escrita
		call int32_to_ascii ; converte para int para ascii e retorna o número de bytes em buffer_escrita
		add esp, 8

		push eax
		push buffer_escrita
		call print_string
		add esp, 8

		mov     eax, 0x1							; flag em 1 para continuar exec
		jmp     end_switch_case_operacao

		eh_int16:
		; push 	eax
		; call    print_int16

		; add     esp, 12 								; limpa pilha por causa da chamada da operação
		mov     eax, 0x1							; flag em 1 para continuar exec

	end_switch_case_operacao:
		mov     esp, ebp
		pop     ebp
		ret