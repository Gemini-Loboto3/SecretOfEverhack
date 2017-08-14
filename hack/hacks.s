// script for xkas plus
arch snes.cpu; hirom

define message		$22
define message_ptr	$26

// disable compression and funky pointer calculations
// Get_mes0
org $CCCD01
	STA {message}
	INY
	db $B7,$26	//LDA [{message_ptr}], Y
	XBA
	AND.w #$00FF
	STA {message}+2
	RTL

// Get_mes1
org $CCCD3F
	STA {message}
	INY
	db $B7,$26	//LDA [{message_ptr}], Y
	XBA
	AND.w #$00FF
	STA {message}+2
	RTL

org $D1D000
incbin "..\source\somins\ptr.bin"
org $F00000
incbin "..\source\somins\text.bin"