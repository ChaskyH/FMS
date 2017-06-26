#pragma once
#pragma pack (1)
#include "sector.h"
#include "vhd.h"
#include "rootDir.h"
#include "dat.h"
#include "dirEntry.h"
#include "fms.h"
#include "fhd.h"


class FCB;

class Disk
{
public:
	VHD vhd;
	DAT dat;
	RootDir rootDir;
	bool mounted;
	fstream diskfl;
	uint currentDiskSectorNr;
	uint(*checkFile)(Disk*, string &);

	/*************************************************
	* FUNCTION
	*	Disk()
	* PARAMETERS
	*	None
	* RETURN VALUE
	*	none
	* MEANING
	*	The default constructor for the Disk class.
	*	Initializes all the class's fields. Sets the disk name
	*	and owner to "".
	* SEE ALSO
	*	Disk(string&, string&, char)
	**************************************************/
	Disk();

	/*************************************************
	* FUNCTION
	*	Disk(string&, string&, char);
	* PARAMETERS
	*	string - the name of the disk
	*	string - the name of the disk's owner
	*	char   - indicates if to create and mount disk (c)
	*			 or just to mount (m)
	* RETURN VALUE
	*	none
	* MEANING
	*	a class constructor for the Disk class
	*	initializes all class fields
	* SEE ALSO
	*	Disk()
	**************************************************/
	Disk(string&, string&, char);
	
	 /*************************************************
	* FUNCTION
	*	~Disk()
	* PARAMETERS
	*	none
	* RETURN VALUE
	*	none
	* MEANING
	*	delete all the classes dynamic memory and unmounts
	*	the disk if it is mounted.
	* SEE ALSO
	* 
	**************************************************/
	~Disk();

	/*************************************************
	* FUNCTION
	*	void createDisk(string&, string&)
	* PARAMETERS
	*	string - the name of the disk to create
	*	string - the name of the owner of the disk
	* RETURN VALUE
	*	void
	* MEANING
	*	creates a virtual disk on a file. also initializes
	*	the vhd fields and set all sectors.
	* SEE ALSO
	* 
	**************************************************/
	void createDisk(string&, string&);

	/*************************************************
	* FUNCTION
	*	void mountDisk(string&)
	* PARAMETERS
	*   string - the name of the disk to mount
	* RETURN VALUE
	*	void
	* MEANING
	*	mounts the virtual disk file. reads all sector data
	*	to relevant fields in disk
	* SEE ALSO
	* 
	**************************************************/
	void mountDisk(string&);

	/*************************************************
	* FUNCTION
	*	void unmountDisk()
	* PARAMETERS
	*	none
	* RETURN VALUE
	*	none
	* MEANING
	*	unmounts the disk by saving all changed sector to the
	*	virual disk file.
	* SEE ALSO
	* 
	**************************************************/
	void unmountDisk();

	/*************************************************
	* FUNCTION
	*	void recreateDisk(string&)
	* PARAMETERS
	*	string - the name of the owner
	* RETURN VALUE
	*	none
	* MEANING
	*	does everything in create disk if (1) the name of the
	*	owner is right, (2) the disk is already created, (3) the
	*	can't be mounted already.
	* SEE ALSO
	*	createDisk, mountDisk
	**************************************************/
	void recreateDisk(string&);

	/*************************************************
	* FUNCTION
	*	fstream* getDskfl()
	* PARAMETERS
	*	none
	* RETURN VALUE
	*	fstream* - a pointer to the fstream that is used for the
	*			   virual disk
	* MEANING
	*	a getter function for the disk file fstream. returns
	*	null on any problem.
	* SEE ALSO
	* 
	**************************************************/
	fstream* getDskfl();

	/*************************************************
	* FUNCTION
	*	void seekToSector(uint)
	* PARAMETERS
	*	uint - to sector number to set to
	* RETURN VALUE
	*	none
	* MEANING
	*	sets the current sector number
	* SEE ALSO
	* 
	**************************************************/
	void seekToSector(uint);

	/*************************************************
	* FUNCTION
	*	void writeSector(uint, Sector*)
	* PARAMETERS
	*	uint - the sector number to write to
	*	Sector*		 - a pointer to the sector to write from
	* RETURN VALUE
	*	none
	* MEANING
	*	copys to the sector specified in the first parameter from
	*	the sector the second parameter is pointing to
	* SEE ALSO
	*
	**************************************************/
	void writeSector(uint, Sector*);

	/*************************************************
	* FUNCTION
	*	void writeSector(Sector*)
	* PARAMETERS
	*	Sector*	- a pointer to the sector to write from
	* RETURN VALUE
	*	none
	* MEANING
	*	copys to the current sector from the sector specified
	* SEE ALSO
	*
	**************************************************/
	void writeSector(Sector*);

	/*************************************************
	* FUNCTION
	*	void readSector(uint, Sector*)
	* PARAMETERS
	*	uint - the sector number to read from
	*	Sector*      - the sector to read to
	* RETURN VALUE
	*	none
	* MEANING
	*	reads a sector from the disk. after reading advances current sector to after
	*	the sector read.
	* SEE ALSO
	*
	**************************************************/
	void readSector(uint, Sector*);

	/*************************************************
	* FUNCTION
	*	void readSector(Sector*)
	* PARAMETERS
	*	Sector* - a pointer to the sector to read to
	* RETURN VALUE
	*	none
	* MEANING
	*	reads one sector from current sector 
	* SEE ALSO
	*	readSector(uint, Sector*)
	**************************************************/
	void readSector(Sector*);

	/*************************************************
	* FUNCTION
	*	void format(string&)
	* PARAMETERS
	*	string - the name of the disk's owner
	* RETURN VALUE
	*	none
	* MEANING
	*	formats the disk if givin the right name. reset the DAT accordinly.
	*	also resets the root directory to be empty.
	* SEE ALSO
	*
	**************************************************/
	void format(string&);

	/*************************************************
	* FUNCTION
	*	int howMuchIsEmpty()
	* PARAMETERS
	*	none
	* RETURN VALUE
	*	the amount of clusters
	* MEANING
	*	checks the amount of empty clusters in the disk
	* SEE ALSO
	*
	**************************************************/
	int howMuchIsEmpty();

	/*************************************************
	* FUNCTION
	*	void alloc(DATtype&, uint)
	* PARAMETERS
	*	DATtype		 - the files FAT table
	*	uint - the size of sectors needed
	* RETURN VALUE
	*	none
	* MEANING
	*	allocates memory in the disk by finding a cluster of the right size.
	*	changes DAT and FAT accordingly.
	* SEE ALSO
	*
	**************************************************/
	void alloc(DATtype&, uint);

	/*************************************************
	* FUNCTION
	*	void allocExtend(DATtype&, uint)
	* PARAMETERS
	*	DATtype		 - the files FAT table
	*	uint - the size of sectors needed to extend
	* RETURN VALUE
	*	none
	* MEANING
	*	works like alloc but adds the sectors to file.
	* SEE ALSO
	*	alloc
	**************************************************/
	void allocExtend(DATtype&, uint);

	/*************************************************
	* FUNCTION
	*	dealloc(DATtype&)
	* PARAMETERS
	*	DATtype - the FAT table of the file to delete
	* RETURN VALUE
	*	none
	* MEANING
	*	releases memory specified in files FAT table from disk by changing
	*	the DAT accordingly
	* SEE ALSO
	*
	**************************************************/
	void dealloc(DATtype&);

	/*************************************************
	* FUNCTION
	*	void createFile(string&, string&, uint)
	* PARAMETERS
	*	string - the name of the file to create
	*	string - the name of the owner of the file
	*	uint - the size of the file (in Bytes)
	* RETURN VALUE
	*	none
	* MEANING
	*	create a file on the disk...
	* SEE ALSO
	*
	**************************************************/
	void createFile(string&, string&, uint);

	/*************************************************
	* FUNCTION
	*	void deleteFile(string&, string&)
	* PARAMETERS
	*	string - the name of the file to delete
	*	string - the name of the owner of the file
	* RETURN VALUE
	*	void
	* MEANING
	*	delete file from disk ...
	* SEE ALSO
	*
	**************************************************/
	void delFile(string&, string&);

	/*************************************************
	* FUNCTION
	*
	* PARAMETERS
	*
	* RETURN VALUE
	*
	* MEANING
	*
	* SEE ALSO
	*
	**************************************************/
	void extendFile(string&, string&, uint);

	/*************************************************
	* FUNCTION
	*
	* PARAMETERS
	*
	* RETURN VALUE
	*
	* MEANING
	*
	* SEE ALSO
	*
	**************************************************/
	void cutFile(string&, string&, uint);

	void openFile(FCB&, string&, string&, uint);

	void setCheckFile(uint(*f)(Disk*, string &));

	void print();
};
