// somext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\encoding.h"

void DumpText(u8 *rom, u8 *ptr, int count, LPCTSTR outname)
{
	CText txt;
	GString str;

	txt.Create(0, TYPE_LIST);
	for (int i = 0; i < count; i++, ptr += 3)
	{
		_tprintf(_T("%d\n"), i);
		if (i == 279)
			i = i;
		u32 p = (*(u16*)&ptr[0]) & 0x7fff;
		p |= (((*(u16*)&ptr[1] & 0x7f80) << 9));
		if (ptr[2] & 0x80)
			str = DecodeString(&rom[p]);
		else
			str = DecodeStringAscii(&rom[p]);
		txt.AddEntry(str, _T(""));
	}
	txt.Save(outname);
}

int main()
{
	CBufferFile f(_T("..\\..\\rom.smc"));
	u8 *rom = (u8*)f.GetBuffer();

	BuildDictionaries(rom);
	DumpText(rom, &rom[0x11D000], 2793 / 3, _T("script\\text_c0.txt"));
	DumpText(rom, &rom[0x11DAE9], 2645 / 3, _T("script\\text_c1.txt"));
	DumpText(rom, &rom[0x11E63E], 2394 / 3, _T("script\\text_c2.txt"));
	DumpText(rom, &rom[0x11EF98], 918  / 3, _T("script\\text_c3.txt"));

    return 0;
}

