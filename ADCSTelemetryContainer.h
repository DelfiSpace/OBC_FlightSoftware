/*
 * EPSTelemetryContainer.h
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#ifndef ADCSTELEMETRYCONTAINER_H_
#define ADCSTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define ADCS_CONTAINER_SIZE  26

class ADCSTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[ADCS_CONTAINER_SIZE];

public:
    virtual int size();
    virtual unsigned char * getArray();

    unsigned long getUpTime();
    void setUpTime(unsigned long ulong);

    signed short getTemperature();
    void setTemperature(signed short ushort);
    bool getTmpStatus();
    void setTmpStatus(bool bval);

    signed short getBusCurrent();
    void setBusCurrent(signed short ushort);
    unsigned short getBusVoltage();
    void setBusVoltage(unsigned short ushort);
    bool getBusStatus();
    void setBusStatus(bool bval);

    signed short getTorquerXCurrent();
    void setTorquerXCurrent(signed short ushort);
    unsigned short getTorquerXVoltage();
    void setTorquerXVoltage(unsigned short ushort);
    bool getTorquerXStatus();
    void setTorquerXStatus(bool bval);

    signed short getTorquerYCurrent();
    void setTorquerYCurrent(signed short ushort);
    unsigned short getTorquerYVoltage();
    void setTorquerYVoltage(unsigned short ushort);
    bool getTorquerYStatus();
    void setTorquerYStatus(bool bval);

    signed short getTorquerZCurrent();
    void setTorquerZCurrent(signed short ushort);
    unsigned short getTorquerZVoltage();
    void setTorquerZVoltage(unsigned short ushort);
    bool getTorquerZStatus();
    void setTorquerZStatus(bool bval);
};

#endif /* ADCSTELEMETRYCONTAINER_H_ */
