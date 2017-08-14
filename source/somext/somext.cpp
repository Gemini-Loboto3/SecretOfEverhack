// somext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\encoding.h"

void DumpText(u8 *rom, u8 *ptr, int count, LPCTSTR outname)
{
#if USE_XML
	CXmlText txt;
#else
	CText txt;
	txt.Create(0, TYPE_LIST);
#endif
	GString str;

	for (int i = 0; i < count; i++, ptr += 3)
	{
		if (i == 279)
			i = i;
		_tprintf(_T("%d\n"), i);
		u32 p = (*(u16*)&ptr[0]) & 0x7fff;
		p |= (((*(u16*)&ptr[1] & 0x7f80) << 9));
		if (ptr[2] & 0x80)
		{
#if USE_XML
			txt.type.push_back(0);
#endif
			str = DecodeString(&rom[p]);
		}
		else
		{
#if USE_XML
			txt.type.push_back(1);
#endif
			str = DecodeStringAscii(&rom[p]);
		}
#if USE_XML
		txt.mes[0].Add(str);
		txt.mes[1].Add(str);
#else
		txt.AddEntry(str, _T(""));
#endif
	}
	txt.Save(outname);
}

int main()
{
	CBufferFile f(_T("..\\..\\rom.smc"));
	u8 *rom = (u8*)f.GetBuffer();

	BuildDictionaries(rom);
#if USE_XML
	DumpText(rom, &rom[0x11d000], 3002, _T("script\\text.xml"));
#else
	DumpText(rom, &rom[0x11d000], 3002, _T("script\\text.txt"));
#endif

    return 0;
}

