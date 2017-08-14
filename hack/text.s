Message_parse:
	PHB
	PEA 7F 7F
	PLB
	PLB
	DEY
	TDC
	SEP #$20
	BRA @@loop_start
@@loop_read:
	INC message
	BNE @@loop_start
	INC message+1
@@loop_start:
	LDA [message]
	BMI @@ck_dictionary
	ASL
	BMI @@negup
	INY
	LSR
	TAX
	LDA font_lookup,x
	STA $0000,y
	BNE @@loop_read
@@leave:
	REP #$20
	PLB
	RTL
@@negup:
	LSR
	AND #$3F
	TAX
	INC message
	BNE @@skip_bank
	INC message+1
	LDA [message]
	INY
	STA $0000,y
	DEX
	BNE @@copy
	CMP #0
	BNE @@loop_read
	BRA @@leave
@@ck_dictionary:
	ASL
	BMI @@dictionary
	INY
	TAX
	LDA dte,x
	STA $0000,y
	INY
	LDA dte+1,x
	STA $0000,y
	BEQ @@leave
	BRA @@loop_read
@@dictionary:
	AND #$7E
	BEQ @@loc_CCCE09
	TXA
	LDA #$91
	STA $28
	PLP
	REP #$20
	LDA $91F3EC,x
	CLC
	ADC #$F66C
	STA $26
	TDC
	SEP #$20
	BRA @@loc_CCCE02
@@loc_CCCDF8:
	INY
	STA $0000,y
	INC message_ptr
	BNE @@loc_CCCE02
	INC message_ptr+1
@@loc_CCCE02:
	LDA [message_ptr]
	BNE @@loc_CCCDF8
	JMP @@loop_read
@@loc_CCCE09:
	INC message
	BNE @@loc_CCCE0F
	INC message+1
@@loc_CCCE0F:
	LDA #$91
	STA $000028
	LDA [message]
	REP #$20
	ASL
	TAX
	LDA word_D1F46C,X
	CLC
	ADC #aAbout         ; "about"
	STA message_ptr
	TDC
	SEP #$20
	BRA @@loc_CCCE02
