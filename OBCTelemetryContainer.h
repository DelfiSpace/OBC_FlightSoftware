/*
 * OBCTelemetryContainer.h
 *
 *  Created on: Jul 6, 2020
 *      Author: tom-h
 */

#ifndef OBCTELEMETRYCONTAINER_H_
#define OBCTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define OBC_CONTAINER_SIZE  12
class OBCTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[OBC_CONTAINER_SIZE];

public:
    virtual int size();
    virtual unsigned char * getArray();

    bool getINAStatus();
    void setINAStatus(bool bval);

    bool getTMPStatus();
    void setTMPStatus(bool bval);

    unsigned short getBootCount();
    void setBootCount(unsigned short ushort);

    unsigned short getCurrent();
    void setCurrent(unsigned short ushort);

    unsigned short getVoltage();
    void setVoltage(unsigned short ushort);

    unsigned short getTemp();
    void setTemp(unsigned short ushort);

    unsigned long getUpTime();
    void setUpTime(unsigned long ulong);




};
#endif /* OBCTELEMETRYCONTAINER_H_ */
