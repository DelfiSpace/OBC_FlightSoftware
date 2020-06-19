/*
 * TestService.h
 *
 *  Created on: 6 Aug 2019
 *      Author: stefanosperett
 */

#ifndef RECORDSERVICE_H_
#define RECORDSERVICE_H_

#include "Service.h"
#include "LittleFS.h"
#include "Console.h"

#define RECORDING_SERVICE 123

class RecordingService: public Service
{
private:
    volatile bool* recordingFlag;
 public:
    RecordingService(volatile bool* flag);
     virtual bool process( DataMessage &command, DataMessage &workingBbuffer );
};
#endif /* TESTSERVICE_H_ */
