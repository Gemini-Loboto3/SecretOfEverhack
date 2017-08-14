// somext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\encoding.h"

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

		_tprintf(_T("Inserting %03d at %06x\n"), i, base_ptr);

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
}

int main()
{
	CBufferFile f(_T("..\\..\\rom.smc"));
	u8 *rom = (u8*)f.GetBuffer();

	u32 base_ptr = 0xf00000;
	MEM_STREAM txt, ptr;
	MemStreamCreate(&txt);
	MemStreamCreate(&ptr);

#if USE_XML
	InsertText(_T("..\\..\\script\\text.xml"), &txt, &ptr, base_ptr);
#else
	InsertText(_T("..\\..\\script\\text.txt"), &txt, &ptr, base_ptr);
#endif

	MemStreamFlush(&txt, _T("text.bin"));
	MemStreamFlush(&ptr, _T("ptr.bin"));

	MemStreamClose(&txt);
	MemStreamClose(&ptr);

    return 0;
}

