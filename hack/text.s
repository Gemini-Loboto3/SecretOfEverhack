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
	
// update pointers for: alchemy
org $CDB972
	LDA.w #(text_alchemysetup & 0xffff)
	STA.b $22
	LDA.w #(text_alchemysetup >> 16)

// update pointers for: Boy Stats
org $CDA281
	// print boy stats header
	LDY #$499
	LDA.w #(text_stats_title & 0xffff)
	STA.b $22
	LDA.w #(text_stats_title >> 16)
	STA.b $24
	JSL $CC9E03		// Display_mes
	LDY #$499
	LDA.w #$2210	// 7E2210
	STA.b $22
	LDA.w #$7E22
	STA.b $23
	JSL $CC9E03		// Display_mes
	BRA $CDA2C4		// jump back to old code
	// others
org $CDA2C7
	LDA.w #(text_stats_hp & 0xffff)
	STA.b $22
	LDA.w #(text_stats_hp >> 16)
org $CDA2DC
	LDA.w #(text_stats_level & 0xffff)
	STA.b $22
	LDA.w #(text_stats_level >> 16)
org $CDA2F1
	LDA.w #(text_stats_exp & 0xffff)
	STA.b $22
	LDA.w #(text_stats_exp >> 16)
org $CDA306
	LDA.w #(text_stats_next & 0xffff)
	STA.b $22
	LDA.w #(text_stats_next >> 16)
org $CDA575
	LDA.w #(text_stats_attack & 0xffff)
	STA.b $22
	LDA.w #(text_stats_attack >> 16)
org $CDA58A
	LDA.w #(text_stats_defend & 0xffff)
	STA.b $22
	LDA.w #(text_stats_defend >> 16)
org $CDA59F
	LDA.w #(text_stats_mdef & 0xffff)
	STA.b $22
	LDA.w #(text_stats_mdef >> 16)
org $CDA5B4
	LDA.w #(text_stats_evasion & 0xffff)
	STA.b $22
	LDA.w #(text_stats_evasion >> 16)
org $CDA5C9
	LDA.w #(text_stats_hit & 0xffff)
	STA.b $22
	LDA.w #(text_stats_hit >> 16)
// update pointers for: Dog Stats
org $CDAB2C
	// print dog stats header
	LDY #$499
	print "", pc, "..."
	LDA.w #(text_stats_title & 0xffff)
	STA.b $22
	print "", pc, "..."
	LDA.w #(text_stats_title >> 16)
	STA.b $24
	JSL $CC9E03		// Display_mes
	LDY #$499
	LDA.w #$2234	// 7E2234
	STA.b $22
	LDA.w #$7E22
	STA.b $23
	JSL $CC9E03		// Display_mes
	BRA $CDAB6F		// jump back to old code
	// others
org $CDAB72
	LDA.w #(text_stats_hp & 0xffff)
	STA.b $22
	LDA.w #(text_stats_hp >> 16)
org $CDAB87
	LDA.w #(text_stats_level & 0xffff)
	STA.b $22
	LDA.w #(text_stats_level >> 16)
org $CDAB9C
	LDA.w #(text_stats_exp & 0xffff)
	STA.b $22
	LDA.w #(text_stats_exp >> 16)
org $CDABB1
	LDA.w #(text_stats_next & 0xffff)
	STA.b $22
	LDA.w #(text_stats_next >> 16)
org $CDADE0
	LDA.w #(text_stats_atkskill & 0xffff)
	STA.b $22
	LDA.w #(text_stats_atkskill >> 16)
org $CDAE4B
	LDA.w #(text_stats_attack & 0xffff)
	STA.b $22
	LDA.w #(text_stats_attack >> 16)
org $CDAE60
	LDA.w #(text_stats_defend & 0xffff)
	STA.b $22
	LDA.w #(text_stats_defend >> 16)
org $CDAE75
	LDA.w #(text_stats_mdef & 0xffff)
	STA.b $22
	LDA.w #(text_stats_mdef >> 16)
org $CDAE8A
	LDA.w #(text_stats_evasion & 0xffff)
	STA.b $22
	LDA.w #(text_stats_evasion >> 16)
org $CDAE9F
	LDA.w #(text_stats_hit & 0xffff)
	STA.b $22
	LDA.w #(text_stats_hit >> 16)
	
// currencies
org $CF8704
	dl text_curr0, text_curr1, text_curr2, text_curr3
