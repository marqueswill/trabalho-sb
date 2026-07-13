section .text
    global div_int32
    global divisao_int16
    extern ler_int32
    extern ler_int16
    extern aviso_div_zero
    extern escolha_op_32bit.fim
    extern escolha_op_16bit.fim

div_int32:
    push    ebp
    mov     ebp, esp

    call    ler_int32
    push    eax

    call    ler_int32
    mov     ebx, eax

    cmp     ebx, 0                              ; Verifica divisão por zero
    je      .div_zero

    pop     eax
    cdq                                         ; estende sinal EAX -> EDX:EAX
    idiv    ebx

    mov     esp, ebp
    pop     ebp
    ret

.div_zero:
    call    aviso_div_zero                      ; Exibe mensagem de erro
    mov     esp, ebp
    pop     ebp
    pop     eax                                 ; retira o endereço de retorno (ret)
    
    mov     eax, 0x0                            ; flag pra parar exec
    jmp     escolha_op_32bit.fim


divisao_int16:
    push    ebp
    mov     ebp, esp

    call    ler_int16
    push    eax

    call    ler_int16
    mov     ebx, eax

    cmp     bx, 0                               ; Verifica divisão por zero em 16-bit
    je      .div_zero16

    pop     eax
    cwd                                         ; estende sinal AX -> DX:AX para 16-bit
    idiv    bx                                  ; divisao em 16-bit

    mov     esp, ebp
    pop     ebp
    ret

.div_zero16:
    call    aviso_div_zero
    mov     esp, ebp
    pop     ebp
    pop     eax                                 ; retira o endereço de retorno (ret)
    jmp     escolha_op_16bit.fim