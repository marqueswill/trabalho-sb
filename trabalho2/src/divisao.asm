section .data
str_divisao      db      "Fazendo divisao", 0xA, 0x0
len_str_divisao  equ     $ - str_divisao

section .text
    global divisao
	extern print_string



divisao:
	push    ebp
	mov     ebp, esp

	push    len_str_divisao
	push    str_divisao
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret