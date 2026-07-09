section .data
str_ecerrar     db      "Encerrando calculadora", 0xA, 0x0
len_str_ecerrar equ     $ - str_ecerrar

str_continuar   db      0xa, "Pressione enter para continuar", 0x0
len_str_continuar equ     $ - str_continuar



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
	global escolha_op_32bit.fim

	; Funções para operações
	extern adicao_int32
	extern sub_int32
	extern mult_int32
	extern div_int32
	extern mod_int32
	extern exp_int32

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
	je      .end

	mov     eax, [resposta_precisao]
	cmp     eax, 0x0
	je      .precisao_16bit

	call    escolha_op_32bit
	jmp     .continuar_loop_execucao

.precisao_16bit:
	call    escolha_op_16bit

.continuar_loop_execucao:
	sub     esp, 32

	mov     eax, esp
	push    eax

	call    ler_string

	add     esp, 36

	jmp     loop_execucao

.end:
	push    len_str_ecerrar
	push    str_ecerrar
	call    print_string
	add     esp, 8

    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80


; === FUNÇÕES AUXILIARES ===
escolha_op_32bit:
	push    ebp
	mov     ebp, esp

	mov     eax, [opcao_menu]

	cmp     eax, 0x1
	je      .op_adicao

	cmp     eax, 0x2
	je      .op_subtracao

	cmp     eax, 0x3
	je      .op_multiplicacao

	cmp     eax, 0x4
	je      .op_divisao

	cmp     eax, 0x5
	je      .op_exponenciacao

	cmp     eax, 0x6
	je      .op_mod

	jmp     .fim

.op_adicao:
	call    adicao_int32
	jmp     .resultado

.op_subtracao:
	call    sub_int32
	jmp     .resultado

.op_multiplicacao:
	call    mult_int32
	jmp     .resultado

.op_divisao:
	call    div_int32
	jmp     .resultado

.op_exponenciacao:
	call    exp_int32
	jmp     .resultado

.op_mod:
	call    mod_int32
	jmp     .resultado


.resultado:
	push    eax
	call    print_int32
	add     esp, 4
	mov     eax, 0x1							; flag em 1 para continuar exec

.fim:
	mov     esp, ebp
	pop     ebp
	ret

escolha_op_16bit:

; TODO: implementar esse switch case aqui