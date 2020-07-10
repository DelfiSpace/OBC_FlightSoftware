/*
 * OBCTelemetryContainer.h
 *
 *  Created on: Jul 6, 2020
 *      Author: tom-h
 */

#ifndef OBCTELEMETRYCONTAINER_H_
#define OBCTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define OBC_CONTAINER_SIZE  11
class OBCTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[OBC_CONTAINER_SIZE];

public:
    virtual int size();
    virtual unsigned char * getArray();

    unsigned short getVoltage();
    void setVoltage(unsigned short ushort);

    unsigned long getUpTime();
    void setUpTime(unsigned long ulong);

    unsigned long getTemp();
    void setTemp(unsigned long ulong);


};
#endif /* OBCTELEMETRYCONTAINER_H_ */
