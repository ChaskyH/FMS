#pragma once
#pragma pack (1)
#include "fms.h"
#include "vhd.h"
class DAT : public Sector
{
public:
	DATtype& table = reinterpret_cast<DATtype&>(rawData[0]);
	DAT();
	void formatDat(int addrDatCpy, int addrRootDirCpy);
	int place_fits_all(int needClusters);
	
};