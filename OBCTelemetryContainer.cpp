/*
 * OBCTelemetryContainer.cpp
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#include <OBCTelemetryContainer.h>

int OBCTelemetryContainer::size()
{
    return ADCS_CONTAINER_SIZE;
}

unsigned char* OBCTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long OBCTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void OBCTelemetryContainer::setUpTime(unsigned long ulong)
{
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool OBCTelemetryContainer::getBusStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void OBCTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool OBCTelemetryContainer::getTorquerXStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void OBCTelemetryContainer::setTorquerXStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool OBCTelemetryContainer::getTorquerYStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void OBCTelemetryContainer::setTorquerYStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool OBCTelemetryContainer::getTorquerZStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void OBCTelemetryContainer::setTorquerZStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool OBCTelemetryContainer::getTmpStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void OBCTelemetryContainer::setTmpStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

signed short OBCTelemetryContainer::getBusCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void OBCTelemetryContainer::setBusCurrent(signed short ushort)
{
    telemetry[16] = ((unsigned char *)&ushort)[1];
    telemetry[17] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[24];
    ((unsigned char *)&ushort)[0] = telemetry[25];
    return ushort;
}

void OBCTelemetryContainer::setBusVoltage(unsigned short ushort)
{
    telemetry[24] = ((unsigned char *)&ushort)[1];
    telemetry[25] = ((unsigned char *)&ushort)[0];
}

signed short OBCTelemetryContainer::getTorquerXCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}

void OBCTelemetryContainer::setTorquerXCurrent(signed short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getTorquerXVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[22];
    ((unsigned char *)&ushort)[0] = telemetry[23];
    return ushort;
}

void OBCTelemetryContainer::setTorquerXVoltage(unsigned short ushort)
{
    telemetry[22] = ((unsigned char *)&ushort)[1];
    telemetry[23] = ((unsigned char *)&ushort)[0];
}

signed short OBCTelemetryContainer::getTorquerYCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void OBCTelemetryContainer::setTorquerYCurrent(signed short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getTorquerYVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[20];
    ((unsigned char *)&ushort)[0] = telemetry[21];
    return ushort;
}

void OBCTelemetryContainer::setTorquerYVoltage(unsigned short ushort)
{
    telemetry[20] = ((unsigned char *)&ushort)[1];
    telemetry[21] = ((unsigned char *)&ushort)[0];
}

signed short OBCTelemetryContainer::getTorquerZCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void OBCTelemetryContainer::setTorquerZCurrent(signed short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getTorquerZVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[18];
    ((unsigned char *)&ushort)[0] = telemetry[19];
    return ushort;
}

void OBCTelemetryContainer::setTorquerZVoltage(unsigned short ushort)
{
    telemetry[18] = ((unsigned char *)&ushort)[1];
    telemetry[19] = ((unsigned char *)&ushort)[0];
}

signed short OBCTelemetryContainer::getTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void OBCTelemetryContainer::setTemperature(signed short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}
