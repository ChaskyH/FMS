#include "fcb.h"
#include "fms.h"
using namespace fms;
FCB::FCB(): buffer(),FAT(),fileDesc()
{
	d = nullptr;
	currByte = 0;
	currByteInBuff = 0;
	currSecNr = 0;
	mode = fms::NONE;
}

FCB::FCB(Disk* d):FCB()
{
	d = d;
}

FCB::~FCB()
{

}


void FCB::openFile(Disk * dsk, string & name, string & owner , uint mode)
{
	if (d != nullptr)
		throw "ERROR: this file is already in use..";
	dsk->openFile(*this, name, owner, mode);
}

void FCB::closeFile()
{
	if (mode == NONE)
		throw "ERROR: no open file to close.";
	if(mode != READ)
		flushFile();

	// update fhd fields 
	FHD fhd(fileDesc, FAT);
	fhd.sectorNr = fileDesc.fileAddr;
	d->writeSector(fileDesc.fileAddr, &fhd);

	// update rootdir entry
	d->rootDir.updateEntry(fileDesc);

	d = nullptr;
}

void FCB::flushFile()
{
	d->writeSector(currSecNr, &buffer);
}

unsigned int FCB::read(char* dst, uint size = 0)
{
	if (mode == NONE)
		throw "ERROR: no file to read from.";
	if (mode == WRITE)
		throw "ERROR: file not open for reading.";
	if (currByte + size >= fileDesc.fileSize)
		throw "ERROR: out of range.";

	if (size) // when size is specified
	{
		for (int i = 0; i < size; i++, currByte++, currByteInBuff++)
		{
			if (currByteInBuff == BUFFER_SIZE) // next buffer
			{
				
			}
			dst[i] = buffer.rawData[currByteInBuff];
		}
	}
	else
	{
		while (buffer.rawData[currByteInBuff] != '\n' && size > 200 && !eof()) // no size specified
		{
			if (currByteInBuff == BUFFER_SIZE)
				
			dst[size] = buffer.rawData[currByteInBuff];
			currByte++, currByteInBuff++, size++;
		}
		currByte++, currByteInBuff++;
	}

	dst[size] = '\0'; // enter null character at end of dst
	return size;
}

void FCB::write(char *, unsigned int)
{
	cout << "Class: FCB, Function: write file" << endl;

}

void FCB::seek(uint relativeTo, int bytes)
{
	int sectors; // the amount of sector to move

	// error handling
	if (mode == NONE)
		throw "ERROR: no file assosiated yet.";
	if (mode == WRITE)
		throw "ERROR: file open for writing.";

	// flush current sector if needed
	if (mode == READWRITE)
		flushFile();

	// finds the appropiate sector needed from memory according to the first paramter
	if (relativeTo == BEGIN)
	{
		// check for underflow
		if (bytes < 0)
			bytes = 0;
		// check for overflow
		if (bytes > fileDesc.fileSize)
			bytes = fileDesc.fileSize;

		// compute the amount of sectors needed to move from current sector
		sectors = bytes / 1020 - currByte / 1020;

		// update the data pointer 
		currByte = bytes;
		currByteInBuff = bytes % 1020;
	}
	else if (relativeTo == CURRENT)
	{
		// check for underflow
		if (currByte + bytes < 0)
			bytes = -1 * currByte;
		// check for overflow
		if (currByte + bytes > fileDesc.fileSize)
			bytes = fileDesc.fileSize - currByte;

		// compute the amount of sectors needed to move from current sector
		sectors = (bytes - currByteInBuff) / 1020;

		// update the data pointer 
		currByte += bytes;
		currByteInBuff = currByte % 1020;
	}
	else if (relativeTo == END)
	{
		// check for underflow
		if (bytes < -1 * fileDesc.fileSize)
			bytes = -1*fileDesc.fileSize;
		// check for overflow
		if (bytes > 0)
			bytes = 0;

		// compute the amount of sectors needed to move from current sector
		sectors = (bytes - (fileDesc.fileSize - currByte))/1020;

		// update the data pointer 
		currByte = fileDesc.fileSize - bytes;
		currByteInBuff = currByte % 1020;
	}

	// move forward
	if (sectors > 0)
	{
		while (sectors)
		{
			if (sectors % 2) // first sector in cluster
				currSecNr++;	
			else			// second sector in cluster -- move on to next cluster
				while (!FAT[currSecNr++ / 2]);
			sectors--;
		}
	}
	else // move backwards
	{
		if (!sectors % 2) // second sector in cluster -- move back one sector
			currSecNr--;
		else              // move on to previous cluster
			while (!FAT[currSecNr-- / 2]);
		sectors++;
	}
}


void FCB::remove()
{
	cout << "Class: FCB, Function: delete file" << endl;

}

bool FCB::eof()
{
	if (mode == NONE)
		throw "ERROR: no file associated yet.";
	if (mode == WRITE)
		throw "ERROR: file is open for writing.";
	if (currByte == fileDesc.fileSize)
		return true;
	return false;
}

void FCB::print()
{
	if (mode == NONE)
		throw "ERROR: nothing to print yet, no file associated.";

	cout << "File information:" << endl;
	cout << "---------------------" << endl;
	cout << "Name: " << fileDesc.fileName << endl;
	cout << "Author: " << fileDesc.fileOwner << endl;
	cout << "Created on: " << string(fileDesc.crDate, 8) << endl;
	cout << "Size: " << fileDesc.fileSize << " bytes" << endl;
	cout << "Size on disk: " << (fileDesc.fileSectors + 1) * 1024 << " bytes" << endl << endl;

	cout << "File Allocation Table: " << endl;
	cout << "----------------------" << endl;
	cout << FAT << endl << endl;

	cout << "File Data Information: " << endl;
	cout << "----------------------" << endl;
	cout << "File mode: ";
	switch (mode)
	{
	case READ:
		cout << "read only";
		break;
	case WRITE:
		cout << "write";
		break;
	case APPEND:
		cout << "append";
		break;
	case READWRITE:
		cout << "read & write";
		break;
	default:
		break;
	}
	cout << endl;

	cout << "Byte in file: " << currByte << endl;
	cout << "Byte in buffer: " << currByteInBuff << endl;
	cout << "Buffer #: " << currByteInBuff <<endl;
	cout << "Cluster #: " << currSecNr << endl;
}
