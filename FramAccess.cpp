/*
 * FRAMAccess.cpp
 *
 *  Created on: June 26, 2020
 *      Author: Zhuoheng
 *
 */

#include "FRAMAccess.h"

int FRAMAccess::ReadList()
{
    fram.read(FILENUMBER_OFFSET, (unsigned char *)framList, FRAMLIST_SIZE);
    fram.read(FRAMLIST_OFFSET, (unsigned char *)framList, FRAMLIST_SIZE);
}

int FRAMAccess::WriteList()
{

}

int FRAMAccess::Init()
{
    fram.init();
}

int FRAMAccess::Write(char file, unsigned long fileSize)
{

}

int FRAMAccess::Read(char file, unsigned long fileSize)
{

}

