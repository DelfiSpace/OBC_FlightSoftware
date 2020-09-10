/*
 * TelemetryRequestService.h
 *
 *  Created on: 10 Sep 2020
 *      Author: casperbroekhuizen
 */

#ifndef TELEMETRYREQUESTSERVICE_H_
#define TELEMETRYREQUESTSERVICE_H_

#include "Service.h"
#include "LittleFS.h"
#include "SDCard.h"
#include "Telemetry/EPSTelemetryContainer.h"

#define TELEMETRY_REQUEST_SERVICE               80
#define TELEMETRY_REQUEST_UNKNOWN_CMD           55

class TelemetryRequestService: public Service
{
protected:
    LittleFS *fs;
    SDCard *sdcard;

    LFSTask requestTask;
    lfs_file_t requestFile;
    uint8_t requestFileBuffer[512];
    const struct lfs_file_config requestFilecfg = {.buffer=requestFileBuffer};

    EPSTelemetryContainer EPSbuffer;



public:
    TelemetryRequestService(LittleFS &fs_in, SDCard &sdcard_in);

    virtual bool process( DataMessage &command, DataMessage &workingBbuffer );

    void getTelemetry(unsigned long targetUptime);
};



#endif /* TELEMETRYREQUESTSERVICE_H_ */
