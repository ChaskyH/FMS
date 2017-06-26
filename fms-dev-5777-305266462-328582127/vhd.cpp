#include "vhd.h"

VHD::VHD() 
{
	sectorNr = 0;
	addrDataStart = 4;
	clusQty = 1600;
	dataClusQty = 1598;
	addrDat = 1;
	addrRootDir = 2;
	addrDatCpy = 1;
	addrRootDirCpy = 2;
	isFormated = false;
}

//string getTimeFormat(time_t c = time(0))
//{
//	struct tm t;
//	localtime_s(&t, &c);
//	char buffer[10];
//	strftime(buffer, 10, "%Y%m%d", &t);
//	string time = buffer;
//	return time;
//}

VHD::VHD(string & name, string & owner):VHD()
{
	init(name, owner);
}

void VHD::init(string& name, string& owner)
{
	sectorNr = 0;
	write(name.c_str(), 0, 11);
	write(owner.c_str(), 12, 11);
	write(fms::fmsGetTimeFormat().c_str(), 24, 8);
}

void VHD::print() {}

ostream& operator<<(ostream& os, VHD vhd)
{
	os << "Sector # " << vhd.sectorNr << endl;
	os << "------------- Data ---------------" << endl;
	os << "disk name: " << vhd.diskName << endl;
	os << "disk owner: " << vhd.diskOwner << endl;
	os << "date created: " << string(vhd.prodDate,8) << endl;
	os << "clusters: " << vhd.clusQty << endl;
	os << "data clusters: " << vhd.dataClusQty << endl;
	os << "data sector NR: " << vhd.addrDataStart << endl;
	os << "DAT address: " << vhd.addrDat << endl;
	os << "DAT copy address: " << vhd.addrDatCpy << endl;
	os << "Root Dir address: " << vhd.addrRootDir << endl;
	os << "Root Dir copy address: " << vhd.addrRootDirCpy << endl;
	os << "format date: " << string(vhd.formatDate,8) << endl;
	os << "is formated: " << vhd.formatDate << endl;

	return os;
}