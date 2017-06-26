#pragma once
#include "disk.h"

class FCB
{
	//
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
	unsigned int read(char *, unsigned int);
	void write(char *, unsigned int);
	void readBuffer();
	void writeBuffer();
	void seek(uint, int);
	void seekBuffer(uint relativeTo, int sectors);
	void remove();
	bool eof();
	void print();
};