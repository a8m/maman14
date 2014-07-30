MAIN: mov/0,1 LENGTH, r1
lea/1/1/1,2 STR(@4), r4
LOOP: jmp/1/0/0,1 END
prn/1/1/0,1 STR(@1)
sub/0,3 #-2, r1
inc/0,1 r0
mov/0,1 r3, STR(@7)
bne/0,1 LOOP
END: stop/0,1
STR: .string "abcdef"
LENGTH: .data 6
K: .data 2