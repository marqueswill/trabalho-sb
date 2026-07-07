section .data
str_ecerrar     db      "Encerrando calculadora", 0xA, 0x0
len_str_ecerrar equ     $ - str_ecerrar

str_resultado   db      "Resultado:", 0xA, 0x0
len_str_resultado equ     $ - str_resultado

str_continuar   db      0xa, "Pressione enter para continuar", 0x0
len_str_continuar equ     $ - str_continuar

str_overflow    db      "OCORREU OVERFLOW", 0xA, 0x0
len_str_overflow equ     $ - str_overflow

str_opcao_invalida db      "OPCAO INVALIDA!", 0xA, 0x0
len_str_opcao_invalida equ     $ - str_opcao_invalida

section .bss
nome_usuario    resb    30						; Armazena o nome do usuário
resposta_precisao resd    1						; Armazena a precisão (0 para 16 bits, 1 para 32 bits)
opcao_menu      resd    1						; Armazena a opção do menu (1 a 7)


section .text
    global _start

	global opcao_menu
	global resposta_precisao
	global nome_usuario

	; Funções para operações
	extern adicao_int32
	extern subtracao_int32
	extern multiplicacao_int32
	extern divisao_int32
	extern mod_int32
	extern exponenciacao_int32

	extern adicao_int16
	extern subtracao_int16
	extern multiplicacao_int16
	extern divisao_int16
	extern mod_int16
	extern exponenciacao_int16

	; Funções de IO
	extern saudacao
	extern pergunta_precisao
	extern exibir_menu

	extern ler_string
    extern print_string

	extern ler_int32
	extern print_int32
	extern int32_to_ascii

	extern ler_int16
    extern print_int16
	extern int16_to_ascii

_start:
	call    saudacao
	call    pergunta_precisao					; salva precisao em [resposta_precisao] e retorna esse valor

loop_execucao:
	call    exibir_menu							; salva operacao em [opcao_menu] e retorna esse valor

	cmp     eax, 0x7
	je      end

	mov     eax, [resposta_precisao]
	cmp     eax, 0x0
	je      precisao_16bit

	call    switch_case_operacao_32bit
	jmp     continuar_loop_execucao

precisao_16bit:
	call    switch_case_operacao_16bit

continuar_loop_execucao:
	call    ler_string
	jmp     loop_execucao

end:
	push    len_str_ecerrar
	push    str_ecerrar
	call    print_string
	add     esp, 8

    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80


; === FUNÇÕES AUXILIARES ===
switch_case_operacao_32bit:
	push    ebp
	mov     ebp, esp

	mov     eax, [opcao_menu]

	cmp     eax, 0x1
	call    adicao_int32
	jmp     mostrar_resultado_int32

	cmp     eax, 0x2
	call    subtracao_int32
	jmp     mostrar_resultado_int32

	cmp     eax, 0x3
	call    multiplicacao_int32
	jmp     mostrar_resultado_int32

	cmp     eax, 0x4
	call    divisao_int32
	jmp     mostrar_resultado_int32

	cmp     eax, 0x5
	call    exponenciacao_int32
	jmp     mostrar_resultado_int32

	cmp     eax, 0x6
	call    mod_int32
	jmp     mostrar_resultado_int32

;	push    len_str_opcao_invalida
;	push    str_opcao_invalida
;	call    print_string
;	add     esp, 8

	jmp     end_switch_case_operacao_32bit


mostrar_resultado_int32:
	sub     esp, 32								; aloca espaço para escrever o resultado
	mov     ebx, eax							; guarda o resultado

	push    len_str_resultado
	push    str_resultado
	call    print_string
	add     esp, 8

	push    ebx
	lea     eax, [ebp - 32]
	push    eax
	call    int32_to_ascii						; converte para int para ascii e retorna o número de bytes em buffer_escrita
	add     esp, 8

	push    eax
	lea     eax, [ebp - 32]
	push    eax
	call    print_string
	add     esp, 8

end_switch_case_operacao_32bit:
	mov     eax, 0x1							; flag em 1 para continuar exec
	mov     esp, ebp
	pop     ebp
	ret

switch_case_operacao_16bit:
	push    ebp
	mov     ebp, esp

	mov     eax, [opcao_menu]

	cmp     eax, 0x1
	call    adicao_int16
	jmp     mostrar_resultado_int16

	cmp     eax, 0x2
	call    subtracao_int16
	jmp     mostrar_resultado_int16

	cmp     eax, 0x3
	call    multiplicacao_int16
	jmp     mostrar_resultado_int16

	cmp     eax, 0x4
	call    divisao_int16
	jmp     mostrar_resultado_int16

	cmp     eax, 0x5
	call    exponenciacao_int16
	jmp     mostrar_resultado_int16

	cmp     eax, 0x6
	call    mod_int16
	jmp     mostrar_resultado_int16

	push    len_str_opcao_invalida
	push    str_opcao_invalida
	call    print_string
	add     esp, 8

	jmp     end_switch_case_operacao_16bit


mostrar_resultado_int16:
	sub     esp, 32								; aloca espaço para escrever o resultado
	mov     ebx, eax							; guarda o resultado

	push    len_str_resultado
	push    str_resultado
	call    print_string
	add     esp, 8

	push    ebx
	lea     eax, [ebp - 32]
	push    eax
	call    int16_to_ascii						; converte para int para ascii e retorna o número de bytes em buffer_escrita
	add     esp, 8

	push    eax
	lea     eax, [ebp - 32]
	push    eax
	call    print_string
	add     esp, 8

end_switch_case_operacao_16bit:
	mov     eax, 0x1							; flag em 1 para continuar exec
	mov     esp, ebp
	pop     ebp
	ret