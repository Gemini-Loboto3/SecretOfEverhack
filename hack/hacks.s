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
incbin "misc_txt.bin"

// talons
text_curr0:
	db "artigli",0
// jewels
text_curr1:
	db "gioielli",0
// gold pieces
text_curr2:
	db "monete",0
// credits
text_curr3:
	db "crediti",0

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
	db "Lv:",0
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
	
// <Boy>'s Stats
// <Boy>' Stats (applies when name ends with an s)
text_stats_title:
	db "Status/",0
// HP:
text_stats_hp:
	db "PS:",0
// Level:
text_stats_level:
	db "Livello:",0
// Exp:
text_stats_exp:
	db "Esp:",0
// Needed:
text_stats_next:
	db "Prossimo:",0
// Attack:
text_stats_attack:
	db "Attacco:",0
// Hit %:
text_stats_hit:
	db "Precisione %:",0
// Defend:
text_stats_defend:
	db "Difesa:",0
// Evade %:
text_stats_evasion:
	db "Evasione %:",0
// Magic Def:
text_stats_mdef:
	db "Dif. magica:",0
// Attack Skill:
text_stats_atkskill:
	db "Abilit",$11," ATT:",0
// 

// alchemy screens
// Alchemy Setup
text_alchemysetup:
	db "Impostazioni alchimia",0

org $3fffff
	db 0
