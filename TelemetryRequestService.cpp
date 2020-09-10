/*
 * TelemetryRequestService.cpp
 *
 *  Created on: 10 Sep 2020
 *      Author: casperbroekhuizen
 */

#include "TelemetryRequestService.h"
#include <stdio.h>

TelemetryRequestService::TelemetryRequestService(LittleFS &fs_in, SDCard &sdcard_in){
    fs = &fs_in;
    sdcard = &sdcard_in;

    requestFile.cfg = &requestFilecfg;
    requestTask.taskFile = &requestFile;
    requestTask.taskCompleted = false; //initialize with completed, as it is not running.
};

bool TelemetryRequestService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getService() == TELEMETRY_REQUEST_SERVICE)
    {
//        Console::log("RadioService: Service started");
        workingBuffer.setService(TELEMETRY_REQUEST_SERVICE);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        switch(command.getDataPayload()[0])
        {
        case 1:
            unsigned long targetUptime;
            ((unsigned char *)&targetUptime)[3] = command.getDataPayload()[1];
            ((unsigned char *)&targetUptime)[2] = command.getDataPayload()[2];
            ((unsigned char *)&targetUptime)[1] = command.getDataPayload()[3];
            ((unsigned char *)&targetUptime)[0] = command.getDataPayload()[4];
            Console::log("TelemetryService: getTelemetry of targetUptime: %d", targetUptime);
            getTelemetry(targetUptime);
            break;
        case 2:
            Console::log("TelemetryService: Check Completed?  : %s", requestTask.taskCompleted ? "OPEN":"CLOSED");
            break;
        case 3:
            Console::log("TelemetryService: Check Result?  : -%d", -requestTask.taskResult);
            if(requestTask.taskCompleted && requestTask.taskResult == 0){
                fs->file_read(&requestFile, EPSbuffer.getArray(), 111);
                Console::log("EPSTelemetry at %d, Voltage : %d mV, BootCounter :%d",EPSbuffer.getUptime(), EPSbuffer.getBatteryGGVoltage(), EPSbuffer.getBootCounter());
                fs->file_close(&requestFile);
            }
            break;
        default:
            Console::log("TelemetryService: Unknown command!");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = TELEMETRY_REQUEST_UNKNOWN_CMD;
            break;
        }

        return true;
    }
    else
    {
        // this command is related to another service,
        // report the command was not processed
        return false;
    }
}

void TelemetryRequestService::getTelemetry(unsigned long targetUptime){
    requestTask.taskOperation = FileSystemOperation::Open;
    requestTask.taskCompleted = false;
    requestTask.taskSize = 0; //unnecessary for open
    requestTask.taskFlags = LFS_O_RDONLY;
    requestTask.taskArray = 0; //unnecessary for open
    snprintf(requestTask.taskNameBuf, sizeof(requestTask.taskNameBuf), "LOG/%d/%d/EPS_%d", (unsigned long)targetUptime/100000,(unsigned long)targetUptime/1000, (unsigned long)targetUptime);
    Console::log("Opening File: %s", requestTask.taskNameBuf);
    fs->queTask(requestTask);
//    fs->file_open(&requestFile, requestTask.taskNameBuf, LFS_O_RDONLY);
//    requestTask.taskCompleted = true;
}


