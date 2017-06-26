#pragma pack (1)
#pragma once
#include "fms.h"
class Sector
{
public:
	uint sectorNr;
	char rawData[1020];
	
	
	Sector(int num = 0);
	virtual void print(); 
	void write(const char*, uint, uint);
	friend ostream& operator<<(ostream& of, Sector s);
};

char* byteToHex(int);
