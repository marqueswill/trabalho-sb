section .data

str_bemvindo    db      "Bem-vindo. Digite seu nome: ", 0x0
len_str_bemvindo equ     $ - str_bemvindo		; endereço atual - endereço final de `bemvindo`

str_saudacao1   db      "Hola, ", 0x0
len_str_saudacao1 equ     $ - str_saudacao1

str_saudacao2   db      ", bem-vindo ao programa de CALC IA-32.", 0xA, 0x0
len_str_saudacao2 equ     $ - str_saudacao2

str_tipo_inteiro db      "Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): ", 0xA, 0x0
len_str_tipo_inteiro equ     $ - str_tipo_inteiro

str_digite_inteiro db      "Digite um inteiro: ", 0x0
len_str_digite_inteiro equ     $ - str_digite_inteiro

str_overflow    db      "OCORREU OVERFLOW", 0xA, 0x0
len_str_overflow equ     $ - str_overflow

str_resultado   db      "Resultado: ", 0x0
len_str_resultado equ     $ - str_resultado

str_menu        db      0xA, "Escolha uma opção:", 0xA, \
	"- 1: soma",0xa,\
	"- 2: subtracao",0xa,\
	"- 3: multiplicacao",0xa,\
	"- 4: divisao",10,\
	"- 5: exponenciacao",0xa,\
	"- 6: mod",0xa,\
	"- 7: sair",0xa,0xa,0x0
len_str_menu    equ     $ - str_menu

; TODO: isso aqui não pode, refatorar tudo
section .bss


; global buffer_escrita
; buffer_leitura  resb    30
; buffer_escrita  resb    30
; tamanho_nome    resd    1

section .text
	extern nome_usuario
	extern resposta_precisao
	extern opcao_menu

    global saudacao
	global pergunta_precisao
    global exibir_menu
	global aviso_overflow

	global print_string
    global ler_string

	global ler_int32
    global print_int32
	global int32_to_ascii

	global ler_int16
    global print_int16
	global int16_to_ascii

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

; TODO: Receber o ponteiro do buffer_leitura pela pilha em [ebp+8]
ler_string:
	push    ebp
	mov     ebp, esp

	mov     eax, 3
	mov     ebx, 0
	mov     ecx, [ebp+8]
	mov     edx, 30								; Limite máximo de bytes a serem lidos
	int     0x80

	mov     ebx, eax
	dec     ebx

	mov     ecx, [ebp+8]
	mov     byte [ecx+ebx], 0

	dec     eax

	mov     esp, ebp
	pop     ebp
	ret

ler_int32:
	push    ebp
	mov     ebp, esp

	sub     esp, 16								; aloca espaço para leitura

	push    len_str_digite_inteiro
	push    str_digite_inteiro
	call    print_string
	add     esp, 8

	mov     eax, 3
	mov     ebx, 0
	lea     ecx, [ebp - 16]						; usa o espaço alocado
	mov     edx, 12								; limite de 12 bytes para 32 bits com sinal
	int     0x80

	lea     ecx, [ebp - 16]
	push    ecx
	call    ascii_to_int32						; converte para inteiro
	add     esp, 4								; resultado final conversão em eax, só encerra

	mov     esp, ebp
	pop     ebp
	ret

ler_int16:
	push    ebp
	mov     ebp, esp

    ; Exibe string de solicitação "Digite um inteiro: "
	push    len_str_digite_inteiro
	push    str_digite_inteiro
	call    print_string
	add     esp, 8

    ; Syscall sys_read
	mov     eax, 3
	mov     ebx, 0
	mov     ecx, [ebp + 8]						; usa o buffer local recebido pela pilha
	mov     edx, 7								; limite de 7 bytes para 32 bits com sinal
	int     0x80

	push    dword [ebp + 8]						; passa o buffer local para a conversão
	call    ascii_to_int32
	add     esp, 4

    ; resultado final conversão em ax, só encerra
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
	add     esp, 8

	push    nome_usuario
	call    ler_string							; retorna tamanho da resposta
	add     esp, 4

	push    eax									; salva antes da chamada

	push    len_str_saudacao1
	push    str_saudacao1
	call    print_string
	add     esp, 8

	push    nome_usuario
	call    print_string
	add     esp, 8

	push    len_str_saudacao2
	push    str_saudacao2
	call    print_string
	add     esp, 8

	mov     esp, ebp
	pop     ebp
	ret


pergunta_precisao:
	push    ebp
	mov     ebp, esp

	push    len_str_tipo_inteiro
	push    str_tipo_inteiro
	call    print_string
	add     esp, 8

	push    resposta_precisao
	call    ler_string
	add     esp, 4

	movzx   eax, byte [resposta_precisao]		; lê o primeiro byte
	sub     eax, 48								; converte para inteiro

    ; TODO: verificar se entrada é valida -> 0 ou 1

	mov     [resposta_precisao], eax

	mov     esp, ebp
	pop     ebp
	ret

; retorna a opcao de operação escolhida pelo usuario
; TODO: Quebrar a string do menu e imprimir iterativamente (linha por linha em loop ou chamadas sequenciais), conforme restrição arquitetural.
; TODO: deve receber um ponteiro para armazenar a seleção
exibir_menu:
	push    ebp
	mov     ebp, esp

	push    len_str_menu
	push    str_menu
	call    print_string
	add     esp, 8

	push    opcao_menu
	call    ler_string
	add     esp, 4

	movzx   eax, byte [opcao_menu]				; lê o primeiro byte
	sub     eax, 48								; converte para inteiro


    ; TODO: verificar se entrada é valida -> 1 a 7

	mov     [opcao_menu], eax

	mov     esp, ebp
	pop     ebp
	ret

aviso_overflow:
	push    ebp
	mov     ebp, esp

	push    len_str_overflow
	push    str_overflow
	call    print_string
	add     esp, 8

	mov     esp, ebp
	pop     ebp
	ret

ascii_to_int16:
	push    ebp
	mov     ebp, esp

loop_ascii_to_int16:
	; TODO: Implementar a lógica de conversão de ASCII para inteiro de 16 bits.
end_loop_ascii_to_int16:

	mov     esp, ebp
	pop     ebp
	ret

; Entrada: ponteiro para a string na stack [ebp+8]
; Saída:   eax, valor inteiro de 32 bits
ascii_to_int32:
	push    ebp
	mov     ebp, esp
	push    ebx
	push    esi

	mov     esi, [ebp+8]						; esi = ponteiro para a string
	xor     eax, eax							; eax = 0 (acumulador do resultado)
	xor     ebx, ebx							; ebx = 0 (armazena o caractere atual)
	xor     ecx, ecx							; ecx = 0 (flag de sinal: 0 = positivo, 1 = negativo)

.ignora_espacos:
	mov     bl, byte [esi]
	cmp     bl, ' '
	jne     .verifica_sinal
	inc     esi
	jmp     .ignora_espacos

.verifica_sinal:
	cmp     bl, '-'
	jne     .verifica_positivo
	mov     ecx, 1								; define flag de negativo
	inc     esi
	jmp     .laco_conversao

.verifica_positivo:
	cmp     bl, '+'
	jne     .laco_conversao
	inc     esi

.laco_conversao:
	mov     bl, byte [esi]
	test    bl, bl								; verifica fim da string (null terminator)
	jz      .fim_laco_conversao

	cmp     bl, '0'
	jl      .fim_laco_conversao					; finaliza se o caractere for menor que '0'
	cmp     bl, '9'
	jg      .fim_laco_conversao					; finaliza se o caractere for maior que '9'

	sub     bl, '0'								; converte de ASCII para valor numérico (0-9)

	imul    eax, 10								; multiplica o resultado acumulado por 10
	add     eax, ebx							; adiciona o novo dígito ao resultado

	inc     esi
	jmp     .laco_conversao

.fim_laco_conversao:
	test    ecx, ecx
	jz      .fim_ascii_to_int32
	neg     eax									; aplica o sinal negativo se a flag foi definida

.fim_ascii_to_int32:
	pop     esi
	pop     ebx

	mov     esp, ebp
	pop     ebp
	ret


; Entrada:
;   [ebp+8] - ponteiro para o buffer_saida
;   [ebp+12]  - valor inteiro de 32 bits
int32_to_ascii:
	push    ebp
	mov     ebp, esp
	push    ebx
	push    edi

	mov     edi, [ebp+8]						; edi = ponteiro para buffer_saida
	mov     eax, [ebp+12]						; eax = valor a ser convertido

	test    eax, eax
	jns     .prepara_divisao					; se o sinal for positivo (SF=0), pula

	mov     byte [edi], '-'						; insere o caractere '-' no buffer
	inc     edi									; avança o ponteiro do buffer
	neg     eax									; converte o valor negativo em positivo (complemento de 2)

.prepara_divisao:
	mov     ebx, 10								; divisor constante = 10
	xor     ecx, ecx							; ecx = 0 (contador de dígitos empilhados)

	; 2. Extração dos dígitos (ordem inversa)
.divide_loop:
	xor     edx, edx							; zera edx pois a instrução div usa edx:eax
	div     ebx									; eax = quociente, edx = resto (dígito atual)
	push    edx									; salva o dígito na pilha
	inc     ecx									; incrementa o contador de dígitos

	test    eax, eax							; verifica se o quociente é 0
	jnz     .divide_loop						; se não for, continua dividindo

.escreve_loop:
	pop     edx									; recupera o último dígito empilhado
	add     dl, '0'								; converte o valor numérico (0-9) para ASCII ('0'-'9')
	mov     [edi], dl							; escreve o caractere no buffer
	inc     edi									; avança o ponteiro do buffer

	dec     ecx									; decrementa o contador de dígitos
	jnz     .escreve_loop						; continua até escrever todos os dígitos

.fim_int32_to_ascii:
	mov     byte [edi], 0						; adiciona o caractere nulo (null-terminator) ao final

	mov     eax, edi							; eax recebe o endereço atual (final) do buffer
	sub     eax, [ebp+8]						; subtrai o endereço inicial recebido na stack

	pop     edi
	pop     ebx
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

	sub     esp, 32								; aloca espaço para escrever o resultado
	push    eax									; guarda o resultado

	push    len_str_resultado
	push    str_resultado
	call    print_string
	add     esp, 8

	lea     eax, [ebp - 32]
	push    eax
	call    int32_to_ascii						; converte para int para ascii e retorna o número de bytes em buffer_escrita
	add     esp, 8

	push    eax									; num bytes que foram escritos
	lea     eax, [ebp - 32]
	push    eax
	call    print_string
	add     esp, 8

	mov     esp, ebp
	pop     ebp
	ret