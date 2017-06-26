#pragma pack (1)
#pragma once

#include "fms.h"
#include "sector.h"
class VHD : public Sector
{
	Name& diskName = reinterpret_cast<Name&>(rawData[0]);
	Name& diskOwner = reinterpret_cast<Name&>(rawData[12]);
	Date& prodDate = reinterpret_cast<Date&>(rawData[24]);
	uint& clusQty = reinterpret_cast<uint&>(rawData[32]);
	uint& dataClusQty = reinterpret_cast<uint&>(rawData[36]);
	uint& addrDat = reinterpret_cast<uint&>(rawData[40]);
	uint& addrRootDir = reinterpret_cast<uint&>(rawData[44]);
	uint& addrDatCpy = reinterpret_cast<uint&>(rawData[48]);
	uint& addrRootDirCpy = reinterpret_cast<uint&>(rawData[52]);
	uint& addrDataStart = reinterpret_cast<uint&>(rawData[56]);
	Date& formatDate = reinterpret_cast<Date&>(rawData[60]);
	bool& isFormated = reinterpret_cast<bool&>(rawData[64]);
public:
	VHD();
	VHD(string& name, string& owner);
	void print();
	friend ostream& operator<<(ostream& os, VHD vhd);
	void init(string& name, string& owner);
	friend class Disk;
};