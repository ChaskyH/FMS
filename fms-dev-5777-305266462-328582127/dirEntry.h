#pragma pack (1)
#pragma once
#include "vhd.h"
class DirEntry
{
public:
	Name fileName;
	Name fileOwner;
	uint fileAddr;
	Date crDate;
	uint fileSectors;
	uint fileSize;
	char emptyArea[3];
	uint entryStatus;
	DirEntry() {}
	DirEntry(string& name, string& owner, uint addr, uint sec, uint size) 
	{
		strcpy_s(fileName, 12, name.substr(0,11).c_str());
		strcpy_s(fileOwner, 12, owner.substr(0, 11).c_str());
		fileAddr = addr;
		string date = fms::fmsGetTimeFormat();
		for (int i = 0; i < 8; i++)
			crDate[i] = date[i];

		fileSectors = sec;
		fileSize = size;
	}
	DirEntry(const DirEntry& entry)
	{
		strcpy_s(fileName, 12, entry.fileName);
		strcpy_s(fileOwner, 12, entry.fileOwner);
		fileAddr = entry.fileAddr;
		
		// copy data info -- can't use strcpy, because there's no '/0'
		for (int i = 0; i < 8; i++)
			crDate[i] = entry.crDate[i];

		fileSectors = entry.fileSectors;
		fileSize = entry.fileSize;
		entryStatus = entry.entryStatus;
	}
	DirEntry& operator=(const DirEntry& entry)
	{
		strcpy_s(fileName, 12, entry.fileName);
		strcpy_s(fileOwner, 12, entry.fileOwner);
		fileAddr = entry.fileAddr;

		//strcpy_s(crDate, 8, entry.crDate);

		// copy data info -- can't use strcpy, because there's no '/0'
		for (int i = 0; i < 8; i++)
			crDate[i] = entry.crDate[i];

		fileSectors = entry.fileSectors;
		fileSize = entry.fileSize;
		entryStatus = entry.entryStatus;
		return *this;
	}
};