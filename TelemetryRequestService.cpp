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
    if (command.getService() == TELEMETRY_REQUEST_SERVICE){
        Console::log("TelemetryService: Service started");
        workingBuffer.setService(TELEMETRY_REQUEST_SERVICE);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        switch(command.getDataPayload()[0]){
        case 1:
            if(command.getPayloadSize() == 5){
                Console::log("TelemetryService: Request File");
                unsigned long ulong;
                ((unsigned char *)&ulong)[3] = command.getDataPayload()[1];
                ((unsigned char *)&ulong)[2] = command.getDataPayload()[2];
                ((unsigned char *)&ulong)[1] = command.getDataPayload()[3];
                ((unsigned char *)&ulong)[0] = command.getDataPayload()[4];
                getTelemetry(ulong);
                workingBuffer.setPayloadSize(1);
                workingBuffer.getDataPayload()[0] = TELEMETRY_NO_ERROR;
            }else{
                workingBuffer.setPayloadSize(1);
                workingBuffer.getDataPayload()[0] = TELEMETRY_UNKNOWN_CMD;
            }
            break;
        case 2:
            Console::log("TelemetryService: Get Result");
            if(requestTask.taskCompleted){
                if(!requestTask.taskResult){
                    Console::log("TelemetryService: SUCCES RESULT!");
                    fs->file_read(&requestFile, totalTelemetryContainer, totalTelemetrySize);
                    if(command.getPayloadSize() == 1){
                        // SEND ALL

                        //is this possible or wanted?

                        workingBuffer.setPayloadSize(1);
                        workingBuffer.getDataPayload()[0] = TELEMETRY_NO_ERROR;

                        memcpy(EPSbuffer.getArray(), &totalTelemetryContainer[OBC_CONTAINER_SIZE], EPS_CONTAINER_SIZE);
                        Console::log("DEBUG REQ:: EPS CONTAINER UPTIME: %d", EPSbuffer.getUptime());


                    }else if(command.getPayloadSize() == 2 && command.getDataPayload()[1] < 5 && command.getDataPayload()[1] > 0){
                        // GET SUBSYSTEM TELEMETRY

                        //get Pointer to target system
                        uint8_t targetSystem = command.getDataPayload()[1];
                        int telemetryIndex = 0;
                        for(int j = 1; j < targetSystem; j++){
                            telemetryIndex += telemetrySizes[j-1];
                        }

                        Console::log("REQ CONTAINER INDEX: %d", telemetryIndex);

                        //copy telemetry into the reply:
                        workingBuffer.setPayloadSize(2 + telemetrySizes[targetSystem-1]);
                        workingBuffer.getDataPayload()[0] = TELEMETRY_NO_ERROR;
                        workingBuffer.getDataPayload()[1] = targetSystem;
                        memcpy(&workingBuffer.getDataPayload()[2], &totalTelemetryContainer[telemetryIndex], telemetrySizes[targetSystem-1]);

                        //Debug Test Print target total uptime (put in EPS as houskeepingheader is same size anyways
//                        memcpy(EPSbuffer.getArray(), &totalTelemetryContainer[telemetryIndex], 16);
//                        Console::log("DEBUG REQ:: TARGET (%d) CONTAINER UPTIME: %d", targetSystem, EPSbuffer.getUptime());

                    }else{
                        workingBuffer.setPayloadSize(1);
                        workingBuffer.getDataPayload()[0] = TELEMETRY_UNKNOWN_CMD;
                    }
                    fs->file_close(&requestFile);
                }else{
                    Console::log("TelemetryService: ERROR RESULT -%d :( ", -requestTask.taskResult);
                    workingBuffer.setPayloadSize(1);
                    workingBuffer.getDataPayload()[0] = TELEMETRY_ERROR;
                }
            }else{
                Console::log("TelemetryService: Not Done Yet!");
                workingBuffer.setPayloadSize(1);
                workingBuffer.getDataPayload()[0] = TELEMETRY_NOT_READY;
            }
            break;
        case 5:
            Console::log("TelemetryService: Format Sd");
            fs->unmount();
            fs->format(sdcard);
            fs->mount_async(sdcard);
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = 0;
            break;
        default:
            Console::log("TelemetryService: Unknown CMD");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = TELEMETRY_UNKNOWN_CMD;
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
    if(requestTask.taskFile->flags & LFS_F_OPENED){
        Console::log("File still open, close first");
        fs->file_close(requestTask.taskFile);
    }
    requestTask.taskOperation = FileSystemOperation::Open;
    requestTask.taskCompleted = false;
    requestTask.taskSize = 0; //unnecessary for open
    requestTask.taskFlags = LFS_O_RDONLY;
    requestTask.taskArray = 0; //unnecessary for open
    snprintf(requestTask.taskNameBuf, sizeof(requestTask.taskNameBuf), "LOG/%d/%d/TLM_%d", (unsigned long)targetUptime/100000,(unsigned long)targetUptime/1000, (unsigned long)targetUptime);
    Console::log("Opening File: %s", requestTask.taskNameBuf);
    fs->queTask(requestTask);
//    fs->file_open(&requestFile, requestTask.taskNameBuf, LFS_O_RDONLY);
//    requestTask.taskCompleted = true;
}


