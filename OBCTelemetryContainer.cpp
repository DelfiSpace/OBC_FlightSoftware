/*
 * OBCTelemetryContainer.cpp
 *
 *  Created on: Jul 6, 2020
 *      Author: tom-h
 */

#include <OBCTelemetryContainer.h>

int OBCTelemetryContainer::size()
{
    return OBC_CONTAINER_SIZE;
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

unsigned short OBCTelemetryContainer::getBootCount()
{
    unsigned long ushort;
    ((unsigned char *)&ushort)[1] = telemetry[4];
    ((unsigned char *)&ushort)[0] = telemetry[5];
    return ushort;
}

void OBCTelemetryContainer::setBootCount(unsigned short ushort)
{
    telemetry[4] = ((unsigned char *)&ushort)[1];
    telemetry[5] = ((unsigned char *)&ushort)[0];
}

bool OBCTelemetryContainer::getINAStatus()
{
    return ((telemetry[6] & 0x80) != 0);
}

void OBCTelemetryContainer::setINAStatus(bool bval)
{
    telemetry[6] &= (~0x80);
    telemetry[6] |= bval ? 0x80 : 0x00;
}

bool OBCTelemetryContainer::getTMPStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void OBCTelemetryContainer::setTMPStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

unsigned short OBCTelemetryContainer::getCurrent()
{
    unsigned long ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void OBCTelemetryContainer::setCurrent(unsigned short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}

unsigned short OBCTelemetryContainer::getVoltage()
{
    unsigned long ushort;
    ((unsigned char *)&ushort)[1] = telemetry[10];
    ((unsigned char *)&ushort)[0] = telemetry[11];
    return ushort;
}

void OBCTelemetryContainer::setVoltage(unsigned short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];

}

unsigned short OBCTelemetryContainer::getTemp()
{
    unsigned long ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void OBCTelemetryContainer::setTemp(unsigned short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}
