/*
 * ADBTelemetryContainer.cpp
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#include <ADBTelemetryContainer.h>

int ADBTelemetryContainer::size()
{
    return ADB_CONTAINER_SIZE;
}

unsigned char* ADBTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long ADBTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void ADBTelemetryContainer::setUpTime(unsigned long ulong)
{
    *((unsigned long *)&(telemetry[0])) = ulong;
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool ADBTelemetryContainer::getBusStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void ADBTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool ADBTelemetryContainer::getTorquerXStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void ADBTelemetryContainer::setTorquerXStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool ADBTelemetryContainer::getTorquerYStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void ADBTelemetryContainer::setTorquerYStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool ADBTelemetryContainer::getTorquerZStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void ADBTelemetryContainer::setTorquerZStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool ADBTelemetryContainer::getTmpStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void ADBTelemetryContainer::setTmpStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

signed short ADBTelemetryContainer::getBusCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void ADBTelemetryContainer::setBusCurrent(signed short ushort)
{
    telemetry[16] = ((unsigned char *)&ushort)[1];
    telemetry[17] = ((unsigned char *)&ushort)[0];
}

unsigned short ADBTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[24];
    ((unsigned char *)&ushort)[0] = telemetry[25];
    return ushort;
}

void ADBTelemetryContainer::setBusVoltage(unsigned short ushort)
{
    telemetry[24] = ((unsigned char *)&ushort)[1];
    telemetry[25] = ((unsigned char *)&ushort)[0];
}

signed short ADBTelemetryContainer::getTorquerXCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}

void ADBTelemetryContainer::setTorquerXCurrent(signed short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}

unsigned short ADBTelemetryContainer::getTorquerXVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[22];
    ((unsigned char *)&ushort)[0] = telemetry[23];
    return ushort;
}

void ADBTelemetryContainer::setTorquerXVoltage(unsigned short ushort)
{
    telemetry[22] = ((unsigned char *)&ushort)[1];
    telemetry[23] = ((unsigned char *)&ushort)[0];
}

signed short ADBTelemetryContainer::getTorquerYCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void ADBTelemetryContainer::setTorquerYCurrent(signed short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}

unsigned short ADBTelemetryContainer::getTorquerYVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[20];
    ((unsigned char *)&ushort)[0] = telemetry[21];
    return ushort;
}

void ADBTelemetryContainer::setTorquerYVoltage(unsigned short ushort)
{
    telemetry[20] = ((unsigned char *)&ushort)[1];
    telemetry[21] = ((unsigned char *)&ushort)[0];
}

signed short ADBTelemetryContainer::getTorquerZCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void ADBTelemetryContainer::setTorquerZCurrent(signed short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
}

unsigned short ADBTelemetryContainer::getTorquerZVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[18];
    ((unsigned char *)&ushort)[0] = telemetry[19];
    return ushort;
}

void ADBTelemetryContainer::setTorquerZVoltage(unsigned short ushort)
{
    telemetry[18] = ((unsigned char *)&ushort)[1];
    telemetry[19] = ((unsigned char *)&ushort)[0];
}

signed short ADBTelemetryContainer::getTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[8];
    ((unsigned char *)&ushort)[0] = telemetry[9];
    return ushort;
}

void ADBTelemetryContainer::setTemperature(signed short ushort)
{
    telemetry[8] = ((unsigned char *)&ushort)[1];
    telemetry[9] = ((unsigned char *)&ushort)[0];
}
