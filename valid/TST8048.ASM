	.LIST 8048.lst
	.CPU  8048
; test sample for xgen8048.c
; opcode results in numeric sequence
; no errors should result

	.ORG	0100

ZLOC	.EQU	$0012	;page 0 address
LOC1	.EQU	$0123	;page 1 address
LOC2	.EQU	$0234	;page 2 address
LOC3	.EQU	$0345	;page 3 address
LOC4	.EQU	$0456	;page 4 address
LOC5	.EQU	$0567	;page 5 address
LOC6	.EQU	$0678	;page 6 address
LOC7	.EQU	$0789	;page 7 address

BEGIN
	NOP			
	.DB	$01	; invalid opcode
	OUTL	BUS,A
	ADD	A,#ZLOC
	JMP	ZLOC
	EN	I
	.DB	$06	; invalid opcode
	DEC	A
	INS	A,BUS
	IN	A,P1
	IN	A,P2
	.DB	$0B	; invalid opcode
L1	MOVD	A,P4
	MOVD	A,P5
	MOVD	A,P6
	MOVD	A,P7
	INC	@R0
	INC	@R1
	JB0	L1
	ADDC	A,#ZLOC
	CALL	ZLOC
	DIS	I
	JTF	L1
	INC	A
	INC	R0
	INC	R1
	INC	R2
	INC	R3
	INC	R4
	INC	R5
	INC	R6
	INC	R7
	XCH	A,@R0
	XCH	A,@R1
	.DB	$22	; invalid opcode
	MOV	A,#ZLOC
	JMP	LOC1
	EN	TCNTI
	JNT0	L1
	CLR	A
	XCH	A,R0
	XCH	A,R1
	XCH	A,R2
	XCH	A,R3
	XCH	A,R4
	XCH	A,R5
	XCH	A,R6
	XCH	A,R7
	XCHD	A,@R0
	XCHD	A,@R1
	JB1	L1
	.DB	$33	; invalid opcode
	CALL	LOC1
	DIS	TCNTI
	JT0	L1
	CPL	A
	.DB	$38	; invalid opcode
	OUTL	P1,A
	OUTL	P2,A
	.DB	$3B	; invalid opcode
	MOVD	P4,A
	MOVD	P5,A
	MOVD	P6,A
	MOVD	P7,A
	ORL	A,@R0
	ORL	A,@R1
	MOV	A,T
	ORL	A,#ZLOC
	JMP	LOC2
	STRT	CNT
	JNT1	L1
	SWAP	A
	ORL	A,R0
	ORL	A,R1
	ORL	A,R2
	ORL	A,R3
	ORL	A,R4
	ORL	A,R5
	ORL	A,R6
	ORL	A,R7
	ANL	A,@R0
	ANL	A,@R1
	JB2	L1
	ANL	A,#ZLOC
	CALL	LOC2
	STRT	T
	JT1	L1
	DA	A
	ANL	A,R0
	ANL	A,R1
	ANL	A,R2
	ANL	A,R3
	ANL	A,R4
	ANL	A,R5
	ANL	A,R6
	ANL	A,R7
	ADD	A,@R0
	ADD	A,@R1
	MOV	T,A
	.DB	$63	; invalid opcode
	JMP	LOC3
	STOP	TCNT
	.DB	$66	; invalid opcode
	RRC	A
	ADD	A,R0
	ADD	A,R1
	ADD	A,R2
	ADD	A,R3
	ADD	A,R4
	ADD	A,R5
	ADD	A,R6
	ADD	A,R7
	ADDC	A,@R0
	ADDC	A,@R1
	JB3	L1
	.DB	$73	; invalid opcode
	CALL	LOC3
	ENT0	CLK
	JF1	L1
	RR	A
	ADDC	A,R0
	ADDC	A,R1
	ADDC	A,R2
	ADDC	A,R3
	ADDC	A,R4
	ADDC	A,R5
	ADDC	A,R6
	ADDC	A,R7
	MOVX	A,@R0
	MOVX	A,@R1
	.DB	$82	; invalid opcode
	RET			
	JMP	LOC4
	CLR	F0
	JNI	L1
	.DB	$87	; invalid opcode
	ORL	BUS,#ZLOC
	ORL	P1,#ZLOC
	ORL	P2,#ZLOC
	.DB	$8B	; invalid opcode
	ORLD	P4,A
	ORLD	P5,A
	ORLD	P6,A
	ORLD	P7,A
	MOVX	@R0,A
	MOVX	@R1,A
	JB4	L1
	RETR			
	CALL	LOC4
	CPL	F0
	JNZ	L1
	CLR	C
	.DB	$98	; invalid opcode
	ANL	P1,#50h
	ANL	P2,#50h
	.DB	$9B	; invalid opcode
	ANL	P4,A
	ANL	P5,A
	ANL	P6,A
	ANL	P7,A
	MOV	@R0,A
	MOV	@R1,A
	.DB	$A2	; invalid opcode
	MOVP	A,@A
	JMP	LOC5
	CLR	F1
	.DB	$A6	; invalid opcode
	CPL	C
	MOV	R0,A
	MOV	R1,A
	MOV	R2,A
	MOV	R3,A
	MOV	R4,A
	MOV	R5,A
	MOV	R6,A
	MOV	R7,A
	MOV	@R0,#ZLOC
	MOV	@R1,#ZLOC
	JB5	L1
	JMPP	@A
	CALL	LOC5
	CPL	F1
	JF0	L1
	.DB	$B7	; invalid opcode
	MOV	R0,#ZLOC
	MOV	R1,#ZLOC
	MOV	R2,#ZLOC
	MOV	R3,#ZLOC
	MOV	R4,#ZLOC
	MOV	R5,#ZLOC
	MOV	R6,#ZLOC
	MOV	R7,#ZLOC
	.DB	$C0	; invalid opcode
	.DB	$C1	; invalid opcode
	.DB	$C2	; invalid opcode
	.DB	$C3	; invalid opcode
	JMP	LOC6
	SEL	RB0
	JZ	L1
	MOV	A,PSW
	DEC	R0
	DEC	R1
	DEC	R2
	DEC	R3
	DEC	R4
	DEC	R5
	DEC	R6
	DEC	R7
	XRL	A,@R0
	XRL	A,@R1
	JB6	L1
	XRL	A,#ZLOC
	CALL	LOC6
	SEL	RB1
	.DB	$D6	; invalid opcode
	MOV	PSW,A
	XRL	A,R0
	XRL	A,R1
	XRL	A,R2
	XRL	A,R3
	XRL	A,R4
	XRL	A,R5
	XRL	A,R6
	XRL	A,R7
	.DB	$E0	; invalid opcode
	.DB	$E1	; invalid opcode
	.DB	$E2	; invalid opcode
	MOVP3	A,@A
	JMP	LOC7
	SEL	MB0
	JNC	L1
	RL	A
	DJNZ	R0,L1
	DJNZ	R1,L1
	DJNZ	R2,L1
	DJNZ	R3,L1
	DJNZ	R4,L1
	DJNZ	R5,L1
	DJNZ	R6,L1
	DJNZ	R7,L1
	MOV	A,@R0
	MOV	A,@R1
	JB7	L1
	.DB	$F3	; invalid opcode
	CALL	LOC7
	SEL	MB1
	JC	L1
	RLC	A
	MOV	A,R0
	MOV	A,R1
	MOV	A,R2
	MOV	A,R3
	MOV	A,R4
	MOV	A,R5
	MOV	A,R6
	MOV	A,R7

	.END


