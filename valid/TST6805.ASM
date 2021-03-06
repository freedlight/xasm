	.LIST 6805.lst
	.CPU  6805
;:ts=8
; test sample for xgen6805.c
; opcode results in numeric sequence
; no errors should result

	.ORG $1000

ZBIT	.EQU	$02  	;SAMPLE BIT
ZLOC	.EQU	$0012	;SAMPLE ADDRESS IN PAGE ZERO
MLOC	.EQU	$3412	;SAMPLE ADDRESS NOT IN PAGE ZERO

BEGIN	BRSET	0,ZLOC,L1
	BRCLR	0,ZLOC,L1
L1	BRSET	1,ZLOC,L2
	BRCLR	1,ZLOC,L2
L2	BRSET	2,ZLOC,L3
	BRCLR	2,ZLOC,L3
L3	BRSET	3,ZLOC,L4
	BRCLR	3,ZLOC,L4
L4	BRSET	4,ZLOC,L5
	BRCLR	4,ZLOC,L5
L5	BRSET	5,ZLOC,L6
	BRCLR	5,ZLOC,L6
L6	BRSET	6,ZLOC,L7
	BRCLR	6,ZLOC,L7
L7	BRSET	7,ZLOC,L8
	BRCLR	7,ZLOC,L8
L8	BSET	0,ZLOC
	BCLR	0,ZLOC
	BSET	1,ZLOC
	BCLR	1,ZLOC
	BSET	2,ZLOC
	BCLR	2,ZLOC
	BSET	3,ZLOC
	BCLR	3,ZLOC
	BSET	4,ZLOC
	BCLR	4,ZLOC
	BSET	5,ZLOC
	BCLR	5,ZLOC
	BSET	6,ZLOC
	BCLR	6,ZLOC
L9	BSET	7,ZLOC
	BCLR	7,ZLOC
	BRA	L9
	BRN	L9
	BHI	L9
	BLS	L9
	BCC	L9
	BCS	L9
	BNE	L9
	BEQ	L9
	BHCC	L9
	BHCS	L9
	BPL	L9
	BMI	L9
	BMC	L9
	BMS	L9
	BIL	L9
	BIH	L9
	NEG	ZLOC
	COM	ZLOC
	LSR	ZLOC
	ROR	ZLOC
	ASR	ZLOC
	LSL	ZLOC
	ROL	ZLOC
	DEC	ZLOC
	INC	ZLOC
	TST	ZLOC
	CLR	ZLOC
	NEG	A
	COM	A
	LSR	A
	ROR	A
	ASR	A
	LSL	A
	ROL	A
	DEC	A
	INC	A
	TST	A
	CLR	A
	NEG	X
	COM	X
	LSR	X
	ROR	X
	ASR	X
	LSL	X
	ROL	X
	DEC	X
	INC	X
	TST	X
	CLR	X
	NEG	ZLOC,X
	COM	ZLOC,X
	LSR	ZLOC,X
	ROR	ZLOC,X
	ASR	ZLOC,X
	LSL	ZLOC,X
	ROL	ZLOC,X
	DEC	ZLOC,X
	INC	ZLOC,X
	TST	ZLOC,X
	CLR	ZLOC,X
	NEG	,X
	COM	,X
	LSR	,X
	ROR	,X
	ASR	,X
	LSL	,X
	ROL	,X
	DEC	,X
	INC	,X
	TST	,X
	CLR	,X
	RTI
	RTS
	SWI
	STOP
	WAIT
	TAX
	CLC
	SEC
	CLI
	SEI
	RSP
	NOP
	TXA
	SUB	#$01
	CMP	#$01
	SBC	#$01
	CPX	#$01
	AND	#$01
	BIT	#$01
LA	LDA	#$01
	EOR	#$01
	ADC	#$01
	ORA	#$01
	ADD	#$01
	BSR	LA
	LDX	#$01
	SUB	ZLOC
	CMP	ZLOC
	SBC	ZLOC
	CPX	ZLOC
	AND	ZLOC
	BIT	ZLOC
	LDA	ZLOC
	STA	ZLOC
	EOR	ZLOC
	ADC	ZLOC
	ORA	ZLOC
	ADD	ZLOC
	JMP	ZLOC
	JSR	ZLOC
	LDX	ZLOC
	STX	ZLOC
	SUB	MLOC
	CMP	MLOC
	SBC	MLOC
	CPX	MLOC
	AND	MLOC
	BIT	MLOC
	LDA	MLOC
	STA	MLOC
	EOR	MLOC
	ADC	MLOC
	ORA	MLOC
	ADD	MLOC
	JMP	MLOC
	JSR	MLOC
	LDX	MLOC
	STX	MLOC
	SUB	MLOC,X
	CMP	MLOC,X
	SBC	MLOC,X
	CPX	MLOC,X
	AND	MLOC,X
	BIT	MLOC,X
	LDA	MLOC,X
	STA	MLOC,X
	EOR	MLOC,X
	ADC	MLOC,X
	ORA	MLOC,X
	ADD	MLOC,X
	JMP	MLOC,X
	JSR	MLOC,X
	LDX	MLOC,X
	STX	MLOC,X
	SUB	ZLOC,X
	CMP	ZLOC,X
	SBC	ZLOC,X
	CPX	ZLOC,X
	AND	ZLOC,X
	BIT	ZLOC,X
	LDA	ZLOC,X
	STA	ZLOC,X
	EOR	ZLOC,X
	ADC	ZLOC,X
	ORA	ZLOC,X
	ADD	ZLOC,X
	JMP	ZLOC,X
	JSR	ZLOC,X
	LDX	ZLOC,X
	STX	ZLOC,X
	SUB	,X
	CMP	,X
	SBC	,X
	CPX	,X
	AND	,X
	BIT	,X
	LDA	,X
	STA	,X
	EOR	,X
	ADC	,X
	ORA	,X
	ADD	,X
	JMP	,X
	JSR	,X
	LDX	,X
	STX	,X

	.END

