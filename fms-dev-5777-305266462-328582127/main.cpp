/*
	Chaim Rottenberg - 305266462
	Chasky Hoffmann - 328582127

	chaskyhoffmann@gmail.com
	chaimke77@gmail.com
*/
#include "disk.h"
#include "fcb.h"

void step1();

string diskName;
string ownerName;
string fileName;
//uint size;
char c;
uint secctorNr;
char buffer[1024];
Disk* ex = nullptr;
Sector* sec;
DATtype fat;
FCB* fcb;
int sel;
VHD * v;
int outerChoice = 1;
int innerChoice;

int main() {
	

	cout << "Testing program for Mini Project in File Management." << endl;
	cout << "Chaim Rottenberg & Chasky Hoffmann ---- March-May 2017" << endl;

	
	while (outerChoice)
	{
		cout << "Select the step to test: " << endl;
		cout << "\t1: Step #1 - physical disk" << endl;
		cout << "\t2: Step #2 - logical disk" << endl;
		cout << "\t3: Step #3 - physical files" << endl;
		cout << "\t4: Step #4 - logical files" << endl;
		cout << "\t5: Step #5 - DMS" << endl;
		cout << "\t0: exit" << endl;

		cin >> outerChoice;
		innerChoice = 1;

		if (ex == nullptr)
			ex = new Disk();

		switch (outerChoice)
		{
			
		case 1:	// physical disk
			step1();
			break;
		
		case 2: // logical disk
			int size;
			cout << "-------Step #2--------" << endl;
			while (innerChoice)
			{
				//MENU
				cout << "Select the function to test:" << endl;
				cout << "\t1:  format disk" << endl;
				cout << "\t2:  empty cluster count" << endl;
				cout << "\t3:  allocate memory" << endl;
				cout << "\t4:  extend memory allocation" << endl;
				cout << "\t5:  deallocate memory" << endl;
				cout << "\t6:  print disk DAT" << endl;
				cout << "\t0: back" << endl;

				cin >> innerChoice;

				switch (innerChoice)
				{
				case 1: // format
					cout << "enter owners name: ";
					cin >> ownerName;
					try 
					{
						ex->format(ownerName);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 2: // empty cluster count
					try
					{
						cout << "count: " << ex->howMuchIsEmpty() << endl;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 3: // allocate memory
					cout << "enter the amount of sectors needed: ";
					cin >> secctorNr;
					fat.reset();
					try
					{
						ex->alloc(fat, secctorNr);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 4: // extend allocation
					cout << "enter the amount of sectors to add: ";
					cin >> secctorNr;
					try
					{
						ex->allocExtend(fat,secctorNr);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}

					break;

				case 5: // dealloc
					try
					{
						ex->dealloc(fat);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;
				case 6: // print dat
					try
					{
						fms::operator<<(cout, ex->dat.table);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 0:
					break;

				default:
					break;
				}
			}
			break; 
		case 3: // physical files
			cout << "-------Step #3--------" << endl;
			while (innerChoice)
			{
				//MENU
				cout << "Select the function to test:" << endl;
				cout << "\t1:  create file" << endl;
				cout << "\t2:  delete file" << endl;
				cout << "\t3:  extend file" << endl;
				cout << "\t4:  cut file" << endl;
				cout << "\t5:  print fat" << endl;
				cout << "\t6:  print root directory" << endl;
				cout << "\t0:  back" << endl;

				cin >> innerChoice;

				switch (innerChoice)
				{
				case 1: // create file
					cout << "enter file name: ";
					cin >> fileName;
					cout << "enter owners name: ";
					cin >> ownerName;
					cout << "enter file size (in Bytes): ";
					cin >> size;
					try
					{
						ex->createFile(fileName,ownerName,size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 2: // delete file
					cout << "enter file name: ";
					cin >> fileName;
					cout << "enter owners name: ";
					cin >> ownerName;
					try
					{
						ex->delFile(fileName, ownerName);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 3: // extend file
					cout << "enter file name: ";
					cin >> fileName;
					cout << "enter owners name: ";
					cin >> ownerName;
					cout << "enter the amount to add (in Bytes): ";
					cin >> size;
					try
					{
						ex->extendFile(fileName,ownerName,size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 4: // cut file
					cout << "enter file name: ";
					cin >> fileName;
					cout << "enter owners name: ";
					cin >> ownerName;
					cout << "enter the amount to cut (in Bytes): ";
					cin >> size;
					try
					{
						ex->cutFile(fileName, ownerName, size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}

					break;

				case 5: // print fat
					try
					{
						fms::operator<<(cout, fat);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 6:
					try
					{
						cout << ex->rootDir << endl;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;
				case 0: // exit
					break;

				default:
					break;
				}
			}
			break;

		case 4: // logical files
			cout << "-------Step #4--------" << endl;
			while (innerChoice)
			{
				//MENU
				cout << "Select the function to test:" << endl;
				cout << "\t1:  FCB default constructor" << endl;
				cout << "\t2:  FCB constructor" << endl;
				cout << "\t3:  open file" << endl;
				cout << "\t4:  close file" << endl;
				cout << "\t5:  flush file" << endl;
				cout << "\t6:  read" << endl;
				cout << "\t7:  write" << endl;
				cout << "\t8:  seek" << endl;
				cout << "\t9:  delete" << endl;
				cout << "\t10: eof" << endl;
				cout << "\t11: print FCB" << endl;
				cout << "\t-1: back" << endl;
				cout << "\t0:  exit" << endl;
				
				cin >> innerChoice;

				switch (innerChoice)
				{
				case 1: // default constructor
					try
					{
						fcb = new FCB();
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 2: // constructor for disk
					try
					{
						fcb = new FCB(ex);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 3: // open file
					cout << "file name: ";
					cin >> fileName;
					cout << "owner name: ";
					cin >> ownerName;
					cout << "select mode: (0) none, (1) read, (2) write, (3) read-write, (6) append: ";
					cin >> sel;
					try
					{
						fcb->openFile(ex, diskName, ownerName, sel);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 4: // close file
					try
					{
						fcb->closeFile();
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}

					break;

				case 5: // flush file
					try
					{
						fcb->flushFile();
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 6: // read
					cout << "bytes to read: ";
					cin >> size;
					try
					{
						fcb->read(buffer, size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					cout << buffer << endl;
					break;

				case 7: // write
					cout << "bytes to write: ";
					cin >> size;
					cout << "data to write: ";
					cin >> buffer;
					try
					{
						fcb->write(buffer, size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 8: // seek
					cout << "enter the seek mode (0) begin, (1) current, (2) end: ";
					cin >> sel;
					cout << "enter offset: ";
					cin >> size;
					try
					{
						fcb->seek(sel,size);
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 9: // delete
					try
					{
						fcb->remove();
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;

				case 10: // eof
					sec = new Sector();
					try
					{
						cout << (fcb->eof() ? "YES" : "NO") << endl;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					break;
				case 11:
					fcb->print();
					break;
				case -1: // go back
					innerChoice = 0;
					break;
				case 0: // exit
					outerChoice = 0;
					cout << "Bye Bye!" << endl;
					break;
				default:
					break;
				}
			}
			break; 
		case 5: // dms
			

			fat.set();
			for (int i = 0; i < 1600; i++)
				cout << fat[i] << " ";

			break;
		case 0: // exit program
			cout << "do you wnat to unmount disk (y or n)? ";
			cin >> c;
			if (c == 'y' || c == 'Y')
				ex->unmountDisk();
			cout << "Bye Bye!" << endl;
			break;
		default:
			cout << "that wasn't a valid choice." << endl;
			break;
		}
	}

	return 0;
}

void step1()
{
	cout << "-------Step #1--------" << endl;
	while (innerChoice)
	{
		// MENU
		cout << "Select the function to test:" << endl;
		cout << "\t1:  default Disk constructor" << endl;
		cout << "\t2:  Disk constructor with 3 parameters" << endl;
		cout << "\t3:  mount disk" << endl;
		cout << "\t4:  unmount disk" << endl;
		cout << "\t5:  recreate disk" << endl;
		cout << "\t6:  seek to sector" << endl;
		cout << "\t7:  write to current sector" << endl;
		cout << "\t8:  write to specific sector" << endl;
		cout << "\t9: read current sector" << endl;
		cout << "\t10: read specific sector" << endl;
		cout << "\t11: print disk" << endl;
		cout << "\t12: print vhd" << endl;
		cout << "\t0:  back" << endl;

		cin >> innerChoice;
		
		if (ex == nullptr)
			ex = new Disk();

		switch (innerChoice)
		{
		case 1: // default constructor
			try
			{
				ex = new Disk();
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 2: // constructor
			cout << "disk name: ";
			cin >> diskName;
			cout << "owner name: ";
			cin >> ownerName;
			cout << "(c) create and mount, (m) mount: ";
			cin >> c;

			try
			{
				ex = new Disk(diskName, ownerName, c);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			catch (...)
			{
				cout << " bad" << endl;
			}
			break;

		case 3: // mount disk
			cout << "disk name: ";
			cin >> diskName;
			try
			{
				ex->mountDisk(diskName);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}

			break;

		case 4: // unmount disk
			try
			{
				ex->unmountDisk();
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 5: // recreate disk
			cout << "owner name: ";
			cin >> ownerName;
			try
			{
				ex->recreateDisk(ownerName);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 6: // seek to sector
			cout << "enter the sector to seek to: ";
			cin >> secctorNr;
			try
			{
				ex->seekToSector(secctorNr);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 7: // write to current sector
			cout << "enter characters to write: ";
			cin.ignore();
			cin.getline(buffer, 1020);
			sec = new Sector();
			sec->write(buffer, 0, 1020);
			try
			{
				ex->writeSector(sec);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 8: // write to specific sector
			cout << "enter characters to write: ";
			cin.ignore();
			cin.getline(buffer, 1020);
			cout << "enter sector number: ";
			cin >> secctorNr;
			sec = new Sector();
			sec->write(buffer, 0, 1020);

			try
			{
				ex->writeSector(secctorNr, sec);
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;

		case 9: // read current sector
			sec = new Sector();
			try
			{
				ex->readSector(sec);
				cout << *sec;
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;
		case 10: // read specific sector
			cout << "enter the sector number to read from: ";
			cin >> secctorNr;
			sec = new Sector();
			try
			{
				ex->readSector(secctorNr, sec);
				cout << *sec;
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;
		case 11: // print disk
			try {
				ex->print();
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
			break;
		case 12: // print vhd
			try {
				cout << ex->vhd << endl;
			}
			catch (const char* msg) {
				cout << msg << endl;
			}
		case 0: // go back
			break;
		default:
			break;
		}
	}
}
