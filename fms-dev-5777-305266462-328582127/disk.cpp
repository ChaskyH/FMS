#pragma pack(1)
#include "disk.h"
#include "fcb.h"
using namespace fms;
uint setNone(Disk*, string&) { return NONE; }

Disk::Disk():vhd(), rootDir(), dat()
{
	mounted = false;
	currentDiskSectorNr = 0;
}

Disk::Disk(string& name, string& owner, char c) :Disk()
{
	switch (c)
	{
	case 'C':
	case 'c':
		try
		{
			createDisk(name, owner);
			mountDisk(name);
		}
		catch (...)
		{
			throw;
		}
		break;
	case 'M':
	case 'm':
		mountDisk(name);
		break;
	default:
		break;
	}
	checkFile = setNone;
}

Disk::~Disk()
{
	if (mounted)
		unmountDisk();
}


void Disk::createDisk(string& name, string& owner)
{
	name = name.substr(0, 11);
	owner = owner.substr(0, 11);

	try
	{
		diskfl.open(name, ios::out | ios::binary| ios::_Noreplace);
		if (!diskfl.good())
			throw "ERROR: problem opening disk.";
		mounted = true;
	}
	catch (const char* msg)
	{
		throw msg;
	}

	currentDiskSectorNr = 0;
	
	vhd.init(name, owner);
	
	writeSector(&vhd);
	writeSector(&dat);
	writeSector(&rootDir.dirs[0]);
	writeSector(&rootDir.dirs[1]);

	for (int i = 4; i < 3200; i++)
	{
		Sector sec(i);
		writeSector(&sec);
	}

	diskfl.close();
	mounted = false;
}

void Disk::mountDisk(string& name)
{
	if (mounted)
		throw "ERROR: disk is already mounted.";
	if (!fstream(name))
		throw "ERROR: disk with this name doesn't exist.";
	try
	{
		diskfl.open(name, ios::in | ios::out | ios::binary);
		diskfl.read(reinterpret_cast<char*>(&vhd),BUFFER_SIZE);
		diskfl.read(reinterpret_cast<char*>(&dat), BUFFER_SIZE);
		diskfl.read(reinterpret_cast<char*>(&rootDir.dirs[0]), BUFFER_SIZE);
		diskfl.read(reinterpret_cast<char*>(&rootDir.dirs[1]), BUFFER_SIZE);
	}
	catch(const char* msg)
	{
		throw msg;
	}
	mounted = true;
	seekToSector(0);
}

void Disk::unmountDisk()
{
	if (!mounted)
		throw "ERROR: disk not mounted.";
	
	try
	{
		writeSector(0,&vhd);
		writeSector(1,&dat);
		writeSector(2,&rootDir.dirs[0]);
		writeSector(3,&rootDir.dirs[1]);

		dat.sectorNr = vhd.addrDatCpy;
		writeSector(vhd.addrDatCpy, &dat);

		rootDir.dirs[0].sectorNr = vhd.addrRootDirCpy;
		rootDir.dirs[1].sectorNr = vhd.addrRootDirCpy + 1;
		writeSector(vhd.addrRootDirCpy, &rootDir.dirs[0]);
		writeSector(vhd.addrRootDir+1, &rootDir.dirs[1]);
		
		diskfl.close();
	}
	catch (const char * msg)
	{
		throw msg;
	}

	mounted = false;
}

void Disk::recreateDisk(string& owner)
{
	if (mounted)
		throw "ERROR: disk is mounted, to recreate unmount first.";
	if (owner != vhd.diskOwner)
		throw "ERROR: owner does not match the disk owner.";
	
	try
	{
		diskfl.open(vhd.diskName, ios::out | ios::binary);
		mounted = true;
	}
	catch (char* msg)
	{
		throw msg;
	}

	currentDiskSectorNr = 0;

	writeSector(&vhd);
	writeSector(&dat);
	writeSector(&rootDir.dirs[0]);
	writeSector(&rootDir.dirs[1]);

	for (int i = 4; i < 3200; i++)
	{
		Sector sec(i);
		writeSector(&sec);
	}	

	diskfl.close();
	mounted = false;
	mountDisk(string(vhd.diskName));
}

fstream* Disk::getDskfl()
{
	try
	{
		return &diskfl;
	}
	catch(...)
	{
		return nullptr;
	}
}

void Disk::seekToSector(uint secNr)
{
	if (!mounted)
		throw "ERROR: Disk not mounted.";
	if (secNr < 0 || secNr > 3200)
		throw "out of range";

	if (secNr == 3200)
		secNr = 0;

	currentDiskSectorNr = secNr;

	try
	{
		diskfl.seekp(fms::BUFFER_SIZE*secNr);
	}
	catch (const char* msg)
	{
		throw msg;
	}
}

void Disk::writeSector(uint secNr, Sector* sec)
{
	seekToSector(secNr);
	writeSector(sec);
}

void Disk::writeSector(Sector* sec)
{
	if (!mounted)
		throw "ERROR: Disk not mounted.";
	
	sec->sectorNr = currentDiskSectorNr;
	try
	{
		diskfl.write(reinterpret_cast<const char*>(sec), fms::BUFFER_SIZE);
		seekToSector(++currentDiskSectorNr);
	}
	catch (const char* msg)
	{
		throw msg;
	}
}

void Disk::readSector(uint secNr, Sector* sec)
{ 
	seekToSector(secNr);
	readSector(sec);
}

void Disk::readSector(Sector* sec)
{
	if (!mounted)
		throw "ERROR: Disk not mounted.";
	try
	{
		diskfl.read(reinterpret_cast<char*>(sec), fms::BUFFER_SIZE);
		seekToSector(++currentDiskSectorNr);
	}
	catch (const char* msg)
	{
		throw msg;
	}
}


void Disk::format(string& owner)
{
	if (owner != vhd.diskOwner)
		throw "ERROR: Only the owner of the disk can format it.";
	if (!mounted)
		throw "ERROR: You must open the disk before formatting it.";
	try
	{
		dat.formatDat(vhd.addrDatCpy, vhd.addrRootDirCpy);
		rootDir.formatRootDir();
	}
	catch (const char* msg)
	{
		throw msg;
	}

}

int Disk::howMuchIsEmpty()
{
	if (!mounted)
		throw "ERROR: disk is not mount.";
	return dat.table.count();
}

void Disk::alloc(DATtype& fat, uint n_sector)
{
	if (n_sector < 1)
		throw "ERROR: number of sector must be a positive number.";
	
	int n_cluster = n_sector / 2.0 + 0.5;

	if (n_cluster > howMuchIsEmpty())
		throw "ERROR: there is no enough free place in the disk to this file.";

	int p = dat.place_fits_all(n_cluster);

	if (p != 0)
	{
		for (int i = 0; i < n_cluster; i++)
		{
			fat[p + i] = 1;
		}
	}
	else
	{
		for (int i = 0; n_cluster != 0; i++)
			if (dat.table[i] == 1)
			{
				fat[i] = 1;
				n_cluster--;
			}
	}
	dat.table &= ~fat;
}

void Disk::allocExtend(DATtype& fat, uint n_sector)
{
	alloc(fat, n_sector);
}

void Disk::dealloc(DATtype& fat)
{
	dat.table |= fat;
	fat.reset();
}

void Disk::createFile(string& name, string& owner, uint size)
{
	DATtype fat;
	int sectors = 1 + size / 1024.0 + 0.5;
	alloc(fat, sectors);
	int addr = 0;
	for (int i = 0; !fat[i]; addr = ++i); // get address of first cluster used for file
	DirEntry entry(name, owner, addr * 2, sectors, size);
	
	if (rootDir.findFile(name).entryStatus == fms::ACTIVE)
		throw "ERROR: there already is a file in this disk with that name.";
	if (!rootDir.setEntry(entry))
		throw "ERROR: there is no more place in this disk's directory.";

	FHD fhd(entry, fat);
	writeSector(entry.fileAddr, &fhd);
}

void Disk::delFile(string& name, string& owner)
{
	DirEntry entry = rootDir.findFile(name);
	if (entry.entryStatus != ACTIVE)
		throw "ERROR: no file with that name exists in directory.";
	FHD fhd;
	readSector(entry.fileAddr, &fhd);
	if (string(fhd.fileDesc.fileOwner) != owner.substr(0, 12))
		throw "ERROR: only the owner of a file can delete it.";
	dealloc(fhd.fat);
	rootDir.inactivateEntry(name);
}

void Disk::extendFile(string& name, string& owner, uint size)
{
	DirEntry entry = rootDir.findFile(name);
	if (entry.entryStatus != fms::ACTIVE)
		throw "ERROR: no file in disk with such a name.";
	if (string(entry.fileOwner) != owner.substr(0, 11))
		throw "ERROR: only the owner of this file can extend it.";

	int sectors = 1 + size / 1024.0 + 0.5;

	FHD fhd;
	readSector(entry.fileAddr, &fhd);

	allocExtend(fhd.fat, sectors);
	fhd.fileDesc.fileSectors += sectors;
	fhd.fileDesc.fileSize += size;
	rootDir.updateEntry(fhd.fileDesc);

	writeSector(entry.fileAddr, &fhd);
}

void Disk::cutFile(string& name, string& owner, uint size)
{
	DirEntry entry = rootDir.findFile(name);
	if (entry.entryStatus != fms::ACTIVE)
		throw "ERROR: there is no such file with that name.";
	if (string(entry.fileOwner) != owner.substr(0, 11))
		throw "ERROR: only the owner of a file can cut it.";

	FHD fhd;
	readSector(entry.fileAddr, &fhd);

	int sectors = 1 + size / 1024.0 + 0.5;
	int clusters = sectors / 2 + 0.5;

	DATtype toCut = fhd.fat;

	for (int i = 0, j = 0; j < clusters; i++)
	{
		if (toCut[i])
		{
			j++;
			toCut[i] = 0;
		}
	}

	dealloc(toCut);

	fhd.fat &= ~toCut;

	fhd.fileDesc.fileSize = size;
	fhd.fileDesc.fileSectors = sectors;

	writeSector(fhd.fileDesc.fileAddr, &fhd);
	rootDir.updateEntry(fhd.fileDesc);
}

void Disk::openFile(FCB& fcb, string & name, string & owner = string(), uint mode = READ)
{
	// find file in rootdir
	DirEntry entry = rootDir.findFile(name);
	
	if (entry.entryStatus != ACTIVE)
		throw "ERROR: no file with such a name exist in the disk";
	if (mode != READ && entry.fileOwner != owner.substr(0, 12))
		throw "ERROR: only the owner of a file can edit it";

	if (checkFile(this, name) == READ && mode != READ)
		throw "ERROR: this file is allready open, you can't edit it.";
	if (checkFile(this, name) != NONE && (fcb.mode == WRITE || fcb.mode == APPEND || fcb.mode == READWRITE))
		throw "ERROR: can't open file. It is already open to read.";
	
	// read file header from disk
	FHD fhd;
	readSector(entry.fileAddr, &fhd);

	fcb.d = this;
	fcb.mode = mode;
	fcb.fileDesc = fhd.fileDesc;
	
	if (mode == WRITE) // trunc file
		fcb.fileDesc.fileSize = 0;
	if (mode == APPEND) // move to eof
		for (int i = 0; i < fcb.fileDesc.fileSize;);
		
	
	fcb.currSecNr = sec;
	fcb.currByteInBuff = 0;
	fcb.currByte = 0;
	fcb.FAT = fhd.fat;
}


void Disk::print()
{
	if (!mounted)
		throw "disk not mounted";

	Sector tmp;
	currentDiskSectorNr = 0;
	for (int i = 0; i < 3200; i++)
	{
		readSector(&tmp);
		cout << tmp;
	}
}

void Disk::setCheckFile(uint (*f)(Disk*, string&))
{
	checkFile = f;
}