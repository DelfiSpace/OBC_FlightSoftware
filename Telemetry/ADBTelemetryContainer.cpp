//File Generated By TelemetryFromXTCE.py

#include "ADBTelemetryContainer.h"

int ADBTelemetryContainer::size()
{
    return ADB_CONTAINER_SIZE;
}

unsigned char* ADBTelemetryContainer::getArray()
{
    return &telemetry[0];
}

char ADBTelemetryContainer::getStatus()
{
    return telemetry[0];
}
void ADBTelemetryContainer::setStatus(char uchar)
{
    telemetry[0] = uchar;
}
char ADBTelemetryContainer::getBootCounter()
{
    return telemetry[1];
}
void ADBTelemetryContainer::setBootCounter(char uchar)
{
    telemetry[1] = uchar;
}
long ADBTelemetryContainer::getResetCause()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[2] = telemetry[2];
    ((unsigned char *)&ulong)[1] = telemetry[3];
    ((unsigned char *)&ulong)[0] = telemetry[4];
    return ulong;
}
void ADBTelemetryContainer::setResetCause(long ulong)
{
    telemetry[2] = ((unsigned char *)&ulong)[2];
    telemetry[3] = ((unsigned char *)&ulong)[1];
    telemetry[4] = ((unsigned char *)&ulong)[0];
}
unsigned long ADBTelemetryContainer::getUptime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[5];
    ((unsigned char *)&ulong)[2] = telemetry[6];
    ((unsigned char *)&ulong)[1] = telemetry[7];
    ((unsigned char *)&ulong)[0] = telemetry[8];
    return ulong;
}
void ADBTelemetryContainer::setUptime(unsigned long ulong)
{
    telemetry[5] = ((unsigned char *)&ulong)[3];
    telemetry[6] = ((unsigned char *)&ulong)[2];
    telemetry[7] = ((unsigned char *)&ulong)[1];
    telemetry[8] = ((unsigned char *)&ulong)[0];
}
unsigned long ADBTelemetryContainer::getTotalUptime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[9];
    ((unsigned char *)&ulong)[2] = telemetry[10];
    ((unsigned char *)&ulong)[1] = telemetry[11];
    ((unsigned char *)&ulong)[0] = telemetry[12];
    return ulong;
}
void ADBTelemetryContainer::setTotalUptime(unsigned long ulong)
{
    telemetry[9] = ((unsigned char *)&ulong)[3];
    telemetry[10] = ((unsigned char *)&ulong)[2];
    telemetry[11] = ((unsigned char *)&ulong)[1];
    telemetry[12] = ((unsigned char *)&ulong)[0];
}
char ADBTelemetryContainer::getVersionNumber()
{
    return telemetry[13];
}
void ADBTelemetryContainer::setVersionNumber(char uchar)
{
    telemetry[13] = uchar;
}
short ADBTelemetryContainer::getMCUTemp()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}
void ADBTelemetryContainer::setMCUTemp(short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}
bool ADBTelemetryContainer::getINAStatus()
{
    return ((telemetry[16] & 0x02) != 0);
}
void ADBTelemetryContainer::setINAStatus(bool bval)
{
    telemetry[16] &= (~0x02);
    telemetry[16] |= bval ? 0x02 : 0x00;
}
bool ADBTelemetryContainer::getTMPStatus()
{
    return ((telemetry[16] & 0x01) != 0);
}
void ADBTelemetryContainer::setTMPStatus(bool bval)
{
    telemetry[16] &= (~0x01);
    telemetry[16] |= bval ? 0x01 : 0x00;
}
short ADBTelemetryContainer::getCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[17];
    ((unsigned char *)&ushort)[0] = telemetry[18];
    return ushort;
}
void ADBTelemetryContainer::setCurrent(short ushort)
{
    telemetry[17] = ((unsigned char *)&ushort)[1];
    telemetry[18] = ((unsigned char *)&ushort)[0];
}
short ADBTelemetryContainer::getVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[19];
    ((unsigned char *)&ushort)[0] = telemetry[20];
    return ushort;
}
void ADBTelemetryContainer::setVoltage(short ushort)
{
    telemetry[19] = ((unsigned char *)&ushort)[1];
    telemetry[20] = ((unsigned char *)&ushort)[0];
}
short ADBTelemetryContainer::getTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[21];
    ((unsigned char *)&ushort)[0] = telemetry[22];
    return ushort;
}
void ADBTelemetryContainer::setTemperature(short ushort)
{
    telemetry[21] = ((unsigned char *)&ushort)[1];
    telemetry[22] = ((unsigned char *)&ushort)[0];
}
