/*
 * FRAMAccess.h
 *
 *  Created on: June 26, 2020
 *      Author: Zhuoheng
 *
 */

#ifndef FRAMACCESS_H_
#define FRAMACCESS_H_

#define FRAMSTATUS          4995 // written or not
#define FILENUMBER_OFFSET   4999 // Current file number
#define FRAMLIST_OFFSET     5000
#define FRAMLIST_LENGTH     100 // Maximum number of files
#define FRAMLIST_SIZE       1000

#include "MB85RS.h"

// Users can expand this definition to include more files
typedef enum FileName {ADBTelemetryFile, ADCSTelemetryFile, COMMSTelemetryFile,
                       EPSTelemetryFile, PROPTelemetryFile, OBCVariableFile} FileName;

class FRAMAccess
{
protected:
    MB85RS &fram;
    char currentFileNumber;
    struct framList
    {
        unsigned long startAddress;
        unsigned long size;
        char fileIndex; // range: 0 ~ (FRAMLIST_LENGTH-1)
        bool status; // written or not
    } framList[FRAMLIST_LENGTH];

    int ReadList();
    int WriteList();

public:
    FRAMAccess(MB85RS &hardware) : fram(hardware) {};
    int Init();
    int Write(char file, unsigned long fileSize);
    int Read(char file, unsigned long fileSize);
    int Delete(char file);
};

#endif /* FRAMACCESS_H_ */
