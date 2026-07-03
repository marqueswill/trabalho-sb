section .data
str_exponenciacao db      "Fazendo exponenciacao", 0xA, 0x0
len_str_exponenciacao equ     $ - str_exponenciacao

section .text
    global exponenciacao
	extern print_string



exponenciacao:
	push    ebp
	mov     ebp, esp


	push    len_str_exponenciacao
	push    str_exponenciacao
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret