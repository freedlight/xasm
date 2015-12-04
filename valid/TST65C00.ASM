	.LIST	65C00.lst
	.SYMBOL	OFF
	.CPU	65C00
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
 	MUL
	.DB	$03		;no opcode
 	.DB	$04		;no opcode
	ORA	ZLOC
	ASL	ZLOC
	RMB0	ZLOC
	PHP
	ORA	#01
	ASL	A
	.DB	$0B		;no opcode
	.DB	$0C		;no opcode
	ORA	MLOC
L1	ASL	MLOC
	BBR0	ZLOC,L1
	BPL	L1
	ORA	(ZLOC),Y
	.DB	$12		;no opcode
	.DB	$13		;no opcode
	.DB	$14		;no opcode
	ORA	ZLOC,X
	ASL	ZLOC,X
	RMB1	ZLOC
	CLC
	ORA	MLOC,Y
	.DB	$1A		;no opcode
	.DB	$1B		;no opcode
	.DB	$1C		;no opcode
	ORA	MLOC,X
	ASL	MLOC,X
	BBR1	ZLOC,L2
	JSR	L3
	AND	(ZLOC,X)
	.DB	$22		;no opcode
	.DB	$23		;no opcode
	BIT	ZLOC
	AND	ZLOC
	ROL	ZLOC
	RMB2	ZLOC
	PLP
	AND	#$01
	ROL	A
	.DB	$2B		;no opcode
L2	BIT	MLOC
	AND	MLOC
	ROL	MLOC
	BBR2	ZLOC,L2
L3	BMI	L2
	AND	(ZLOC),Y
	.DB	$32		;no opcode
	.DB	$33		;no opcode
	.DB	$34		;no opcode
	AND	ZLOC,X
	ROL	ZLOC,X
	RMB3	ZLOC
	SEC
	AND	MLOC,Y
	.DB	$3A		;no opcode
	.DB	$3B		;no opcode
	.DB	$3C		;no opcode
	AND	MLOC,X
	ROL	MLOC,X
	BBR3	ZLOC,L3
	RTI
	EOR	(ZLOC,X)
	.DB	$42		;no opcode
	.DB	$43		;no opcode
	.DB	$44		;no opcode
	EOR	ZLOC
	LSR	ZLOC
	RMB4	ZLOC
	PHA
	EOR	#01
	LSR	A
	.DB	$4B		;no opcode
	JMP	L3
	EOR	MLOC
	LSR	MLOC
	BBR4	ZLOC,L4
	BVC	L4
	EOR	(ZLOC),Y
	.DB	$52		;no opcode
	.DB	$53		;no opcode
	.DB	$54		;no opcode
L4	EOR	ZLOC,X
	LSR	ZLOC,X
	RMB5	ZLOC
	CLI
	EOR	MLOC,Y
	PHY
	.DB	$5B		;no opcode
	.DB	$5C		;no opcode
	EOR	MLOC,X
	LSR	MLOC,X
	BBR5	ZLOC,L4
	RTS
	ADC	(ZLOC,X)
	.DB	$62		;no opcode
	.DB	$63		;no opcode
	.DB	$64		;no opcode
	ADC	ZLOC
	ROR	ZLOC
	RMB6	ZLOC
	PLA
	ADC	#$01
	ROR	A
	.DB	$6B		;no opcode
	JMP	(MLOC)
	ADC	MLOC
	ROR	MLOC
	BBR6	ZLOC,L5
	BVS	L5
	ADC	(ZLOC),Y
	.DB	$72		;no opcode
	.DB	$73		;no opcode
	.DB	$74		;no opcode
L5	ADC	ZLOC,X
	ROR	ZLOC,X
	RMB7	ZLOC
	SEI
	ADC	MLOC,Y
	PLY
	.DB	$7B		;no opcode
	.DB	$7C		;no opcode
	ADC	MLOC,X
	ROR	MLOC,X
	BBR7	ZLOC,L5
	BRA	L5
	STA	(ZLOC,X)
	.DB	$82		;no opcode
	.DB	$83		;no opcode
	STY	ZLOC
	STA	ZLOC
	STX	ZLOC
	SMB0	ZLOC
	DEY
	.DB	$89		;no opcode
	TXA
	.DB	$8B		;no opcode
	STY	MLOC
	STA	MLOC
L6	STX	MLOC
	BBS0	ZLOC,L6
	BCC	L6
	STA	(ZLOC),Y
	.DB	$92		;no opcode
	.DB	$93		;no opcode
	STY	ZLOC,X
	STA	ZLOC,X
	STX	ZLOC,Y
	SMB1	ZLOC
	TYA
	STA	MLOC,Y
	TXS
	.DB	$9B		;no opcode
	.DB	$9C		;no opcode
	STA	MLOC,X
	.DB	$9E		;no opcode
	BBS1	ZLOC,L6
	LDY	#01
	LDA	(ZLOC,X)
	LDX	#01
	.DB	$A3		;no opcode
	LDY	ZLOC
	LDA	ZLOC
	LDX	ZLOC
	SMB2	ZLOC
	TAY
	LDA	#01
	TAX
	.DB	$AB		;no opcode
	LDY	MLOC
	LDA	MLOC
	LDX	MLOC
	BBS2	ZLOC,L7
	BCS	L7
	LDA	(ZLOC),Y
	.DB	$B2		;no opcode
	.DB	$B3		;no opcode
L7	LDY	ZLOC,X
	LDA	ZLOC,X
	LDX	ZLOC,Y
	SMB3	ZLOC
	CLV
	LDA	MLOC,Y
	TSX
	.DB	$BB		;no opcode
	LDY	MLOC,X
	LDA	MLOC,X
	LDX	MLOC,Y
	BBS3	ZLOC,L7
	CPY	#01
	CMP	(ZLOC,X)
	.DB	$C2		;no opcode
	.DB	$C3		;no opcode
	CPY	ZLOC
	CMP	ZLOC
	DEC	ZLOC
	SMB4	ZLOC
	INY
	CMP	#01
	DEX
	.DB	$CB		;no opcode
	CPY	MLOC
	CMP	MLOC
	DEC	MLOC
	BBS4	ZLOC,L8
	BNE	L8
	CMP	(ZLOC),Y
	.DB	$D2		;no opcode
	.DB	$D3		;no opcode
	.DB	$D4		;no opcode
	CMP	ZLOC,X
	DEC	ZLOC,X
	SMB5	ZLOC
L8	CLD
	CMP	MLOC,Y
	PHX
	.DB	$DB		;no opcode
	.DB	$DC		;no opcode
	CMP	MLOC,X
	DEC	MLOC,X
	BBS5	ZLOC,L8
	CPX	#$10
	SBC	(ZLOC,X)
	.DB	$E2		;no opcode
	.DB	$E3		;no opcode
	CPX	ZLOC
	SBC	ZLOC
	INC	ZLOC
	SMB6	ZLOC
	INX
	SBC	#01
	NOP
	.DB	$EB		;no opcode
	CPX	MLOC
	SBC	MLOC
	INC	MLOC
	BBS6	ZLOC,L9
	BEQ	L9
	SBC	(ZLOC),Y
	.DB	$F2		;no opcode
	.DB	$F3		;no opcode
	.DB	$F4		;no opcode
	SBC	ZLOC,X
	INC	ZLOC,X
	SMB7	ZLOC
L9	SED
	SBC	MLOC,Y
	PLX
	.DB	$FB		;no opcode
	.DB	$FC		;no opcode
	SBC	MLOC,X
	INC	MLOC,X
	BBS7	ZLOC,L9

	.END

