      INP
      BRZ ZEXIT // 0 -> 1
      SUB ONE
      BRZ ZEXIT // 1 -> 1
      ADD ONE
      STA ACC
FACT  STA N
      SUB ONE // N == 1?
      BRZ EXIT // y -> exit
      LDA N
      SUB TWO
      STA M
      LDA ZERO
      STA RES
MULT  LDA RES
      ADD ACC
      STA RES
      LDA M
      SUB ONE
      STA M
      BRP MULT
      LDA RES
      STA ACC
      LDA N
      SUB ONE
      BRP FACT
      BRA EXIT
EXIT  LDA RES
      OUT
      COB
ZEXIT LDA ONE
      STA RES
      BRA EXIT
ZERO  DAT
ONE   DAT 1
TWO   DAT 2
RES   DAT
ACC   DAT
N     DAT
M     DAT