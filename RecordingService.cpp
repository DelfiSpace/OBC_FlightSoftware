/*
 * RecordingService.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: stefanosperett
 */

#include "RecordingService.h"
#include <stdio.h>

extern LittleFS fs;
extern SDCard sdcard;
extern unsigned long uptime;

RecordingService::RecordingService(volatile bool* flag){
    recordingFlag = flag;
}

bool RecordingService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getPayload()[0] == RECORDING_SERVICE && command.getPayload()[1] == 1)
     {
        workingBuffer.getPayload()[0] = RECORDING_SERVICE;
        workingBuffer.getPayload()[1] = 2; //reply
        workingBuffer.setSize(2);

        //Console::log("RecordingService");

         if (command.getPayload()[2] == 0)
         {
             //disable recording
             *recordingFlag = false;
             Console::log("Recording Disabled");
         }else if (command.getPayload()[2] == 1)
         {
             //enable recording
             *recordingFlag = true;
             Console::log("Recording Enabled");
         }else if (command.getPayload()[2] == 2)
         {
             //reply with Housekeeping frame given by long
             unsigned long targetUptime;
             ((unsigned char *)&targetUptime)[3] = command.getPayload()[3];
             ((unsigned char *)&targetUptime)[2] = command.getPayload()[4];
             ((unsigned char *)&targetUptime)[1] = command.getPayload()[5];
             ((unsigned char *)&targetUptime)[0] = command.getPayload()[6];

             char namebuf[50];
             int got_len = snprintf(namebuf, sizeof(namebuf), "LOG/%d/%d/EPS_%d", targetUptime/1000, targetUptime/100, targetUptime);

             Console::log("Target: %s", namebuf);

             int error = fs.file_open(&fs.workfile, namebuf, LFS_O_RDWR | LFS_O_CREAT);

             if(error){
//                 Console::log("File open Error: -%d", -error);
                 fs.file_close(&fs.workfile);
             }else{
                 //no error, return telemetry while mimicking EPS
                 int telemetrySize = fs.file_size(&fs.workfile);
//                 Console::log("No Error: -%d", -error);
                 if(telemetrySize > 0){
                     // Console::log("TelemetrySize: %d", telemetrySize);

                     fs.file_read(&fs.workfile, &workingBuffer.getPayload()[2], telemetrySize);

                     //the frame should not be protected, so going backwards into the payload start pointer allows us to hack the frame
                     workingBuffer.getPayload()[-1] = 2; //source = EPS
                     workingBuffer.getPayload()[0] = 3; //housekeeping service
                     workingBuffer.setSize(telemetrySize+2);
                 }
                 fs.file_close(&fs.workfile);
             }
         }else if (command.getPayload()[2] == 3){
             //reset uptime
             Console::log("Format sd");
             fs.unmount();
             fs.format(&sdcard);
             fs.mount_async(&sdcard);
             uptime = 0;
             //enable recording
             *recordingFlag = true;
         }else if (command.getPayload()[2] == 4){
             //reset uptime
             Console::log("Get Uptime");
             Console::log("Uptime: %d", uptime);
             //enable recording
             workingBuffer.getPayload()[0] = RECORDING_SERVICE;
             workingBuffer.getPayload()[1] = 2; //reply
             workingBuffer.getPayload()[2] = ((unsigned char *)&uptime)[3];
             workingBuffer.getPayload()[3] = ((unsigned char *)&uptime)[2];
             workingBuffer.getPayload()[4] = ((unsigned char *)&uptime)[1];
             workingBuffer.getPayload()[5] = ((unsigned char *)&uptime)[0];
             workingBuffer.setSize(6);
         }

         return true;
     }
     return false;
}


