/*
 * COMMSTelemetryContainer.h
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#ifndef COMMSTELEMETRYCONTAINER_H_
#define COMMSTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define COMMS_CONTAINER_SIZE  66
class COMMSTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[COMMS_CONTAINER_SIZE];

public:
    virtual int size();
    virtual unsigned char * getArray();

    unsigned long getUpTime();
    void setUpTime(unsigned long ulong);

    signed short getIntBCurrent();
    void setIntBCurrent(signed short ushort);
    unsigned short getIntBVoltage();
    void setIntBVoltage(unsigned short ushort);
    bool getIntBStatus();
    void setIntBStatus(bool bval);

    signed short getURBCurrent();
    void setURBCurrent(signed short ushort);
    unsigned short getURBVoltage();
    void setURBVoltage(unsigned short ushort);
    bool getURBStatus();
    void setURBStatus(bool bval);

    signed short getB1Current();
    void setB1Current(signed short ushort);
    unsigned short getB1Voltage();
    void setB1Voltage(unsigned short ushort);
    bool getB1Status();
    void setB1Status(bool bval);

    signed short getB2Current();
    void setB2Current(signed short ushort);
    unsigned short getB2Voltage();
    void setB2Voltage(unsigned short ushort);
    bool getB2Status();
    void setB2Status(bool bval);

    signed short getB3Current();
    void setB3Current(signed short ushort);
    unsigned short getB3Voltage();
    void setB3Voltage(unsigned short ushort);
    bool getB3Status();
    void setB3Status(bool bval);

    signed short getB4Current();
    void setB4Current(signed short ushort);
    unsigned short getB4Voltage();
    void setB4Voltage(unsigned short ushort);
    bool getB4Status();
    void setB4Status(bool bval);

    signed short getSAYpCurrent();
    void setSAYpCurrent(signed short ushort);
    unsigned short getSAYpVoltage();
    void setSAYpVoltage(unsigned short ushort);
    bool getSAYpStatus();
    void setSAYpStatus(bool bval);
    signed short getSAYpTemperature();
    void setSAYpTemperature(signed short ushort);
    bool getSAYpTmpStatus();
    void setSAYpTmpStatus(bool bval);

    signed short getSAYmCurrent();
    void setSAYmCurrent(signed short ushort);
    unsigned short getSAYmVoltage();
    void setSAYmVoltage(unsigned short ushort);
    bool getSAYmStatus();
    void setSAYmStatus(bool bval);
    signed short getSAYmTemperature();
    void setSAYmTemperature(signed short ushort);
    bool getSAYmTmpStatus();
    void setSAYmTmpStatus(bool bval);

    signed short getSAXpCurrent();
    void setSAXpCurrent(signed short ushort);
    unsigned short getSAXpVoltage();
    void setSAXpVoltage(unsigned short ushort);
    bool getSAXpStatus();
    void setSAXpStatus(bool bval);
    signed short getSAXpTemperature();
    void setSAXpTemperature(signed short ushort);
    bool getSAXpTmpStatus();
    void setSAXpTmpStatus(bool bval);

    signed short getSAXmCurrent();
    void setSAXmCurrent(signed short ushort);
    unsigned short getSAXmVoltage();
    void setSAXmVoltage(unsigned short ushort);
    bool getSAXmStatus();
    void setSAXmStatus(bool bval);
    signed short getSAXmTemperature();
    void setSAXmTemperature(signed short ushort);
    bool getSAXmTmpStatus();
    void setSAXmTmpStatus(bool bval);

    unsigned short getBattVoltage();
    void setBattVoltage(unsigned short ushort);
    signed short getBattTemperature();
    void setBattTemperature(signed short ushort);
    unsigned short getBattCapacity();
    void setBattCapacity(unsigned short ushort);
    bool getBattStatus();
    void setBattStatus(bool bval);
};

#endif /* COMMSTELEMETRYCONTAINER_H_ */
