;This is a COMMENT !
.entry Next
.extern wNumber
;This is a COMMENT !
:add $2
add: add $3,$5,$4
fake $4
add $a,$33,$1
STR: .asciz "aBcd"
MAIN: add $3,$5,$9
LOOP: ori $9,$9,$2
;This is a COMMENT !
la val1
;This is a COMMENT !
jmp Next
Next: move $20,$4
;This is a COMMENT !
LIST: .db 6,-9
;This is a COMMENT !
bgt $4, $2,END
;This is a COMMENT !
la K
;This is a COMMENT !
sw $0,4,$10
;This is a COMMENT !
call val1
;This is a COMMENT !
jmp $4
;This is a COMMENT !
la wNumber
;This is a COMMENT !
.extern val1
.dh 27056
;This is a COMMENT !
K: .dw 3121398749123,-12
;This is a COMMENT !
END: stop
;This is a COMMENT !
.entry K
