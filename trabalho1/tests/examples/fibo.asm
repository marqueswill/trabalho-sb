SECTION DATA
    ; Constantes:
    ZERO:   CONST 0
    ONE:    CONST 1

    ; Alocações:
    OLDER:  SPACE
    OLD:    SPACE
    NEW:    SPACE
    LIMIT:  SPACE

SECTION TEXT
    COPY ZERO,OLDER     ; older = 0
    COPY ONE,OLD     ; old = 1

    INPUT LIMIT

    ; print(old) -> 1
    OUTPUT OLD

    ; while(1)
    FRONT:                          
        ; new = older + old
        LOAD OLDER
        ADD OLD 
        STORE NEW

        ; if(new - limit > 0) then break
        ; mesma coisa que 
        ; if(new > limit) then break
        SUB LIMIT
        JMPP FINAL
        
        ; print(new)
        OUTPUT NEW
        
        ; older = old
        ; old = new
        COPY OLD,OLDER
        COPY NEW,OLD

        JMP FRONT

    FINAL: OUTPUT LIMIT ; se limit==new, vai printar o ultimo valor da sequencia 2 vezes
    STOP

