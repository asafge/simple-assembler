  .entry   COUNT
  .extern  STRADD
  .extern  LEN
COUNT:  mov  STRADD[r3], r1
  cmp  #0, r1
  jmp  ENDCOUNT
  inc  LEN
  inc  r1
  bne  COUNT
ENDCOUNT: rts
