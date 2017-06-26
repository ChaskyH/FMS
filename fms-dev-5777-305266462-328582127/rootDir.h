#pragma once
#pragma pack (1)
#include "dir.h"

class RootDir
{
public:
	Dir dirs[2];
	
	RootDir()
	{
	}

	void formatRootDir();

	bool setEntry(DirEntry& entry);

	DirEntry findFile(string& name);

	void inactivateEntry(string& name);

	void updateEntry(DirEntry entry);

	friend ostream& operator<<(ostream& os, RootDir& root);
};