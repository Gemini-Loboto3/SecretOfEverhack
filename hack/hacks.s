// script for xkas plus
arch snes.cpu; hirom
table "table.tbl"

// make font start from 0x10 instead of 0x20
org $CC9ED7
	SBC #$10
org $CCA54E
	SBC #$10
org $CCA5A3
	SBC #$10
org $CCA929
	SBC #$10
org $CCBB20
	SBC #$10	// name entry character table

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
	
incsrc text.s

org $D1D000
incbin "ptr.bin"
org $F00000
incbin "text.bin"

// expansion area for all extra data
align $10000

// game selection screen
// New Game
text_newgame:
	db "Nuova partita",0
// Save Game
text_savegame:
	db "Salva partita",0
// Empty
text_empty:
	db "Vuoto",0
// LV:
text_level:
	db "LV:",0
// HP:
text_hp:
	db "PS:",0
// Exp
text_exp:
	db "Esp",0

// load /save game screen
// Load this game?
text_loadthis:
	db "Confermi la scelta?",0
// Save over this game?
text_savethis:
	db "Confermi di salvare?",0
text_stats:
	db "Lv:",$0a,"Esp:",$0a,"PS:",0
text_yesno:
	db "S",$17,$0a,"No",0
	
// name entry screen
// Enter Character Name
text_name_title:
	db "Inserire nome personaggio",0
// Choose letters by moving the selector and pressing B.  Y deletes the last letter. Press START to exit.
text_name_desc:
	db "B inserisce un carattere e Y elimina l'ultimo carattere. Premi START per confermare.",0

// game over screen
// Sadly, <Boy> and <Dog> never came back.
text_gameover_mes:
	db $96,$92,"Purtroppo ",$80,$3D,$80,$81," e ",$82," non fecero mai pi",$1B," ritorno...",$80,$a5,$81,0
// his dog (replacement if there is no dog name set)
text_gameover_dog:
	db "il suo cane",0
text_gameover_tag:
	db $86,0

// misc text
// The Dog
text_thedog:
	db "Cane",0

org $3fffff
	db 0
