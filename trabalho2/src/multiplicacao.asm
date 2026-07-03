section .data
str_multiplicacao db      "Fazendo multiplicacao", 0xA, 0x0
len_str_multiplicacao equ     $ - str_multiplicacao

section .text
    global multiplicacao
	extern print_string

multiplicacao:
	push    ebp
	mov     ebp, esp

	push    len_str_multiplicacao
	push    str_multiplicacao
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret