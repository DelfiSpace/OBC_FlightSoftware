/*
 * EPSTelemetryContainer.h
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#ifndef EPSTELEMETRYCONTAINER_H_
#define EPSTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define EPS_CONTAINER_SIZE  87
class EPSTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[EPS_CONTAINER_SIZE];

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

    signed short getSPXmCurrent();
    void setSPXmCurrent(signed short ushort);
    unsigned short getSPXmVoltage();
    void setSPXmVoltage(unsigned short ushort);
    bool getSPXmStatus();
    void setSPXmStatus(bool bval);

    signed short getSPXpCurrent();
    void setSPXpCurrent(signed short ushort);
    unsigned short getSPXpVoltage();
    void setSPXpVoltage(unsigned short ushort);
    bool getSPXpStatus();
    void setSPXpStatus(bool bval);

    signed short getSPYmCurrent();
    void setSPYmCurrent(signed short ushort);
    unsigned short getSPYmVoltage();
    void setSPYmVoltage(unsigned short ushort);
    bool getSPYmStatus();
    void setSPYmStatus(bool bval);

    signed short getSPYpCurrent();
    void setSPYpCurrent(signed short ushort);
    unsigned short getSPYpVoltage();
    void setSPYpVoltage(unsigned short ushort);
    bool getSPYpStatus();
    void setSPYpStatus(bool bval);

    unsigned short getBattVoltage();
    void setBattVoltage(unsigned short ushort);
    unsigned short getBattVoltage1();
    void setBattVoltage1(unsigned short ushort);
    signed short getBattCurrent();
    void setBattCurrent(signed short ushort);
    signed short getBattTemperature();
    void setBattTemperature(signed short ushort);
    unsigned short getBattCapacity();
    void setBattCapacity(unsigned short ushort);
    bool getBattStatus();
    void setBattStatus(bool bval);
    bool getBattINAStatus();
    void setBattINAStatus(bool bval);

    unsigned char getBusStatus();
    void setBusStatus(unsigned char uchar);
    unsigned char getBusErrorStatus();
    void setBusErrorStatus(unsigned char uchar);

    signed short getMCUTemperature();
    void setMCUTemperature(signed short ushort);
};

#endif /* EPSTELEMETRYCONTAINER_H_ */
