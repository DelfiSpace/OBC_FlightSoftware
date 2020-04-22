/*
 * EPSTelemetryContainer.h
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#ifndef OBCTELEMETRYCONTAINER_H_
#define OBCTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define ADCS_CONTAINER_SIZE  26

class OBCTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[ADCS_CONTAINER_SIZE];

public:
    virtual int size();
    virtual unsigned char * getArray();

    unsigned long getUpTime();
    void setUpTime(unsigned long ulong);

    bool getTmpStatus();
    void setTmpStatus(bool bval);

    signed short getBusCurrent();
    void setBusCurrent(signed short ushort);
    unsigned short getBusVoltage();
    void setBusVoltage(unsigned short ushort);
    bool getBusStatus();
    void setBusStatus(bool bval);
};

#endif /* OBCTELEMETRYCONTAINER_H_ */
