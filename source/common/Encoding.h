#pragma once
#include <stdafx.h>

void BuildDictionaries(u8 *rom);

GString DecodeString(u8 *data);
GString DecodeStringAscii(u8 *data);

class CEncoding
{
public:
	CEncoding();
	~CEncoding();
};

