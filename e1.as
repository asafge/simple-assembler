MAIN:	mov/0		LENGTH, r1
	lea/1/1/1	STR{LENGTH},r4
LOOP:	jmp/1/0/0	END
	prn/1/1/0	STR{r3}
	sub/0		#1, r1
	inc/0		r0
	mov/0		r3,STR{7}
	bne/0		LOOP
END:	stop/0
STR:	.string		"abcdef"
LENGTH:	.data		-6
K:	.data		2

