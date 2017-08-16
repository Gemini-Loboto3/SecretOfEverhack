// somext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\encoding.h"

/*==================================================================*
 * Return width of a character inside a given font.					*
 *==================================================================*/
int GetCharWidth(Image *img, int _x, int _y, int w, int h)
{
	int maxw = 0;
	for (int y = 0; y < h; y++)
	{
		int curw = 0;
		for (int x = w; x > 0; x--)
		{
			if (img->GetPixelAt(_x + x - 1, _y + y))
			{
				curw = x;
				break;
			}
		}
		if (curw > maxw)
			maxw = curw;
	}
	return maxw;
}

/*==================================================================*
 * Generate a full width table for a given font picture.			*
 *==================================================================*/
void GetWidthTable(Image *img, u8 *table, int x1, int y1, int w1, int h1, int w, int h)
{
	for (int i = 0, y = y1; y < h1 + y1; y += h)
	{
		for (int x = x1; x < w1 + x1; x += w)
		{
			table[i] = GetCharWidth(img, x, y, w, h);
			i++;
		}
	}
}

void InsertFont(LPCTSTR in_name, u8* rom)
{
	const int count = 128;
	Image img;

	// obtain pointer to width table
	u8 *width = &rom[count * 12 * 2];

	img.LoadFromFile(in_name);

	// replace width table width new stuff
	GetWidthTable(&img, width, 0, 0, img.width, img.height, 16, 12);

	for (int i = 0; i < count; i++)
	{
		// left slice
		for (int y = 0; y < 12; y++)
		{
			u8 row = 0;
			for (int x = 0; x < 8; x++)
			{
				row <<= 1;
				row |= img.GetPixelAt(x + (i % 16 * 16), y + (i / 16 * 12)) ? 1 : 0;
			}
			*rom++ = row;
		}
		// right slice
		for (int y = 0; y < 12; y++)
		{
			u8 row = 0;
			for (int x = 0; x < 8; x++)
			{
				row <<= 1;
				row |= img.GetPixelAt(x + (i % 16 * 16) + 8, y + (i / 16 * 12)) ? 1 : 0;
			}
			*rom++ = row;
		}
	}
}

void InsertText(LPCTSTR in_name, MEM_STREAM *out_txt, MEM_STREAM *out_ptr, u32 &base_ptr)
{
#if USE_XML
	CXmlText txt;
#else
	CText txt;
#endif
	GString str;

	txt.Open(in_name);
	for (int i = 0, si = txt.GetCount(); i < si; i++)
	{
		u8 *buf;

		if (i == 2858)
			i = i;

		_tprintf(_T("Inserting dialog %03d/%03d at %06x\r"), i+1, si, base_ptr);

#if USE_XML
		int size = EncodeString(txt.GetString(MESS_ITA, i), buf);
#else
		int size = EncodeString(txt.GetString(i), buf);
#endif

		// adjust bank gap
		if ((base_ptr & 0xffff) + size >= 0x10000)
		{
			u32 pp = align(base_ptr, 0x10000);
			// write filler
			for (u32 j = 0; j < pp - base_ptr; j++)
				MemStreamWriteByte(out_txt, 0xff);
			base_ptr = pp;
		}
		// write data
		MemStreamWrite(out_ptr, &base_ptr, 3);
		MemStreamWrite(out_txt, buf, size);

		base_ptr += size;
		delete[] buf;
	}
	_tprintf(_T("\nDialog insertion done.\n"));
}

static LPCTSTR filenames[]=
{
#ifdef _DEBUG
	_T("..\\..\\rom.smc"),
	_T("..\\..\\script\\text.txt"),
	_T("..\\..\\hack\\text.bin"),
	_T("..\\..\\hack\\ptr.bin"),
	_T("..\\..\\hack\\font.png"),
	_T("..\\..\\hack\\font_small.png")
#else
	_T("rom.smc"),
	_T("..\\script\\text.txt"),
	_T("text.bin"),
	_T("ptr.bin"),
	_T("font.png"),
	_T("font_small.png")
#endif
};

int main()
{
	CBufferFile f(filenames[0]);
	u8 *rom = (u8*)f.GetBuffer();

	u32 base_ptr = 0xf00000;
	MEM_STREAM txt, ptr;
	MemStreamCreate(&txt);
	MemStreamCreate(&ptr);

#if USE_XML
	InsertText(_T("..\\..\\script\\text.xml"), &txt, &ptr, base_ptr);
#else
	InsertText(filenames[1], &txt, &ptr, base_ptr);
#endif

	MemStreamFlush(&txt, filenames[2]);
	MemStreamFlush(&ptr, filenames[3]);

	MemStreamClose(&txt);
	MemStreamClose(&ptr);

	InsertFont(filenames[4], &rom[0x40002]);
	InsertFont(filenames[5], &rom[0x40C84]);
	FlushFile(_T("rom.smc"), (u8*)f.GetBuffer(), f.GetSize());

    return 0;
}

