// change "The Dog" pointer
org $C08BDF
	LDA.w #(text_thedog & 0xffff)
	STA $26
	LDA.w #(text_thedog >> 16)
	STA $28

// update pointers for: name entry
org $CD8168
	LDA.w #(text_name_title & 0xffff)
	STA.b $22
	LDA.w #(text_name_title >> 16)
	STA.b $24
org $CD8240
	LDA.w #(text_name_desc & 0xffff)
	STA.b $22
	LDA.w #(text_name_desc >> 16)
	STA.b $24

// update pointers for: game selection
org $CDCBC6
	LDA.w #(text_newgame & 0xffff)
	STA.b $22
	LDA.w #(text_newgame >> 16)
org $CDCC12
	LDA.w #(text_empty & 0xffff)
	STA.b $22
	LDA.w #(text_empty >> 16)
org $CDCC55
	LDA.w #(text_level & 0xffff)
	STA.b $22
	LDA.w #(text_level >> 16)
org $CDCC8E
	LDA.w #(text_hp & 0xffff)
	STA.b $22
	LDA.w #(text_hp >> 16)
org $CDCCE0
	LDA.w #(text_exp & 0xffff)
	STA.b $22
	LDA.w #(text_exp >> 16)
org $CDD16F
	LDA.w #(text_savegame & 0xffff)
	STA.b $22
	LDA.w #(text_savegame >> 16)
org $CDD1B8
	LDA.w #(text_empty & 0xffff)
	STA.b $22
	LDA.w #(text_empty >> 16)
org $CDD1FB
	LDA.w #(text_level & 0xffff)
	STA.b $22
	LDA.w #(text_level >> 16)
org $CDD234
	LDA.w #(text_hp & 0xffff)
	STA.b $22
	LDA.w #(text_hp >> 16)
org $CDD286
	LDA.w #(text_exp & 0xffff)
	STA.b $22
	LDA.w #(text_exp >> 16)

// update pointers for: load / save game
org $CDCE25
	LDA.w #(text_loadthis & 0xffff)
	STA.b $22
	LDA.w #(text_loadthis >> 16)
org $CDD3D0
	LDA.w #(text_savethis & 0xffff)
	STA.b $22
	LDA.w #(text_savethis >> 16)
org $CDDD24
	LDA.w #(text_yesno & 0xffff)
	STA.b $22
	LDA.w #(text_yesno >> 16)		// bank is recycled from here on
org $CDDD35
	LDA.w #(text_stats & 0xffff)
org $CDDD41
	LDA.w #(text_stats & 0xffff)

// update pointers for: game over
org $CEE196
	LDA.w #(text_gameover_dog & 0xffff)
	STA.b $26
	LDA.w #(text_gameover_dog >> 16)
org $CEE2EE
	LDA.w #(text_gameover_mes & 0xffff)
	STA.b $22
	LDA.w #(text_gameover_mes >> 16)
org $CEE3A8
	LDA.w #(text_gameover_tag & 0xffff)
	STA.b $22
	LDA.w #(text_gameover_tag >> 16)
