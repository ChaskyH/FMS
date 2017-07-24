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


void FCB::openFile(Disk * dsk, string & name, string & owner = string() , uint mode = READ)
{
	if (d != nullptr)
		throw "ERROR: this file control is already in use.";
	dsk->openFile(*this, name, owner, mode);


	if (mode == WRITE) // trunc file
	{
		fileDesc.fileSize = 0;
		currByte = 0;
		currByteInBuff = 0;		
	}
	if (mode == APPEND) // move to eof
	{
		for (int i = 0; i < fileDesc.fileSize; i += BUFFER_SIZE)
		{
			if (currSecNr % 2)
				currSecNr++;
			else
				while (!FAT[++currSecNr / 2]);
		}
		currByte = fileDesc.fileSize;
		currByteInBuff = currByte % 1020;
	}
	if (mode == READ || mode == READWRITE) // read and write at beggining of file
	{
		currByte = 0;
		currByteInBuff = 0;
		currSecNr = fileDesc.fileAddr + 1;
		d->readSector(fileDesc.fileAddr + 1,&buffer);
	}
}

void FCB::closeFile()
{
	if (mode == NONE)
		throw "ERROR: there is no open file to close.";
	if(mode != READ)
		flushFile();

	// update fhd fields 
	FHD fhd(fileDesc, FAT);
	fhd.sectorNr = fileDesc.fileAddr;
	d->writeSector(fileDesc.fileAddr, &fhd);

	// update rootdir entry
	d->rootDir.updateEntry(fileDesc);

	if(mode == APPEND || mode == WRITE)
		remove();

	d = nullptr;
}

void FCB::flushFile()
{
	d->writeSector(currSecNr, &buffer);
}

uint FCB::read(char* dst, uint size = 0)
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
				if (mode == READWRITE) // flush file if neccesary
					flushFile();

				while (!FAT[++currSecNr / 2]); // move to next sector
				d->readSector(currSecNr, &buffer);
				currByteInBuff = 0;
			}
			dst[i] = buffer.rawData[currByteInBuff];
		}
	}
	else                                         // no size specified
	{
		while (buffer.rawData[currByteInBuff] != '\n' && size > 200 && !eof()) 
		{
			if (currByteInBuff == BUFFER_SIZE)
			{
				if (mode == READWRITE) // flush file if neccesary
					flushFile();

				while (!FAT[++currSecNr / 2]); // move to next sector
				d->readSector(currSecNr, &buffer);
				currByteInBuff = 0;
			}
			dst[size] = buffer.rawData[currByteInBuff];
			currByte++, currByteInBuff++, size++;
		}
	}

	dst[size] = '\0'; // enter null character at end of dst
	return size;
}

void FCB::write(char * src, uint size)
{
	// error handling
	if (mode == NONE)
		throw "ERROR: no file associated yet.";
	if (mode != WRITE && mode != READWRITE && mode != APPEND)
		throw "ERROR: the file isn't in edit mode.";

	// when size isn't specified
	if (size == 0)
	{
		size = strlen(src);
		src[size] = '\n';
		src[++size] = '\0';
	}

	// check if there is enough space
	if (mode == WRITE || mode == APPEND)
	{
		if (currByte + size > fileDesc.fileSectors*BUFFER_SIZE)
		{
			d->extendFile(string(fileDesc.fileName), string(fileDesc.fileOwner), size);
		}
	}


	for (int i = 0; i < size; i++, currByte++, currByteInBuff++)
	{
		if (currByteInBuff == BUFFER_SIZE)
		{
			flushFile();
			currByteInBuff = 0;
			while (!FAT[++currSecNr / 2]); // move to next sector
		}

		if (mode == APPEND || mode == WRITE)	// write mode
		{
			fileDesc.fileSize++;
		}
	}
	
}

void FCB::seek(uint relativeTo, int bytes)
{
	int sectors; // the amount of sectors to move

	// error handling
	if (mode == NONE)
		throw "ERROR: no file assosiated yet.";
	if (mode == WRITE || mode == APPEND)
		throw "ERROR: file open for writing.";

	// flush current sector if needed
	if (mode == READWRITE && bytes + currByteInBuff > BUFFER_SIZE)
		flushFile();

	// finds the appropriate sector needed from memory according to the first parameter
	if (relativeTo == BEGIN)
	{
		// check for underflow
		if (bytes < 0)
			bytes = 0;
		// check for overflow
		if (bytes > fileDesc.fileSize)
			bytes = fileDesc.fileSize;

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

		// update the data pointer 
		currByte = fileDesc.fileSize - bytes;
		currByteInBuff = currByte % 1020;
	}


	// compute the amount of sectors to move
	if (bytes > 0)
	{
		bytes -= (BUFFER_SIZE - currByteInBuff);
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
	d->cutFile(string(fileDesc.fileName), string(fileDesc.fileOwner), fileDesc.fileSectors*BUFFER_SIZE - fileDesc.fileSize);
}

bool FCB::eof()
{
	if (mode == NONE)
		throw "ERROR: no file associated yet.";
	if (mode == WRITE || mode == APPEND)
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
