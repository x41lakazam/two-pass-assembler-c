;This is a COMMENT !
;This is a COMMENT !
STR: .asciz "XxxXxx32131"
MAIN: add $3,$5,$9
eh12312: add $2,$6,$10
LOOP: ori $9,-5,$2
;This is a COMMENT !
la val1
;This is a COMMENT !
jmp Next
Next: move $20,$4
;This is a COMMENT !
LIST: .db 6,-9
la K
;This is a COMMENT !
sw $0,4,$10
;This is a COMMENT !
la wNumber
;This is a COMMENT !
;This is a COMMENT !
bne $31,$9,LOOP
call val1
;This is a COMMENT !
jmp $4
.extern val1
.dh 27056
;This is a COMMENT !
K: .dw 31,-12
;This is a COMMENT !
.entry Next
.extern wNumber
END: stop
;This is a COMMENT !
.entry K
;This is a COMMENT !
bgt $4, $2,END
;This is a COMMENT !
