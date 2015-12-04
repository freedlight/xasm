	.LIST	6502.lst
	.SYMBOL	OFF
	.CPU	6502
;:ts=8
; test sample for xgen6502.c
; opcode results in numeric sequence
; no errors should result

	.ORG $9000

ZLOC	.EQU	$12	;SAMPLE ADDRESS IN PAGE ZERO
MLOC	.EQU	$3412	;SAMPLE ADDRESS NOT IN PAGE ZERO
HLOC	.DS	$10

	BRK
	ORA	(ZLOC,X)
 	.DB	$02		;no opcode
	.DB	$03		;no opcode
	.DB	$04		;no opcode
	ORA	ZLOC
	ASL	ZLOC
	.DB	$07		;no opcode
	PHP
	ORA	#01
	ASL	A
	.DB	$0B		;no opcode
	.DB	$0C		;no opcode
	ORA	MLOC
L1	ASL	MLOC
	.DB	$0F		;no opcode
	BPL	L1
	ORA	(ZLOC),Y
	.DB	$12		;no opcode
	.DB	$13		;no opcode
	.DB	$14		;no opcode
	ORA	ZLOC,X
	ASL	ZLOC,X
	.DB	$17		;no opcode
	CLC
	ORA	MLOC,Y
	.DB	$1A		;no opcode
	.DB	$1B		;no opcode
	.DB	$1C		;no opcode
	ORA	MLOC,X
	ASL	MLOC,X
	.DB	$1F		;no opcode
	JSR	L3
	AND	(ZLOC,X)
	.DB	$22		;no opcode
	.DB	$23		;no opcode
	BIT	ZLOC
	AND	ZLOC
	ROL	ZLOC
	.DB	$27		;no opcode
	PLP
	AND	#$01
	ROL	A
	.DB	$2B		;no opcode
L2	BIT	MLOC
	AND	MLOC
	ROL	MLOC
	.DB	$2F		;no opcode
L3	BMI	L2
	AND	(ZLOC),Y
	.DB	$32		;no opcode
	.DB	$33		;no opcode
	.DB	$34		;no opcode
	AND	ZLOC,X
	ROL	ZLOC,X
	.DB	$37		;no opcode
	SEC
	AND	MLOC,Y
	.DB	$3A		;no opcode
	.DB	$3B		;no opcode
	.DB	$3C		;no opcode
	AND	MLOC,X
	ROL	MLOC,X
	.DB	$3F		;no opcode
	RTI
	EOR	(ZLOC,X)
	.DB	$42		;no opcode
	.DB	$43		;no opcode
	.DB	$44		;no opcode
	EOR	ZLOC
	LSR	ZLOC
	.DB	$47		;no opcode
	PHA
	EOR	#01
	LSR	A
	.DB	$4B		;no opcode
	JMP	L3
	EOR	MLOC
	LSR	MLOC
	.DB	$4F		;no opcode
	BVC	L4
	EOR	(ZLOC),Y
	.DB	$52		;no opcode
	.DB	$53		;no opcode
	.DB	$54		;no opcode
L4	EOR	ZLOC,X
	LSR	ZLOC,X
	.DB	$57		;no opcode
	CLI
	EOR	MLOC,Y
	.DB	$5A		;no opcode
	.DB	$5B		;no opcode
	.DB	$5C		;no opcode
	EOR	MLOC,X
	LSR	MLOC,X
	.DB	$5F		;no opcode
	RTS
	ADC	(ZLOC,X)
	.DB	$62		;no opcode
	.DB	$63		;no opcode
	.DB	$64		;no opcode
	ADC	ZLOC
	ROR	ZLOC
	.DB	$67		;no opcode
	PLA
	ADC	#$01
	ROR	A
	.DB	$6B		;no opcode
	JMP	(MLOC)
	ADC	MLOC
	ROR	MLOC
	.DB	$6F		;no opcode
	BVS	L5
	ADC	(ZLOC),Y
	.DB	$72		;no opcode
	.DB	$73		;no opcode
	.DB	$74		;no opcode
L5	ADC	ZLOC,X
	ROR	ZLOC,X
	.DB	$77		;no opcode
	SEI
	ADC	MLOC,Y
	.DB	$7A		;no opcode
	.DB	$7B		;no opcode
	.DB	$7C		;no opcode
	ADC	MLOC,X
	ROR	MLOC,X
	.DB	$7F		;no opcode
	.DB	$80		;no opcode
	STA	(ZLOC,X)
	.DB	$82		;no opcode
	.DB	$83		;no opcode
	STY	ZLOC
	STA	ZLOC
	STX	ZLOC
	.DB	$87		;no opcode
	DEY
	.DB	$89		;no opcode
	TXA
	.DB	$8B		;no opcode
	STY	MLOC
	STA	MLOC
L6	STX	MLOC
	.DB	$8F		;no opcode
	BCC	L6
	STA	(ZLOC),Y
	.DB	$92		;no opcode
	.DB	$93		;no opcode
	STY	ZLOC,X
	STA	ZLOC,X
	STX	ZLOC,Y
	.DB	$97		;no opcode
	TYA
	STA	MLOC,Y
	TXS
	.DB	$9B		;no opcode
	.DB	$9C		;no opcode
	STA	MLOC,X
	.DB	$9E		;no opcode
	.DB	$9F		;no opcode
	LDY	#01
	LDA	(ZLOC,X)
	LDX	#01
	.DB	$A3		;no opcode
	LDY	ZLOC
	LDA	ZLOC
	LDX	ZLOC
	.DB	$A7		;no opcode
	TAY
	LDA	#01
	TAX
	.DB	$AB		;no opcode
	LDY	MLOC
	LDA	MLOC
	LDX	MLOC
	.DB	$AF		;no opcode
	BCS	L7
	LDA	(ZLOC),Y
	.DB	$B2		;no opcode
	.DB	$B3		;no opcode
L7	LDY	ZLOC,X
	LDA	ZLOC,X
	LDX	ZLOC,Y
	.DB	$B7		;no opcode
	CLV
	LDA	MLOC,Y
	TSX
	.DB	$BB		;no opcode
	LDY	MLOC,X
	LDA	MLOC,X
	LDX	MLOC,Y
	.DB	$BF		;no opcode
	CPY	#01
	CMP	(ZLOC,X)
	.DB	$C2		;no opcode
	.DB	$C3		;no opcode
	CPY	ZLOC
	CMP	ZLOC
	DEC	ZLOC
	.DB	$C7		;no opcode
	INY
	CMP	#01
	DEX
	.DB	$CB		;no opcode
	CPY	MLOC
	CMP	MLOC
	DEC	MLOC
	.DB	$CF		;no opcode
	BNE	L8
	CMP	(ZLOC),Y
	.DB	$D2		;no opcode
	.DB	$D3		;no opcode
	.DB	$D4		;no opcode
	CMP	ZLOC,X
	DEC	ZLOC,X
	.DB	$D7		;no opcode
L8	CLD
	CMP	MLOC,Y
	.DB	$DA		;no opcode
	.DB	$DB		;no opcode
	.DB	$DC		;no opcode
	CMP	MLOC,X
	DEC	MLOC,X
	.DB	$DF		;no opcode
	CPX	#$10
	SBC	(ZLOC,X)
	.DB	$E2		;no opcode
	.DB	$E3		;no opcode
	CPX	ZLOC
	SBC	ZLOC
	INC	ZLOC
	.DB	$E7		;no opcode
	INX
	SBC	#01
	NOP
	.DB	$EB		;no opcode
	CPX	MLOC
	SBC	MLOC
	INC	MLOC
	.DB	$EF		;no opcode
	BEQ	L9
	SBC	(ZLOC),Y
	.DB	$F2		;no opcode
	.DB	$F3		;no opcode
	.DB	$F4		;no opcode
	SBC	ZLOC,X
	INC	ZLOC,X
	.DB	$F7		;no opcode
L9	SED
	SBC	MLOC,Y
	.DB	$FA		;no opcode
	.DB	$FB		;no opcode
	.DB	$FC		;no opcode
	SBC	MLOC,X
	INC	MLOC,X
	.DB	$FF		;no opcode

	.END

