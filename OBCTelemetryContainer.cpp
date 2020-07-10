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

unsigned short OBCTelemetryContainer::getVoltage()
{
    unsigned long ushort;
    ((unsigned char *)&ushort)[1] = telemetry[0];
    ((unsigned char *)&ushort)[0] = telemetry[1];
    return ushort;
}

void OBCTelemetryContainer::setVoltage(unsigned short ushort)
{
    telemetry[0] = ((unsigned char *)&ushort)[1];
    telemetry[1] = ((unsigned char *)&ushort)[0];

}
unsigned long OBCTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[3];
    ((unsigned char *)&ulong)[2] = telemetry[4];
    ((unsigned char *)&ulong)[1] = telemetry[5];
    ((unsigned char *)&ulong)[0] = telemetry[6];
    return ulong;
}

void OBCTelemetryContainer::setUpTime(unsigned long ulong)
{
    telemetry[3] = ((unsigned char *)&ulong)[3];
    telemetry[4] = ((unsigned char *)&ulong)[2];
    telemetry[5] = ((unsigned char *)&ulong)[1];
    telemetry[6] = ((unsigned char *)&ulong)[0];
}

unsigned long OBCTelemetryContainer::getTemp()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[7];
    ((unsigned char *)&ulong)[2] = telemetry[8];
    ((unsigned char *)&ulong)[1] = telemetry[9];
    ((unsigned char *)&ulong)[0] = telemetry[10];
    return ulong;
}

void OBCTelemetryContainer::setTemp(unsigned long ulong)
{
    telemetry[7] = ((unsigned char *)&ulong)[3];
    telemetry[8] = ((unsigned char *)&ulong)[2];
    telemetry[9] = ((unsigned char *)&ulong)[1];
    telemetry[10] = ((unsigned char *)&ulong)[0];
}
