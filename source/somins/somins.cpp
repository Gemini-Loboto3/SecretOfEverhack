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

#if 0
	CreateDirectory(_T("font"), NULL);
	for (int i = 0; i < count; i++)
	{
		Image ch;
		GString name;
		ch.Create(width[i] + 1, 12, img.depth, img.palette);
		ch.BitBlit(&img, i % 16 * 16, i / 16 * 12, width[i], 12, 0, 0, Image::dir_normal);
		name.Format(_T("font\\%02X.bmp"), i + 0x10);
		ch.SaveBitmap(name);
	}
#endif
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

typedef struct tagAsmRep
{
	int index;
	u32 ptr, bank;
} ASM_REP;

void InsertMisc(u8 *rom, LPCTSTR text_name, LPCTSTR desc_name)
{
	typedef struct tagStuff
	{
		u16 ptr;
		u8 unk[6];
	} STUFF;

	ASM_REP rep[]=
	{
		// swords
		{ 20, 0xCD8C40, 0xCD8C45 },	// Bone Crusher
		{ 21, 0xCD8CB0, 0xCD8CB5 },	// Gladiator Sword
		{ 22, 0xCD8D20, 0xCD8D25 },	// Crusader Sword
		{ 23, 0xCD8D90, 0xCD8D95 },	// Neutron Blade
		// axes
		{ 24, 0xCD8E19, 0xCD8E1E },	// Spider's Claw
		{ 25, 0xCD8E89, 0xCD8E8E },	// Bronze Axe
		{ 26, 0xCD8EF9, 0xCD8EFE },	// Knight Basher
		{ 27, 0xCD8F69, 0xCD8F6E },	// Atom Smasher
		// spears
		{ 28, 0xCD8FF0, 0xCD8FF5 },	// Horn Spear
		{ 29, 0xCD9060, 0xCD9065 },	// Bronze Spear
		{ 30, 0xCD90D0, 0xCD90D5 },	// Lance
		{ 31, 0xCD9140, 0xCD9145 }	// Laser Lance
	};

	u16 *ptr = (u16*)&rom[0xE8000];
	CText txt(text_name);

	MEM_STREAM pp, tx;
	MemStreamCreate(&pp);
	MemStreamCreate(&tx);
	
	u32 base_ptr = 0;	// start from beginning of bank
	MemStreamSeek(&pp, txt.GetCount() * 2, SEEK_SET);

	for (int i = 0, si = txt.GetCount(); i < si; i++)
	{
		u8 *buf;
		STUFF *s = (STUFF*)&rom[0xE0000 + ptr[i]];

		s->ptr = base_ptr;
		int size = EncodeString(txt.GetString(i), buf);

		ptr[i] = pp.pos + 0x8000;	// update structure pointer
		for (int j = 0; j < _countof(rep); j++)
		{
			if (i == rep[j].index)
			{
				*(u16*)&rom[rep[j].ptr  -0xC00000 + 1] = base_ptr & 0xffff;
				*(u16*)&rom[rep[j].bank -0xC00000 + 1] = base_ptr >> 16;
			}
		}
		base_ptr += size;			// seek forward with text pointer
		MemStreamWrite(&pp, s, sizeof(*s));
		MemStreamWrite(&tx, buf, size);
		delete[] buf;
	}

	// write structure pointers
	MemStreamSeek(&pp, 0, SEEK_SET);
	MemStreamWrite(&pp, ptr, txt.GetCount() * 2);

	//MemStreamFlush(&pp, ptr_out);
	//MemStreamFlush(&tx, txt_out);
	MemStreamClose(&pp);
	MemStreamClose(&tx);
}

static LPCTSTR filenames[]=
{
#ifdef _DEBUG
	_T("..\\..\\rom.smc"),
	_T("..\\..\\script\\text.txt"),
	_T("..\\..\\hack\\text.bin"),
	_T("..\\..\\hack\\ptr.bin"),
	_T("..\\..\\hack\\font.png"),
	_T("..\\..\\hack\\font_small.png"),
	_T("..\\..\\script\\misc.txt")
#else
	_T("rom.smc"),
	_T("..\\script\\text.txt"),
	_T("text.bin"),
	_T("ptr.bin"),
	_T("font.png"),
	_T("font_small.png"),
	_T("..\\script\\misc.txt"),
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
	//InsertFont(filenames[5], &rom[0x40C84]);
	FlushFile(_T("rom.smc"), (u8*)f.GetBuffer(), f.GetSize());

	//InsertMisc(rom, filenames[6]);

    return 0;
}

