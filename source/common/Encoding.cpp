#include "Encoding.h"

// used as shuffled table
static TCHAR tbl_eng[]=
{
	_T(" eotanrishlu«dy\0")	// 00
	_T(".mcgÂf˚wpbv'Ik!,")	// 10
	_T("=T\nSYWÁH˘A?CÔENR")	// 20
	_T("OFBDMPGjqxLÙKÖ\t∆")	// 30
};

static TCHAR tbl_dte[64][3];

GStringArray dic1,	// Cxxxx
	dic2;	// C1-FF

void BuildDictionaries(u8 *rom)
{
	GString s;
	// alchemy, and, etc.
	u16 *ptr = (u16*)&rom[0x11F3EC];
	u8 *str = &rom[0x11F66C];

	for (int i = 1; i < 64; i++)
	{
		s.Set(&str[ptr[i]], GString::type_ascii);
		dic2.Add(s);
	}

	// about, across, etc.
	ptr = (u16*)&rom[0x11F46C];
	str = &rom[0x11F7D5];

	for (int i = 0; i < 256; i++)
	{
		s.Set(&str[ptr[i]], GString::type_ascii);
		dic1.Add(s);
	}

	str = &rom[0x11F32E];
	// dte
	for (int i = 0; i < _countof(tbl_dte); i++)
	{
		tbl_dte[i][0] = str[0];
		tbl_dte[i][1] = str[1];
		tbl_dte[i][2] = '\0';
		str += 2;
	}
}

GString DecodeString(u8 *data)
{
	GString str;

	for (;;)
	{
		u8 c = *data++;
		// shuffle order
		if (c < 0x40)
		{
			GString tmp;
			TCHAR ch = tbl_eng[c];
			switch (ch)
			{
			case '\0':
				str += _T("<End>");
				return str;
			case '\n':
				str += _T("\r\n");
				break;
			case '\t':
				str += _T("<Tab>");
				break;
			//case _T('«'):
				//tmp.Format(_T("<Delay %d>"), *data);
				//str += tmp;
				//data++;
				//break;
			default:
				str += ch;
			}
		}
		// ascii or special case
		else if (c >= 0x40 && c < 0x80)
		{
			// special case
			if (c == 0x41)
			{
				switch (*data)
				{
				case ':':
				case '-':
				case 'A':
				case 'V':
				case 'Q':
				case 'J':
				case 'U':
				case 'z':
				case '`':
					str += (TCHAR)*data;
					break;
				case 0x81:
					str += _T("<Boy>");
					break;
				case 0x82:
					str += _T("<Dog>");
					break;
				case 0x31:
				case 0x32:
				case 0x34:
				case 0x38:
				case 0xA1:
				case 0xA2:
					{
						GString tmp;
						tmp.Format(_T("<Var %02X>"), *data);
						str += tmp;
					}
					break;
				default:
					{
					GString tmp;
					tmp.Format(_T("<0x41><0x%02X>"), *data);
					str += tmp;
					}
				}
				data++;
			}
			// ascii
			else str += (TCHAR)c;
		}
		// dte
		else if (c >= 0x80 && c < 0xc0)
			str += tbl_dte[c - 0x80];
		// 16 bit dictionary (0xc0 0xXX)
		else if (c == 0xC0) str += dic1[*data++];
		// 8 bit dictionary (0xc1-0xff)
		else str += dic2[c - 0xC1];
	}

	return str;
}

GString DecodeStringAscii(u8 *data)
{
	GString str;

	while (1)
	{
		switch (*data)
		{
		case '\0':
			return str+_T("<End>");
		case '\n':
			str += _T("\r\n");
			break;
		default:
			str += (TCHAR)*data;
		}
		data++;
	}

	return str;
}

CEncoding::CEncoding()
{
}


CEncoding::~CEncoding()
{
}
