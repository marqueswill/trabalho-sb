; int mod(int a, int b) -> eax = resto de (a / b)
section .text
    global mod_int32
    global mod_int16

mod_int32:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
    cdq
    idiv dword [ebp+12]        ; instrucao de divisao do IA-32 (exigida)
    mov eax, edx                ; idiv deixa o resto em EDX

    mov esp, ebp
    pop ebp
    ret

mod_int16:
    push ebp
    mov ebp, esp

    mov esp, ebp
    pop ebp
    ret