%define TESTE 7
 
section .text
    global _start
    extern adicao
    extern subtracao
    extern multiplicacao
    extern divisao
    extern modulo
    extern exponenciacao
 
_start:
 
%if TESTE == 1
    push 3
    push 5
    call adicao
    add esp, 8
 
%elif TESTE == 2
    push 3
    push 5
    call subtracao
    add esp, 8
 
%elif TESTE == 3
    push 3
    push 5
    call multiplicacao
    add esp, 8
    ; edx = 1 se deu overflow (nao deveria dar aqui)
 
%elif TESTE == 4
    push 3
    push 10
    call divisao
    add esp, 8
 
%elif TESTE == 5
    push 3
    push 10
    call modulo
    add esp, 8
 
%elif TESTE == 6
    push 5
    push 2
    call exponenciacao
    add esp, 8
    ; edx = 1 se deu overflow (nao deveria dar aqui)
 
%elif TESTE == 7
    push 70000
    push 70000
    call multiplicacao
    add esp, 8
    cmp edx, 1
    jne .semOverflow
    mov eax, 99          ; codigo especial = overflow DETECTADO corretamente
    jmp .fimTeste
.semOverflow:
    mov eax, 1           ; se cair aqui, a deteccao de overflow FALHOU
.fimTeste:
 
%endif
 
    ; usa o resultado (EAX) como codigo de saida do processo
    mov ebx, eax
    mov eax, 1           ; sys_exit
    int 0x80