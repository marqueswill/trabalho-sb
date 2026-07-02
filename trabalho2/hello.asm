section .data
msg             db      "Olá, Mundo!", 0xA
len             equ     $ - msg

section .text
    global _start

_start:
    ; nasm -f elf32 hello.asm -o hello.o
    ; ld -m elf_i386 hello.o -o hello
    ; ./hello

    ; Syscall de escrita (sys_write)
	mov     eax, 4								; número da syscall para 32-bits (ou 1 para 64-bits usando rax/syscall)
	mov     ebx, 1								; stdout
	mov     ecx, msg							; ponteiro para a mensagem
	mov     edx, len							; tamanho da mensagem
	int     0x80								; chama o kernel

    ; Syscall de saída (sys_exit)
	mov     eax, 1								; número da syscall para sair
	xor     ebx, ebx							; código de retorno 0
	int     0x80