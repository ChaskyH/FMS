#include "dat.h"
using namespace fms;

DAT::DAT()
{
	sectorNr = 1;
	formatDat(0, 1);
}
void DAT::formatDat(int addrDatCpy, int addrRootDirCpy)
{

	table.set();
	table.set(addrDatCpy, 0);
	table.set(addrRootDirCpy, 0);
}

int DAT::place_fits_all(int needClusters)
{

	int needClustTmp = needClusters;
	int firstClust = -1;

	//check if we have continuity of clusters
	for (int i = 0; i < NUM_CLUSTERS; i++)
	{
		if (table[i] == 1)
		{
			if (firstClust == -1)
				firstClust = i;
			needClustTmp--;
			if (needClustTmp == 0)
				return firstClust;
		}
		else //if(cluster[i]==0)
		{
			firstClust = -1;
			needClustTmp = needClusters;
		}
	}
	return 0;
}