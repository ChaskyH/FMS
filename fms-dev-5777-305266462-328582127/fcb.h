#pragma once
#include "disk.h"

class FCB
{
public:
	Disk* d;
	DirEntry fileDesc;
	DATtype FAT;
	uint mode;
	Sector buffer;
	uint currByte;
	uint currSecNr;
	uint currByteInBuff;
	FCB();
	FCB(Disk*);
	~FCB();

	void openFile(Disk *, string &, string &, uint);

	void closeFile();
	void flushFile();
	uint read(char *, uint);
	void write(char *, uint);
	void seek(uint, int);
	void remove();
	bool eof();
	void print();
};