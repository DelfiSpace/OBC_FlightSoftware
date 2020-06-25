/*
 * PROPTelemetryContainer.cpp
 *
 *  Created on: 14 Nov 2019
 *      Author: guillemrueda
 */

#include "PROPTelemetryContainer.h"

int PROPTelemetryContainer::size()
{
    return PROP_CONTAINER_SIZE;
}

unsigned char* PROPTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long PROPTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void PROPTelemetryContainer::setUpTime(unsigned long ulong)
{
    *((unsigned long *)&(telemetry[0])) = ulong;
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool PROPTelemetryContainer::getBusStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void PROPTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool PROPTelemetryContainer::getValveHoldStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void PROPTelemetryContainer::setValveHoldStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool PROPTelemetryContainer::getValveSpikeStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void PROPTelemetryContainer::setValveSpikeStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool PROPTelemetryContainer::getHeatersStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void PROPTelemetryContainer::setHeatersStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool PROPTelemetryContainer::getTmpStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void PROPTelemetryContainer::setTmpStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

signed short PROPTelemetryContainer::getBusCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void PROPTelemetryContainer::setBusCurrent(signed short ushort)
{
    telemetry[16] = ((unsigned char *)&ushort)[1];
    telemetry[17] = ((unsigned char *)&ushort)[0];
}

unsigned short PROPTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[24];
    ((unsigned char *)&ushort)[0] = telemetry[25];
    return ushort;
}

void PROPTelemetryContainer::setBusVoltage(unsigned short ushort)
{
    telemetry[24] = ((unsigned char *)&ushort)[1];
    telemetry[25] = ((unsigned char *)&ushort)[0];
}

signed short PROPTelemetryContainer::getValveHoldCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}

void PROPTelemetryContainer::setValveHoldCurrent(signed short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}

unsigned short PROPTelemetryContainer::getValveHoldVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[22];
    ((unsigned char *)&ushort)[0] = telemetry[23];
    return ushort;
}

void PROPTelemetryContainer::setValveHoldVoltage(unsigned short ushort)
{
    telemetry[22] = ((unsigned char *)&ushort)[1];
    telemetry[23] = ((unsigned char *)&ushort)[0];
}

signed short PROPTelemetryContainer::getValveSpikeCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void PROPTelemetryContainer::setValveSpikeCurrent(signed short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}

unsigned short PROPTelemetryContainer::getValveSpikeVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[20];
    ((unsigned char *)&ushort)[0] = telemetry[21];
    return ushort;
}

void PROPTelemetryContainer::setValveSpikeVoltage(unsigned short ushort)
{
    telemetry[20] = ((unsigned char *)&ushort)[1];
    telemetry[21] = ((unsigned char *)&ushort)[0];
}

signed short PROPTelemetryContainer::getHeatersCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void PROPTelemetryContainer::setHeatersCurrent(signed short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
}

unsigned short PROPTelemetryContainer::getHeatersVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[18];
    ((unsigned char *)&ushort)[0] = telemetry[19];
    return ushort;
}

void PROPTelemetryContainer::setHeatersVoltage(unsigned short ushort)
{
    telemetry[18] = ((unsigned char *)&ushort)[1];
    telemetry[19] = ((unsigned char *)&ushort)[0];
}

signed short PROPTelemetryContainer::getTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void PROPTelemetryContainer::setTemperature(signed short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}
