#include "sector.h"
#include<iomanip>

Sector::Sector(int num)
{
	sectorNr = num;
	for (int i = 0; i < 1020; i++)
		rawData[i] = '\0';
}

void Sector::write(const char* source, uint index, uint size)
{
	if (index + size > 1020)
		return;
	for (int i = index; i < index + size; i++) // changed here
		rawData[i] = source[i - index];
	rawData[size] = 0;
}

void Sector::print()
{

}

ostream& operator<<(ostream & os, Sector s)
{
	os << "Sector # " << s.sectorNr << endl;
	os << "----------------------- Data -----------------------" << endl << "    ";
	for (int i = 0; i < 16; i++)
		os << hex << uppercase << setw(3) << i;
	os << endl;
	for (int i = 0; i < 1020; i += 16)
	{
		os << std::hex << std::uppercase << setfill('0') << setw(3) << i<< " | ";
		
		for (int j = 0; j < 16; j++)
		{
			int byte = ((i + j) < 1020) ? s.rawData[i + j] : 0;
			os << setw(2) << byte << " ";
		}
		
		os << " | ";
		for (int j = 0; j < 16; j++)
		{
			int byte = ((i+j) < 1020) ? s.rawData[i + j] : 0;
			if (byte >= 32 && byte < 127)
			{
				os << (char)byte;
			}
			else
			{
				 os << '.';
			}
		}
		os << endl;
	}
	os << setw(0) << setfill(' ') << dec;
	return os;
}

char* byteToHex(int) { return nullptr; }