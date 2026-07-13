section .data
str_ecerrar     db      "Encerrando calculadora", 0xA, 0x0
len_str_ecerrar equ     $ - str_ecerrar

str_continuar   db      0xa, "Pressione ENTER para continuar", 0x0
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
	global escolha_op_16bit.fim

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
	extern exp_int16


_start:
	call    saudacao
	call    pergunta_precisao					; salva precisao em [resposta_precisao] e retorna esse valor
	mov     [resposta_precisao], eax

loop_execucao:
	call    exibir_menu							; salva operacao em [opcao_menu] e retorna esse valor
	mov     [opcao_menu], eax

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
	call    mensagem_continuar

	jmp     loop_execucao

.end:
	call    encerrar_calculadora



; === FUNÇÕES AUXILIARES ===

encerrar_calculadora:
	push    len_str_ecerrar
	push    str_ecerrar
	call    print_string
	add     esp, 8

    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80

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
	push    eax									; salvo resultado na pilha
	call    print_int32							; chamo função pra dar print nele
	add     esp, 4
	mov     eax, 0x1							; flag em 1 para continuar exec

.fim:
	mov     esp, ebp
	pop     ebp
	ret

escolha_op_16bit:
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
	call    adicao_int16
	jmp     .resultado

.op_subtracao:
	call    subtracao_int16
	jmp     .resultado

.op_multiplicacao:
	call    multiplicacao_int16
	jmp     .resultado

.op_divisao:
	call    divisao_int16
	jmp     .resultado

.op_exponenciacao:
	call    exp_int16
	jmp     .resultado

.op_mod:
	call    mod_int16
	jmp     .resultado

.resultado:
	push    eax
	call    print_int16
	add     esp, 4
	mov     eax, 0x1

.fim:
	mov     esp, ebp
	pop     ebp
	ret


; === INPUT E OUTPUT ===
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

str_div0        db      "DIVISAO POR 0", 0xA, 0x0
len_str_div0    equ     $ - str_div0

str_resultado   db      "Resultado: ", 0x0
len_str_resultado equ     $ - str_resultado


; manu
str_m0          db      0xA, "Escolha uma opção:", 0xA
len_m0          equ     $ - str_m0
str_m1          db      "- 1: soma", 0xA
len_m1          equ     $ - str_m1
str_m2          db      "- 2: subtracao", 0xA
len_m2          equ     $ - str_m2
str_m3          db      "- 3: multiplicacao", 0xA
len_m3          equ     $ - str_m3
str_m4          db      "- 4: divisao", 0xA
len_m4          equ     $ - str_m4
str_m5          db      "- 5: exponenciacao", 0xA
len_m5          equ     $ - str_m5
str_m6          db      "- 6: mod", 0xA
len_m6          equ     $ - str_m6
str_m7          db      "- 7: sair", 0xA, 0xA
len_m7          equ     $ - str_m7

menu_array:
	dd      str_m0, len_m0
	dd      str_m1, len_m1
	dd      str_m2, len_m2
	dd      str_m3, len_m3
	dd      str_m4, len_m4
	dd      str_m5, len_m5
	dd      str_m6, len_m6
	dd      str_m7, len_m7



section .text

    global saudacao
	global pergunta_precisao
    global exibir_menu
	global aviso_overflow
	global aviso_div_zero

	global print_string
    global ler_string

	global ler_int32
    global print_int32
	global int32_to_ascii
	global ascii_to_int32

	global ler_int16
    global print_int16
	global int16_to_ascii
	global ascii_to_int16

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

; Receber o ponteiro do buffer para leitura pela pilha em [ebp+8]
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

	sub     esp, 8								; buffer local ("-32768\n" = 7 bytes)

    ; Exibe string de solicitação "Digite um inteiro: "
	push    len_str_digite_inteiro
	push    str_digite_inteiro
	call    print_string
	add     esp, 8

	mov     eax, 3
	mov     ebx, 0
	lea     ecx, [ebp - 8]
	mov     edx, 7								; limite de 7 bytes: "-32768\n" a "32767\n"
	int     0x80

	lea     ecx, [ebp - 8]
	push    ecx
	call    ascii_to_int16						; converte para inteiro
	add     esp, 4

    ; resultado final conversão em eax, só encerra
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

	sub     esp, 4

	push    len_str_tipo_inteiro
	push    str_tipo_inteiro
	call    print_string
	add     esp, 8

	lea     eax, [ebp - 4]						; alocação local
	push    eax
	call    ler_string
	add     esp, 4

	movzx   eax, byte [ebp - 4]					; lê o primeiro byte
	sub     eax, 48								; converte para inteiro

    ; TODO: verificar se entrada é valida -> 0 ou 1


	mov     esp, ebp
	pop     ebp
	ret

; retorna a opcao de operação escolhida pelo usuario
exibir_menu:
	push    ebp
	mov     ebp, esp

	sub     esp, 4								; aloca espaço pra leitura [ebp - 4]

	push    esi
	push    ecx

	mov     esi, menu_array
	mov     ecx, 8								; 8 linhas para imprimir

.imprimir_loop:
	push    ecx

	mov     eax, [esi + 4]						; comprimento
	push    eax
	mov     eax, [esi]							; string
	push    eax
	call    print_string
	add     esp, 8

	pop     ecx
	add     esi, 8								; avança para próxima string no array
	dec     ecx
	jnz     .imprimir_loop

	pop     ecx
	pop     esi

	lea     eax, [ebp - 4]						; carrega o endereço do buffer local
	push    eax
	call    ler_string
	add     esp, 4

	movzx   eax, byte [ebp - 4]
	sub     eax, 48

	mov     esp, ebp
	pop     ebp
	ret

mensagem_continuar:
	push    ebp
	mov     ebp, esp

	push    len_str_continuar
	push    str_continuar
	call    print_string
	add     esp, 8

	sub     esp, 32
	mov     eax, esp
	push    eax
	call    ler_string
	add     esp, 36

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

aviso_div_zero:
	push    ebp
	mov     ebp, esp

	push    len_str_div0
	push    str_div0
	call    print_string
	add     esp, 8

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

; Entrada: ponteiro para a string na stack [ebp+8]
; Saída:   eax, valor inteiro com sinal (cabe em 16 bits)
ascii_to_int16:
	push    ebp
	mov     ebp, esp
	push    ebx
	push    esi

	mov     esi, [ebp+8]
	xor     eax, eax
	xor     ebx, ebx
	xor     ecx, ecx

.ignora_espacos16:
	mov     bl, byte [esi]
	cmp     bl, ' '
	jne     .verifica_sinal16
	inc     esi
	jmp     .ignora_espacos16

.verifica_sinal16:
	cmp     bl, '-'
	jne     .verifica_positivo16
	mov     ecx, 1
	inc     esi
	jmp     .laco_conversao16

.verifica_positivo16:
	cmp     bl, '+'
	jne     .laco_conversao16
	inc     esi

.laco_conversao16:
	mov     bl, byte [esi]
	test    bl, bl
	jz      .fim_laco_conversao16
	cmp     bl, '0'
	jl      .fim_laco_conversao16
	cmp     bl, '9'
	jg      .fim_laco_conversao16

	sub     bl, '0'
	imul    eax, 10
	add     eax, ebx
	inc     esi
	jmp     .laco_conversao16

.fim_laco_conversao16:
	test    ecx, ecx
	jz      .fim_ascii_to_int16
	neg     eax

.fim_ascii_to_int16:
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

; Reaproveita int32_to_ascii: o algoritmo de conversão para texto e o mesmo
; independente da largura do inteiro, so muda a faixa de valores possiveis.
; Entrada:
;   [ebp+8]  - ponteiro para o buffer_saida
;   [ebp+12] - valor inteiro (cabe em 16 bits)
int16_to_ascii:
	push    ebp
	mov     ebp, esp

	push    dword [ebp+12]
	push    dword [ebp+8]
	call    int32_to_ascii
	add     esp, 8

	mov     esp, ebp
	pop     ebp
	ret


print_int32:
	push    ebp
	mov     ebp, esp

	sub     esp, 32								; aloca espaço para escrever o resultado


	push    len_str_resultado
	push    str_resultado
	call    print_string
	add     esp, 8

	mov     eax, [ebp+8]
	push    eax
	lea     eax, [ebp - 32]
	push    eax									; inteiro que foi lido em ascii
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


print_int16:
	push    ebp
	mov     ebp, esp

	sub     esp, 32								; aloca espaço para escrever o resultado

	push    len_str_resultado
	push    str_resultado
	call    print_string
	add     esp, 8

	mov     eax, [ebp + 8]
	push    eax									; guarda o resultado
	lea     eax, [ebp - 32]
	push    eax
	call    int16_to_ascii						; converte para ascii e retorna o número de bytes escritos
	add     esp, 8

	push    eax									; num bytes que foram escritos
	lea     eax, [ebp - 32]
	push    eax
	call    print_string
	add     esp, 8

	mov     esp, ebp
	pop     ebp
	ret