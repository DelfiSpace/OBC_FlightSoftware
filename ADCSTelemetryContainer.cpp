/*
 * ADCSTelemetryContainer.cpp
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#include "ADCSTelemetryContainer.h"

int ADCSTelemetryContainer::size()
{
    return ADCS_CONTAINER_SIZE;
}

unsigned char* ADCSTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long ADCSTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void ADCSTelemetryContainer::setUpTime(unsigned long ulong)
{
    *((unsigned long *)&(telemetry[0])) = ulong;
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool ADCSTelemetryContainer::getBusStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void ADCSTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool ADCSTelemetryContainer::getTorquerXStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void ADCSTelemetryContainer::setTorquerXStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool ADCSTelemetryContainer::getTorquerYStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void ADCSTelemetryContainer::setTorquerYStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool ADCSTelemetryContainer::getTorquerZStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void ADCSTelemetryContainer::setTorquerZStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool ADCSTelemetryContainer::getTmpStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void ADCSTelemetryContainer::setTmpStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

signed short ADCSTelemetryContainer::getBusCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void ADCSTelemetryContainer::setBusCurrent(signed short ushort)
{
    telemetry[16] = ((unsigned char *)&ushort)[1];
    telemetry[17] = ((unsigned char *)&ushort)[0];
}

unsigned short ADCSTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[24];
    ((unsigned char *)&ushort)[0] = telemetry[25];
    return ushort;
}

void ADCSTelemetryContainer::setBusVoltage(unsigned short ushort)
{
    telemetry[24] = ((unsigned char *)&ushort)[1];
    telemetry[25] = ((unsigned char *)&ushort)[0];
}

signed short ADCSTelemetryContainer::getTorquerXCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerXCurrent(signed short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}

unsigned short ADCSTelemetryContainer::getTorquerXVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[22];
    ((unsigned char *)&ushort)[0] = telemetry[23];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerXVoltage(unsigned short ushort)
{
    telemetry[22] = ((unsigned char *)&ushort)[1];
    telemetry[23] = ((unsigned char *)&ushort)[0];
}

signed short ADCSTelemetryContainer::getTorquerYCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerYCurrent(signed short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}

unsigned short ADCSTelemetryContainer::getTorquerYVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[20];
    ((unsigned char *)&ushort)[0] = telemetry[21];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerYVoltage(unsigned short ushort)
{
    telemetry[20] = ((unsigned char *)&ushort)[1];
    telemetry[21] = ((unsigned char *)&ushort)[0];
}

signed short ADCSTelemetryContainer::getTorquerZCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerZCurrent(signed short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
}

unsigned short ADCSTelemetryContainer::getTorquerZVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[18];
    ((unsigned char *)&ushort)[0] = telemetry[19];
    return ushort;
}

void ADCSTelemetryContainer::setTorquerZVoltage(unsigned short ushort)
{
    telemetry[18] = ((unsigned char *)&ushort)[1];
    telemetry[19] = ((unsigned char *)&ushort)[0];
}

signed short ADCSTelemetryContainer::getTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void ADCSTelemetryContainer::setTemperature(signed short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}
