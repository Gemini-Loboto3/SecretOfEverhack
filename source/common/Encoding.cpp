#include "Encoding.h"

// used as shuffled table
static TCHAR tbl_eng[64];
static u8 tbl_dte[64][2];	// 80-BF

GStringArray dic1,	// Cxxxx
	dic2;		// C1-FF

GString ParseAscii(u8 *&ch)
{
	GString ret, t;
	if (*ch >= 0x80)
	{
		switch (*ch)
		{
		case 0x80:	// special shit
			ch++;
			ret = _T("<S>");
			while (*ch != 0x80)
			{
				t.Format(_T("<$%02X>"), *ch);
				ret += t;
				ch++;
			}
			ret += _T("</S>");
			ch++;
			break;
		case 0x86:
			t.Format(_T("<Delay %d>"), *ch | (*ch << 8));
			//ret = _T("<New>\r\n");
			ret += t;
			ch += 3;
			break;
		default:
			ret.Format(_T("<$%02X>"), *ch);
			ch++;
		}
	}
	else
	{
		ret = (TCHAR)*ch;
		ch++;
	}

	return ret;
}

GString Str2Str(u8 ch)
{
	GString ret;
	if (ch >= 0x80)
	{
		switch (ch)
		{
		case 0x86:
			ret = _T("<New>\r\n");
			break;
		default:
			ret = (TCHAR)ch;
		}
	}
	else ret = (TCHAR)ch;

	return ret;
}

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
	for (int i = 0; i < 64; i++)
	{
		tbl_dte[i][0] = str[0];
		tbl_dte[i][1] = str[1];
		str += 2;
	}

	// scatter table
	str = &rom[0x11F3AE];
	for (int i = 0; i < 64; i++)
	{
		// replace weird crap symbols
		/*if (str[i] == '_') tbl_eng[i] = _T('…');
		else*/ tbl_eng[i] = (TCHAR)str[i];
	}
}

void ExpandString(u8 *data, MEM_STREAM &str)
{
	MemStreamCreate(&str);

	while (1)
	{
		u8 c = *data++;
		// shuffle order
		if (c < 0x40)
		{
			TCHAR ch = tbl_eng[c];

			MemStreamWriteByte(&str, (u8)ch);
			if (ch == 0)
			{
				//MemStreamFlush(&str, _T("test.bin"));
				return;
			}
		}
		// copy chars to buffer
		else if (c >= 0x40 && c < 0x80)
		{
			int size = c & 0x3f;
			for (int i = 0; i < size; i++)
				MemStreamWriteByte(&str, *data++);
		}
		// dte
		else if (c >= 0x80 && c < 0xc0)
		{
			MemStreamWriteByte(&str, (u8)tbl_dte[c - 0x80][0]);
			MemStreamWriteByte(&str, (u8)tbl_dte[c - 0x80][1]);
		}
		// 16 bit dictionary (0xc0 0xXX)
		else if (c == 0xC0)
		{
			TCHAR *t = dic1[*data].GetBuffer();
			while (*t != 0)
				MemStreamWriteByte(&str, (u8)*t++);
			data++;
		}
		// 8 bit dictionary (0xc1-0xff)
		else
		{
			TCHAR *t = dic2[c - 0xC1].GetBuffer();
			while (*t != 0)
				MemStreamWriteByte(&str, (u8)*t++);
		}
	}

}

GString DecodeString(u8 *data)
{
	GString str, tmp;
	MEM_STREAM dec;

	ExpandString(data, dec);
	data = dec.data;

	for (;;)
	{
		u8 c = *data++;

		// characters
		if (c < 0x80)
		{
#if USE_XML
			if (c == '\0') return str;
#else
			if (c == '\0') return str + _T("<End>");
#endif
			else if (c == '_')
				str += _T('…');
			else if (c == '\n')
				str += _T("\r\n");
			else if (c == '\v')
				str += _T("<VTab>");
			else if (c == '<')
				str += _T('«');
			else if (c == '>')
				str += _T('»');
			//{
			//	tmp.Format(_T("<Autoclose %X>"), *data | (data[1])<<8);
			//	return str + tmp;
			//	//str += _T("<New>\r\n");
			//}
			else str += (TCHAR)c;
		}
		// special symbols
		else
		{
			switch (c)
			{
			case 0x80:	// needs to be documented better
				str += _T("<S");
				for (int i = 0; ; i++)
				{
					if (i >= 1 && (*data == 0x80 || *data == 0x81 || *data == 0x82))
						break;
					tmp.Format(_T(" $%02X"), *data);
					str += tmp;
					data++;
				}
				tmp.Format(_T(" $%02X>"), *data);
				str += tmp;
				data++;
				break;
			case 0x81:
				str += _T("<Boy>");
				break;
			case 0x82:
				str += _T("<Dog>");
				break;
			case 0x83:
				str += _T("<Player3>");
				break;
			case 0x84:
				str += _T("<Player4>");
				break;
			case 0x8B:
				str += _T("<Choice>");
				break;
			case 0xA1:
				str += _T("<Count>");
				break;
			case 0xA2:
				str += _T("<Item>");
				break;
			default:
				tmp.Format(_T("<$%02X>"), c);
				str += tmp;
			}
		}
	}

	return str;
}

GString DecodeStringAscii(u8 *data)
{
	GString str;

	while (1)
	{
		if (*data >= 0x80)
		{
			GString tmp;
			tmp.Format(_T("<$%02X>"), *data);
			str += tmp;
		}
		else
		{
			switch (*data)
			{
			case '\0':
#if USE_XML
				return str;
#else
				return str + _T("<End>");
#endif
			case '\n':
				str += _T("\r\n");
				break;
			default:
				str += (TCHAR)*data;
			}
		}
		data++;
	}

	return str;
}

static TCHAR sym_table[] =
{
	_T("@@@@@@@@àèéìòùÈÉ")		// 10
	_T(" !\"#$%&\'()*+,-./")	// 20
	_T("0123456789:;«=»?")		// 30
	_T("@ABCDEFGHIJKLMNO")		// 40
	_T("PQRSTUVWXYZ[\\]▶…")		// 50
	_T("`abcdefghijklmno")		// 60
	_T("pqrstuvwxyz{|}~ ")		// 70
};

u8 Find_symbol(TCHAR c)
{
	for (int i = 0; i < sizeof(sym_table); i++)
		if (c == sym_table[i])
			return i + 0x10;

	// couldn't find anything
	return '?';
}

int EncodeString(GString str, u8 *&dest)
{
	MEM_STREAM s;
	MemStreamCreate(&s);

	for (int i = 0, si = str.GetLength(); i < si;)
	{
		TCHAR c = str[i++];

		// it's a tag
		if (c == _T('<'))
		{
			GString tag;
			int h0, h1;

			while (str[i] != _T('>') && str[i] >= ' ')
				tag += str[i++];
			if(str[i] == '>') i++;

#if !USE_XML
			// <End>
			if (tag.Compare(_T("End")) == 0)
			{
				MemStreamWriteByte(&s, 0);
				dest = s.data;
				return s.size;
			}
#endif
			if (tag.Compare(_T("Choice")) == 0)
				MemStreamWriteByte(&s, 0x8b);
			else if (tag.Compare(_T("Boy")) == 0)
				MemStreamWriteByte(&s, 0x81);
			else if (tag.Compare(_T("Dog")) == 0)
				MemStreamWriteByte(&s, 0x82);
			else if (tag.Compare(_T("Player3")) == 0)
				MemStreamWriteByte(&s, 0x83);
			else if (tag.Compare(_T("Player4")) == 0)
				MemStreamWriteByte(&s, 0x84);
			else if (tag.Compare(_T("Count")) == 0)
				MemStreamWriteByte(&s, 0xa1);
			else if (tag.Compare(_T("Item")) == 0)
				MemStreamWriteByte(&s, 0xa2);
			// <$xx>
			else if (_stscanf((LPCTSTR)tag, _T("$%02X"), &h0) == 1)
				MemStreamWriteByte(&s, h0 & 0xff);
			// <S $xx $xx>
			else if (_stscanf((LPCTSTR)tag, _T("S $%02X $%02X"), &h0, &h1))
			{
				MemStreamWriteByte(&s, 0x80);
				MemStreamWriteByte(&s, h0 & 0xff);
				MemStreamWriteByte(&s, h1 & 0xff);
			}
			else if (tag.Compare(_T("New")) == 0)
			{
				MemStreamWriteByte(&s, '<');
				if (str[i] == '\r') i++;
				if (str[i] == '\n') i++;
			}
			else
				_tprintf(_T("Uknown tag %s\n"), (LPCTSTR)tag);
		}
		// something else entirely
		else
		{
			if (c == '\r' || c == '\n')
			{
				MemStreamWriteByte(&s, '\n');
				// skip any extra line carries
				if (str[i] == '\n') i++;
			}
			else MemStreamWriteByte(&s, Find_symbol(c));
		}
	}

	MemStreamWriteByte(&s, 0);

	dest = s.data;
	return s.size;
}

#if USE_XML
void CXmlText::Open(LPCTSTR filename)
{
	TCHAR *buffer;
	CMarkup xml;
	StoreUtf8Text(filename, buffer);
	xml.SetDoc(buffer);
	delete[] buffer;

	if (!xml.FindElem(_T("Text"))) return;

	Reset();

	xml.IntoElem();
	while (xml.FindElem(_T("Entry")))
	{
		if (xml.GetAttrib(_T("type")).Compare(_T("0")) == 0)
			type.push_back(0);
		else type.push_back(1);

		xml.IntoElem();

		// reserve a new entry for all strings
		for (int i = 0; i < LANGUAGE_MAX; i++)
			mes[i].Add(_T(""));
		int cnt = mes[0].GetCount() - 1;

		while (xml.FindElem(_T("String")))
		{

			GString id = xml.GetAttrib(_T("id"));
			id.MakeUpper();
			int idx;

			if (id.Compare(_T("ENG")) == 0)
				idx = MESS_ENG;
			else if (id.Compare(_T("ITA")) == 0)
				idx = MESS_ITA;

			GString temp = xml.GetData();
			temp.Replace(_T("\\n"), _T("\r\n"));
			mes[idx][cnt] = temp;
		}

		// if entries are empty, copy them from english
		for (int i = 0; i < LANGUAGE_MAX; i++)
		{
			if (i == MESS_ENG || i == LANGUAGE_MAX) continue;
			if (mes[i][cnt].IsEmpty())
				mes[i][cnt] = mes[MESS_ENG][cnt];
		}

		xml.OutOfElem();
	}
	xml.OutOfElem();
}

void CXmlText::Save(LPCTSTR filename)
{
	CMarkup xml;
	LPCTSTR order[] = { _T("ENG"),_T("ITA") };

	xml.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

	xml.AddElem(_T("Text"));
	xml.IntoElem();
	for (int i = 0; i < GetCount(); i++)
	{
		xml.AddElem(_T("Entry"));
		xml.AddAttrib(_T("type"), type[i]);
		xml.IntoElem();
		for (int j = 0; j < LANGUAGE_MAX; j++)
		{
			GString temp = mes[j][i];
			temp.Replace(_T("\r\n"), _T("\\n"));
			xml.AddElem(_T("String"), temp, CMarkup::MNT_TEXT);
			xml.AddAttrib(_T("id"), order[j]);
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();

	GString doc = xml.GetDoc();
	FILE *out = _tfopen(filename, _T("wb+"));
	WriteBOM(out);
	WriteUtf8(doc, out);
	fclose(out);
}
#endif
