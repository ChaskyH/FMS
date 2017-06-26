#pragma pack (1)
#pragma once

#include "fms.h"
#include "sector.h"
class FHD : public Sector
{
public:
	DirEntry& fileDesc = reinterpret_cast<DirEntry&>(rawData[0]);
	DATtype& fat = reinterpret_cast<DATtype&>(rawData[48]);
	FHD() {}
	FHD(DirEntry& desc, DATtype& fat)
	{
		write(reinterpret_cast<char*>(&desc), 0, 48);
		write(reinterpret_cast<char*>(&fat), 48, 200);
	}
};
