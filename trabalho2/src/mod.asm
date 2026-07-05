section .data
str_mod         db      "Fazendo mod", 0xA, 0x0
len_str_mod     equ     $ - str_mod

section .text
    global mod
	extern print_string

mod:
	push    ebp
	mov     ebp, esp

	push    len_str_mod
	push    str_mod
	call    print_string

	mov     esp, ebp
	pop     ebp
	ret