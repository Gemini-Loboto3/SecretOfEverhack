// change "The Dog" pointer
org $C08BDF
	LDA.w #<!inject 19>
	STA $26
	LDA.w #<@inject 19>
	STA $28

// update pointers for: name entry
org $CD8168
	LDA.w #<!inject 14>
	STA.b $22
	LDA.w #<@inject 14>
	STA.b $24
org $CD8240
	LDA.w #<!inject 15>
	STA.b $22
	LDA.w #<@inject 15>
	STA.b $24

// update pointers for: game selection
org $CDCBC6
	LDA.w #<!inject 4>
	STA.b $22
	LDA.w #<@inject 4>
org $CDCC12
	LDA.w #<!inject 6>
	STA.b $22
	LDA.w #<@inject 6>
org $CDCC55
	LDA.w #<!inject 7>
	STA.b $22
	LDA.w #<@inject 7>
org $CDCC8E
	LDA.w #<!inject 8>
	STA.b $22
	LDA.w #<@inject 8>
org $CDCCE0
	LDA.w #<!inject 9>
	STA.b $22
	LDA.w #<@inject 9>
org $CDD16F
	LDA.w #<!inject 5>
	STA.b $22
	LDA.w #<@inject 5>
org $CDD1B8
	LDA.w #<!inject 6>
	STA.b $22
	LDA.w #<@inject 6>
org $CDD1FB
	LDA.w #<!inject 7>
	STA.b $22
	LDA.w #<@inject 7>
org $CDD234
	LDA.w #<!inject 8>
	STA.b $22
	LDA.w #<@inject 8>
org $CDD286
	LDA.w #<!inject 9>
	STA.b $22
	LDA.w #<@inject 9>

// update pointers for: load / save game
org $CDCE25
	LDA.w #<!inject 10>
	STA.b $22
	LDA.w #<@inject 10>
org $CDD3D0
	LDA.w #<!inject 11>
	STA.b $22
	LDA.w #<@inject 11>
org $CDDD24
	LDA.w #<!inject 13>
	STA.b $22
	LDA.w #<@inject 13>		// bank is recycled from here on
org $CDDD35
	LDA.w #<!inject 12>
org $CDDD41
	LDA.w #<!inject 12>

// update pointers for: game over
org $CEE196
	LDA.w #<!inject 17>
	STA.b $26
	LDA.w #<@inject 17>
org $CEE2EE
	LDA.w #<!inject 16>
	STA.b $22
	LDA.w #<@inject 16>
org $CEE3A8
	LDA.w #<!inject 18>
	STA.b $22
	LDA.w #<@inject 18>
	
// update pointers for: alchemy
org $CDB972
	LDA.w #<!inject 31>
	STA.b $22
	LDA.w #<@inject 31>

// update pointers for: Boy Stats
org $CDA281
	// print boy stats header
	LDY #$499
	LDA.w #<!inject 20>
	STA.b $22
	LDA.w #<@inject 20>
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
	LDA.w #<!inject 20>
	STA.b $22
	LDA.w #<@inject 20>
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
	dl <#inject 0>, <#inject 1>, <#inject 2>, <#inject 3>
