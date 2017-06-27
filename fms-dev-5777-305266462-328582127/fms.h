#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <time.h>
#include <iomanip>
using namespace std;

class DirEntry;

using Name = char[12];
using Date = char[8];
using uint = unsigned int;
using EntryArray = DirEntry[21];
using DATtype = bitset<1600> ;

namespace fms
{
	const int NONE = 0;
	const int READ = 1;
	const int WRITE = 2;
	const int READWRITE = 3;
	const int APPEND = 6;

	const int BEGIN = 0;
	const int CURRENT = 1;
	const int END = 2;

	const int EMPTY = 0;
	const int ACTIVE = 1;
	const int INACTIVE = 2;

	const int BUFFER_SIZE = 1024;
	const int NUM_CLUSTERS = 1600;

	
	
	namespace {
		void stam() {}
		string fmsGetTimeFormat(time_t c = time(0))
		{
			struct tm t;
			localtime_s(&t, &c);
			char buffer[10];
			strftime(buffer, 10, "%Y%m%d", &t);
			string time = buffer;
			return time;
		}
		ostream& operator<< (ostream& os, DATtype& dat)
		{
			os << "     ";
			for (int i = 0; i < 32; i++)
				os << hex << uppercase << setw(2) << setfill('0') << i << "|" << dec;
			os << endl;
			for (int i = 0; i < 1600; i += 32)
			{
				os << hex << setw(3) << setfill('0') << i << " | " << dec;

				for (int j = 0; j < 32; j++)
				{ 
					os << setw(0) << setfill('0') << dat[i + j] << "| ";
				} 
				os << endl;
			}
			os << setw(0) << setfill(' ');
			return os;
		}
	}
}


