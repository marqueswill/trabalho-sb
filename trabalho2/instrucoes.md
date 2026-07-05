
push eax        ; Subtrai 4 de ESP e copia o valor de EAX para a posição [ESP]
pop ebx         ; Copia o valor que está em [ESP] para EBX e soma 4 em ESP


sub esp, 8      ; Reserva espaço para duas variáveis de 4 bytes (pilha cresce para baixo)
add esp, 8      ; Libera o espaço dessas duas variáveis (limpeza da pilha)