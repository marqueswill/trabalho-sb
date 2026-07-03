section .data
str_adicao      db      "Fazendo adicao", 0xA, 0x0
len_str_adicao  equ     $ - str_adicao

section .text
    global adicao
	extern print_string



adicao:
	push    ebp
	mov     ebp, esp

	push    len_str_adicao
	push    str_adicao
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret