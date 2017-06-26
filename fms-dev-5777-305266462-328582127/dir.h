#pragma once
#include "fms.h"
#include "sector.h"
#include "dirEntry.h"

class Dir :public Sector
{
public:
	EntryArray& entrys = reinterpret_cast<EntryArray&>(rawData[0]);
};