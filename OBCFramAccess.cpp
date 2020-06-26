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

int OBCFramRead(MB85RS &fram, unsigned long startAddress, unsigned char *array, unsigned char arraySize)
{
    unsigned long written;
    char size;

    // Check whether the FRAM is available
    if (fram.ping() == false)
    {
        return FRAM_NOT_AVAILABLE;
    }

    // Check whether the block is available
    fram.read(startAddress, &written, 4);
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

int OBCFramWrite(MB85RS &fram, unsigned long startAddress, unsigned char *array, unsigned char arraySize)
{
    // Check whether the FRAM is available
    if (fram.ping() == false)
    {
        return FRAM_NOT_AVAILABLE;
    }

    // Check the size of the block
    if (size > 195 || size == 0)
    {
        return FRAM_WRONG_SIZE;
    }
    else
    {
        fram.write(startAddress + 4, &arraySize, 1);
        fram.write(startAddress + 5, array, arraySize);
        fram.write(startAddress, "1", 1);
        return FRAM_OPERATION_SUCCESS;
    }
}



