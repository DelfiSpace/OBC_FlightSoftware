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
    return ((telemetry[7] & 0x20) != 0);
}

void OBCTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

signed short OBCTelemetryContainer::getBusCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void OBCTelemetryContainer::setBusCurrent(signed short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[10];
    ((unsigned char *)&ushort)[0] = telemetry[11];
    return ushort;
}

void OBCTelemetryContainer::setBusVoltage(unsigned short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
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
