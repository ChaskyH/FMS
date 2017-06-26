#pragma once
#include "disk.h"

class FCB
{
public:
	Disk* d;
	DirEntry fileDesc;
	DATtype FAT;
	unsigned int mode;
	Sector buffer;
	unsigned int currByte;
	unsigned int currSecNr;
	unsigned int currByteInBuff;
	FCB();
	FCB(Disk*);
	~FCB();

	void openFile(Disk *, string &, string &, uint);
	void closeFile();
	void flushFile();
	uint read(char *, unsigned int);
	void write(char *, unsigned int);
	void seek(uint, int);
	void remove();
	bool eof();
	void print();
};