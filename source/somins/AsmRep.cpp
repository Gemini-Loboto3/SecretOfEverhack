#include "AsmRep.h"

CAsmRep::CAsmRep()
{
}

CAsmRep::~CAsmRep()
{
}

void CAsmRep::Open(LPCTSTR in_name)
{
	TCHAR *buf;
	StoreUtf8Text(in_name, buf);

	text = buf;
	delete[] buf;
}

void CAsmRep::Save(LPCTSTR out_name)
{
	FILE *fout = _tfopen(out_name, _T("wb+"));

	TCHAR *b = text.GetBuffer();
	for (int i = 0, si = text.GetSize(); i < si; i++)
		fwrite(&b[i], 1, 1, fout);
	fclose(fout);
}

void CAsmRep::Process()
{
	//for (int i = 0, is = cl.size(); i < is; i++)
	//{
	//	CPtrCollector &c = cl[i];
	//	for (int j = 0, js = c.size(); j < js; j++)
	//	{

	//	}
	//}
	
	GString ntxt, tmp;	// new text
	for (int i = 0, si = text.GetSize(); i < si; i++)
	{
		// <!name id> (low pointer)
		// <@name id> (high pointer)
		// <#name id> (full pointer)
		if (text[i] == '<')
		{
			int j;
			GString n;
			// build tag name
			for (j = 1; text[i + j] != '>'; j++)
				n += text[i + j];

			u32 ptr;
			// search for name inside tables
			for (int k = 0, ks = cl.size(); k < ks; k++)
			{
				CPtrCollector &c = cl[k];
				for (int l = 0, ls = c.size(); l < ls; l++)
				{
					c.name[l];
				}
			}

			switch (n[0])
			{
			case '!':	// low pointer
				tmp.Format(_T("$%04X"), ptr & 0xffff);
				ntxt += tmp;
				break;
			case '@':	// high pointer
				tmp.Format(_T("$%02X"), (ptr >> 16) & 0xff);
				ntxt += tmp;
				break;
			case '#':	// full pointer
				tmp.Format(_T("$%06X"), ptr & 0xffffff);
				ntxt += tmp;
				break;
			default:
				_tprintf(_T("Undefined char type in %s\n"), (LPCTSTR)n);
				ntxt += _T("<ERROR>");
			}
				
			// skip tag
			i += j + 1;
		}
		// copy as is
		else ntxt += text[i];
	}
}
