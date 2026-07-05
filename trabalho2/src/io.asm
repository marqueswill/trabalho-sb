section .data

str_bemvindo    db      "Bem-vindo. Digite seu nome: ", 0xA, 0x0
len_str_bemvindo equ     $ - str_bemvindo		; endereço atual - endereço final de `bemvindo`

str_saudacao1   db      "Hola, ", 0x0
len_str_saudacao1 equ     $ - str_saudacao1

str_saudacao2   db      ", bem-vindo ao programa de CALC IA-32.", 0xA, 0x0
len_str_saudacao2 equ     $ - str_saudacao2

str_tipo_inteiro db      "Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): ", 0xA, 0x0
len_str_tipo_inteiro equ     $ - str_tipo_inteiro

str_menu        db      0xA, "Escolha uma opção:", 0xA, \
	"- 1: soma",0xa,\
	"- 2: subtracao",0xa,\
	"- 3: multiplicacao",0xa,\
	"- 4: divisao",10,\
	"- 5: exponenciacao",0xa,\
	"- 6: mod",0xa,\
	"- 7: sair",0xa,0xa,0x0
len_str_menu    equ     $ - str_menu

section .bss
buffer_leitura  resb    30
tamanho_nome    resd    1

section .text
    global saudacao
    global print_string
    global ler_string
    global exibir_menu
    global ler_int16
    global ler_int32
    global print_int16
    global print_int32

; Recebe via pilha: [ponteiro da string] e [quantidade de bytes]
print_string:
	push    ebp									; empilho <referencia antiga> de epb -> [ebp] = ponteiro para fixar esp atual; [ebp + 4] = endereço retorno ; [ebp + 8] = ponteiro string; [ebp + 12] = tamanho string
	mov     ebp, esp							; salvo a <referencia atual de esp> em epb, fixando as referências

	mov     eax, 4								; número da syscall para 32-bits (ou 1 para 64-bits usando rax/syscall)
	mov     ebx, 1								; stdout
	mov     ecx, [ebp + 8]						; copio o ponteiro string
	mov     edx, [ebp + 12]						; copio o tamanho string
	int     0x80								; chama o kernel

	mov     esp, ebp							; restauro a <referência antiga de esp>
	pop     ebp									; restauro a <referência antiga de ebp>, esp agora aponta para o endereço de retorno
	ret

ler_string:
	push    ebp
	mov     ebp, esp

	mov     eax, 3
	mov     ebx, 0
	mov     ecx, buffer_leitura
	mov     edx, 30								; Limite máximo de bytes a serem lidos
	int     0x80

	mov     ebx, eax
	dec     ebx
	mov     byte [buffer_leitura + ebx], 0		; ignora o enter no nome
	dec     eax

	mov     esp, ebp
	pop     ebp
	ret

ler_int32:
	push    ebp
	mov     ebp, esp


	mov     esp, ebp
	pop     ebp
	ret

ler_int16:
	push    ebp
	mov     ebp, esp


	mov     esp, ebp
	pop     ebp
	ret



; faz a saudacao e retorna o tipo de inteiro das operações
saudacao:
	push    ebp
	mov     ebp, esp

	push    len_str_bemvindo
	push    str_bemvindo
	call    print_string

	call    ler_string
	mov     [tamanho_nome], eax

	push    len_str_saudacao1
	push    str_saudacao1
	call    print_string

	push    dword [tamanho_nome]
	push    buffer_leitura
	call    print_string

	push    len_str_saudacao2
	push    str_saudacao2
	call    print_string

	push    len_str_tipo_inteiro
	push    str_tipo_inteiro
	call    print_string

	call    ler_string
	movzx   eax, byte [buffer_leitura]			; lê o primeiro byte
	sub     eax, 48								; converte para inteiro

    ; TODO: verificar se entrada é valida -> 0 ou 1

	mov     esp, ebp
	pop     ebp
	ret

; retorna a opcao de operação escolhida pelo usuario
exibir_menu:
	push    ebp
	mov     ebp, esp

	push    len_str_menu
	push    str_menu
	call    print_string

	call    ler_string
	movzx   eax, byte [buffer_leitura]			; lê o primeiro byte
	sub     eax, 48								; converte para inteiro

    ; TODO: verificar se entrada é valida -> 1 a 7

	mov     esp, ebp
	pop     ebp
	ret

ascii_to_int16:
	push    ebp
	mov     ebp, esp


	mov     esp, ebp
	pop     ebp
	ret

ascii_to_int32:
	push    ebp
	mov     ebp, esp


	mov     esp, ebp
	pop     ebp
	ret

print_int16:
	push    ebp
	mov     ebp, esp

	; TODO: converter para ascii e dar print

	mov     esp, ebp
	pop     ebp
	ret

print_int32:
	push    ebp
	mov     ebp, esp


	; TODO: converter para ascii e dar print

	mov     esp, ebp
	pop     ebp
	ret