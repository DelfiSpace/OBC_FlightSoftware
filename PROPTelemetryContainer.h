/*
 * PROPTelemetryContainer.h
 *
 *  Created on: 14 Nov 2019
 *      Author: guillemrueda
 */

#ifndef PROPTELEMETRYCONTAINER_H_
#define PROPTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define PROP_CONTAINER_SIZE  26

class PROPTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[PROP_CONTAINER_SIZE];

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

    signed short getValveHoldCurrent();
    void setValveHoldCurrent(signed short ushort);
    unsigned short getValveHoldVoltage();
    void setValveHoldVoltage(unsigned short ushort);
    bool getValveHoldStatus();
    void setValveHoldStatus(bool bval);

    signed short getValveSpikeCurrent();
    void setValveSpikeCurrent(signed short ushort);
    unsigned short getValveSpikeVoltage();
    void setValveSpikeVoltage(unsigned short ushort);
    bool getValveSpikeStatus();
    void setValveSpikeStatus(bool bval);

    signed short getHeatersCurrent();
    void setHeatersCurrent(signed short ushort);
    unsigned short getHeatersVoltage();
    void setHeatersVoltage(unsigned short ushort);
    bool getHeatersStatus();
    void setHeatersStatus(bool bval);
};

#endif /* PROPTELEMETRYCONTAINER_H_ */
