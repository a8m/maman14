.entry K 
STR:    .string  "abcdef"
MAIN:    mov/0,1   w, r1
lea/1/1/1,2  STR(@4), r4
LOOP:    jmp/1/0/0,1  END
prn/1/1/0,1  w(@1)
sub/0,3  #-2, r1
.extern w
inc/0,1   r0
LENGTH:  .data  6
mov/0,1  r3,STR(@7)
bne/0,1  LOOP
END:    stop/0,1
.entry LENGTH
K:    .data  2
