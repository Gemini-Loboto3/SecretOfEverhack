// somext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\encoding.h"

void DumpMisc(u8 *rom, LPCTSTR out_name)
{
	typedef struct tagStuff
	{
		u16 ptr;
		u8 unk[8];
	} STUFF;

	u16 *ptr = (u16*)&rom[0xE8000];

	CText txt;
	txt.Create(0, TYPE_LIST);
	for (int i = 0; i < 162; i++)
	{
		STUFF *s = (STUFF*)&rom[0xE0000 + ptr[i]];
		GString str = DecodeStringAscii(&rom[0x40000 + s->ptr]);
		txt.AddEntry(str, _T(""));
	}
	txt.Save(out_name);

	txt.Create(0, TYPE_LIST);
	ptr = (u16*)&rom[0x459FA];
	for (int i = 0; i < 12; i++)
	{
		GString str = DecodeStringAscii(&rom[0x40000 + ptr[i]]);
		txt.AddEntry(str, _T(""));
	}
	txt.Save(_T("script\\weapon_desc.txt"));
}

void DumpFont(u8 *font, LPCTSTR out_name)
{
#if IS_EURO
	const int count = 160;
#else
	const int count = 128;
#endif
	Image img;

	RGBQUAD pal[16];
	*(u32*)&pal[0] = 0xff000000;
	*(u32*)&pal[1] = 0xffffffff;
	img.Create(256, count / 16 * 12, 4, pal);

	for (int i = 0; i < count; i++)
	{
		// left rows
		for (int y = 0; y < 12; y++)
		{
			u8 row = *font++;
			for (int x = 0; x < 8; x++, row <<= 1)
				img.SetPixelAt(x + (i % 16 * 16), y + (i / 16 * 12), row & 0x80 ? 1 : 0);
		}
		// right rows
		for (int y = 0; y < 12; y++)
		{
			u8 row = *font++;
			for (int x = 0; x < 8; x++, row <<= 1)
				img.SetPixelAt(x + (i % 16 * 16) + 8, y + (i / 16 * 12), row & 0x80 ? 1 : 0);
		}
	}

	img.SaveBitmap(out_name);
}

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
#if !IS_EURO
	CBufferFile f(_T("..\\..\\rom.smc"));
	u8 *rom = (u8*)f.GetBuffer();

	BuildDictionaries(rom);
#if USE_XML
	DumpText(rom, &rom[0x11d000], 3002, _T("script\\text.xml"));
#else
	DumpText(rom, &rom[0x11d000], 3002, _T("script\\text.txt"));
#endif

	DumpMisc(rom, _T("script\\misc.txt"));

	DumpFont(&rom[0x40002], _T("font.bmp"));
	DumpFont(&rom[0x40C84], _T("font_small.bmp"));
#else
	CBufferFile f(_T("..\\..\\..\\Secret of Evermore (France).sfc"));
	u8 *rom = (u8*)f.GetBuffer();

	DumpFont(&rom[0x30002], _T("font_fr.bmp"));
#endif

    return 0;
}

