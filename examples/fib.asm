        INP
        STA N
LOOP    LDA A
        SUB N
        BRP ENDLOOP
        LDA A
        OUT
        LDA B
        ADD A
        STA ACC
        LDA B
        STA A
        LDA ACC
        STA B
        BRA LOOP
ENDLOOP HLT
A       DAT 0
B       DAT 1
N       DAT
ACC     DAT