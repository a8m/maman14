.entry  STRADD
.entry  MAIN
.extern REVERSE
.extern COUNT
.extern PRTSTR
.entry K
STRADD: .data 0
STR: .string "abcdef"
LASTCHAR: .data 0
LEN: .data 0
K: .data 0
STR: .string "abcdef"
MAIN:  lea/0,0 STR{!STR}, STRADD
inc/0,0 PRTSTR
mov/0,1 LOOP,r1
lea/1/1/1,0 STR{!STR},r4
LOOP: jmp/1/0/0,1 END
prn/1/1/0,1 LOOP{!STR}
sub/0,1 #-2,r1
.extern w
inc/0,1 r0
LENGTH: .data 6
mov/0,1 r3,STR{!STR}
bne/0,1 LOOP
END: stop/0,1
.entry LENGTH
K: .data 2
stop/0,0

