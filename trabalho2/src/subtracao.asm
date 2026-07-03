section .data
str_subtracao   db      "Fazendo subtracao", 0xA, 0x0
len_str_subtracao equ     $ - str_subtracao

section .text
    global subtracao
	extern print_string

subtracao:
	push    ebp
	mov     ebp, esp

	push    len_str_subtracao
	push    str_subtracao
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret