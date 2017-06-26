#include "rootDir.h"

void RootDir::formatRootDir()
{
	for (int i = 0; i < 21; i++)
	{
		dirs[0].entrys[i].entryStatus = fms::EMPTY;
		dirs[1].entrys[i].entryStatus = fms::EMPTY;
	}
}

bool RootDir::setEntry(DirEntry& entry)
{
	for (int i = 0; i < 21; i++)
	{
		if (dirs[0].entrys[i].entryStatus != fms::ACTIVE)
		{
			dirs[0].entrys[i] = entry;
			dirs[0].entrys[i].entryStatus = fms::ACTIVE;
			return true;
		}
		if (dirs[1].entrys[i].entryStatus != fms::ACTIVE)
		{
			dirs[1].entrys[i] = entry;
			dirs[1].entrys[i].entryStatus = fms::ACTIVE;
			return true;
		}
	}
	return false;
}

DirEntry RootDir::findFile(string& name)
{
	for (int i = 0; i < 21; i++)
	{
		if ((dirs[0].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[0].entrys[i].fileName == name.substr(0, 12)))
			return dirs[0].entrys[i];
		if (dirs[1].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[1].entrys[i].fileName == name.substr(0, 12))
			return dirs[1].entrys[i];
	}
	return DirEntry();
}

void RootDir::inactivateEntry(string& name)
{
	for (int i = 0; i < 21; i++)
	{
		if ((dirs[0].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[0].entrys[i].fileName == name.substr(0, 12)))
		{
			dirs[0].entrys[i].entryStatus = fms::INACTIVE;
			return;
		}
		if (dirs[1].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[1].entrys[i].fileName == name.substr(0, 12))
		{
			dirs[1].entrys[i].entryStatus = fms::INACTIVE;
			return;
		}
	}
}

void RootDir::updateEntry(DirEntry entry)
{
	for (int i = 0; i < 21; i++)
	{
		if ((dirs[0].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[0].entrys[i].fileName == entry.fileName))
		{
			dirs[0].entrys[i] = entry;
			return;
		}
		if (dirs[1].entrys[i].entryStatus == fms::ACTIVE &&
			dirs[1].entrys[i].fileName == entry.fileName)
		{
			dirs[1].entrys[i] = entry;
			return;
		}
	}
	throw "ERROR: no file with such name is in the disk.";
}

ostream& operator<<(ostream& os, RootDir& root)
{
	int count = 0, size = 0, sizeOnDisk=0, clusters = 0;
	os << setw(12) << left << "name";
	os << setw(12) << left << "owner";
	os << setw(10) << left << "created";
	os << setw(10) << left << "size";
	os << setw(10) << left << "size on disk" << endl;
	os << setw(80) << setfill('-') << "-" << endl << setfill(' ');
	for (int i = 0; i < 21; i++)
	{
		DirEntry& entry1 = root.dirs[0].entrys[i];
		DirEntry& entry2 = root.dirs[1].entrys[i];
		if (entry1.entryStatus == fms::ACTIVE)
		{
			os << setw(12) << left << entry1.fileName;
			os << setw(12) << left << entry1.fileOwner;
			os << setw(10) << right << string(entry1.crDate, 8);
			os << setw(10) << right << entry1.fileSize;
			os << setw(10) << right << (entry1.fileSectors + entry1.fileSectors%2)*fms::BUFFER_SIZE << endl;
			count++;
			size += entry1.fileSize;
			sizeOnDisk += entry1.fileSectors*fms::BUFFER_SIZE;
		}
		if (entry2.entryStatus == fms::ACTIVE)
		{
			os << setw(12) << left << entry2.fileName;
			os << setw(12) << left << entry2.fileOwner;
			os << setw(10) << right << string(entry2.crDate, 8);
			os << setw(10) << right << entry2.fileSize;
			os << setw(10) << right << (entry2.fileSectors + entry2.fileSectors % 2)*fms::BUFFER_SIZE << endl;
			count++;
			size += entry2.fileSize;
			sizeOnDisk += entry2.fileSectors*fms::BUFFER_SIZE;
		}
	}
	os << count << " files on " << size / 1024 << "Kb (" << sizeOnDisk / 1024 << "Kb on disk)" << endl;
	return os;
}
