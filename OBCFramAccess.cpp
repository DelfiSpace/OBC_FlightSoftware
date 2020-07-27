/*
 * OBCFRAMAccess.cpp
 *
 *  Start address + 0~3     Used to identify whether the block is written
 *                          (1: written, other values: not written)
 *  Start address + 4       Size of the array
 *  Start address + 5~      The array
 *
 *  Created on: June 26, 2020
 *      Author: Zhuoheng
 *
 */

#include "OBCFramAccess.h"

int OBCFramRead(MB85RS &fram, unsigned long startAddress, unsigned char *array, int arraySize)
{
    unsigned long written;
    unsigned char size;

    // Check whether the FRAM is available
    if (fram.ping() == false)
    {
        return FRAM_NOT_AVAILABLE;
    }

    // Check whether the block is available
    fram.read(startAddress, (unsigned char *)&written, 4);
    if (written != 1)
    {
        return FRAM_NOT_WRITTEN;
    }

    // Check the size of the block
    fram.read(startAddress + 4, &size, 1);
    if (size != arraySize)
    {
        return FRAM_WRONG_SIZE;
    }

    // Read the block
    fram.read(startAddress + 5, array, arraySize);

    return FRAM_OPERATION_SUCCESS;
}

int OBCFramWrite(MB85RS &fram, unsigned long startAddress, unsigned char *array, int arraySize)
{
    unsigned long written = 1;

    // Check whether the FRAM is available
    if (fram.ping() == false)
    {
        return FRAM_NOT_AVAILABLE;
    }

    // Check the size of the block
    if (arraySize > 295 || arraySize == 0) // 295 is the maximum size of an array at this moment
    {
        return FRAM_WRONG_SIZE;
    }
    else
    {
        fram.write(startAddress + 4, (unsigned char *)&arraySize, 1);
        fram.write(startAddress + 5, array, arraySize);
        fram.write(startAddress, (unsigned char *)&written, 4);
        return FRAM_OPERATION_SUCCESS;
    }
}
