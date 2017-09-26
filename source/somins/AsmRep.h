#pragma once
#include <stdafx.h>
#include <vector>

class CPtrCollector
{
public:
	CPtrCollector() {}
	~CPtrCollector() {}

	void Set_name(LPCTSTR id) { name = id; }
	void Add_ptr(u32 ptr) { pointer.push_back(ptr); }

	int size() { return (int)pointer.size(); }

	// return -1 when there's no match
	// otherwise return id number after string name
	int Get_match(LPCTSTR n)
	{
		int len = name.GetLength(), pos = 0;

		// match substring until space is found
		for (; pos < len; pos++)
		{
			// done parsing, we still alive
			if (n[pos + 1] == ' ')
			{
				pos++;	// skip space
				break;
			}
			// no match, kill this entirely
			if (n[pos + 1] != name[pos])
				return -1;
		}

		return _tstoi(&n[pos]);
	}

	void Clear()
	{
		name.Empty();
		pointer.clear();
	}

	GString name;				// file ID
	std::vector<u32> pointer;	// pointer data gathered
};

class CAsmRep
{
public:
	CAsmRep();
	~CAsmRep();

	void Open(LPCTSTR in_name);
	void Save(LPCTSTR out_name);

	void Add_collector(CPtrCollector &c) { cl.push_back(c); }
	void Process();

	GString text;
	std::vector<CPtrCollector> cl;	// pointer collectors
};

