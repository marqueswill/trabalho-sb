SECTION TEXT
INPUT N       ; 12
LOAD N        ; 10
FAT: SUB ONE  ; 02
JMPN FIM      ; 06
STORE AUX     ; 11
MULT N        ; 03
STORE N       ; 11
LOAD AUX      ; 10
JMP FAT       ; 05
FIM: OUTPUT N ; 13
STOP          ; 14
SECTION DATA  ;
AUX: SPACE    ; 0
N: SPACE      ; 0
ONE: CONST 1  ; 1