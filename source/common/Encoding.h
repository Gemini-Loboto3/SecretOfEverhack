#pragma once
#include <stdafx.h>

void BuildDictionaries(u8 *rom);

GString DecodeString(u8 *data);
GString DecodeStringAscii(u8 *data);

int EncodeString(GString str, u8 *&dest);

#if USE_XML
enum
{
	MESS_ENG,
	MESS_ITA,
	LANGUAGE_MAX
};

class CXmlText
{
public:
	void Open(LPCTSTR filename);
	void Save(LPCTSTR filename);
	void Reset() { for(int i=0; i<LANGUAGE_MAX; i++) mes[i].RemoveAll(); }

	GString GetString(int language, int index) { return mes[language][index]; }
	int GetType(int index) { return type[index]; }

	int GetCount() { return mes[0].GetCount(); }

	GStringArray mes[LANGUAGE_MAX];
	std::vector<int> type;
};
#else
class CXText : CText
{

};
#endif
