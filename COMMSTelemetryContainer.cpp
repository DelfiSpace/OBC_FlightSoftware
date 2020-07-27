/*
 * COMMSTelemetryContainer.cpp
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#include <COMMSTelemetryContainer.h>

int COMMSTelemetryContainer::size()
{
    return COMMS_CONTAINER_SIZE;
}

unsigned char* COMMSTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long COMMSTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void COMMSTelemetryContainer::setUpTime(unsigned long ulong)
{
    *((unsigned long *)&(telemetry[0])) = ulong;
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool COMMSTelemetryContainer::getB1Status()
{
    return ((telemetry[8] & 0x10) != 0);
}

void COMMSTelemetryContainer::setB1Status(bool bval)
{
    telemetry[8] &= (~0x10);
    telemetry[8] |= bval ? 0x10 : 0x00;
}

bool COMMSTelemetryContainer::getB2Status()
{
    return ((telemetry[8] & 0x20) != 0);
}

void COMMSTelemetryContainer::setB2Status(bool bval)
{
    telemetry[8] &= (~0x20);
    telemetry[8] |= bval ? 0x20 : 0x00;
}

bool COMMSTelemetryContainer::getB3Status()
{
    return ((telemetry[8] & 0x40) != 0);
}

void COMMSTelemetryContainer::setB3Status(bool bval)
{
    telemetry[8] &= (~0x40);
    telemetry[8] |= bval ? 0x40 : 0x00;
}

bool COMMSTelemetryContainer::getB4Status()
{
    return ((telemetry[8] & 0x80) != 0);
}

void COMMSTelemetryContainer::setB4Status(bool bval)
{
    telemetry[8] &= (~0x80);
    telemetry[8] |= bval ? 0x80 : 0x00;
}

bool COMMSTelemetryContainer::getIntBStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void COMMSTelemetryContainer::setIntBStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

bool COMMSTelemetryContainer::getURBStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void COMMSTelemetryContainer::setURBStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool COMMSTelemetryContainer::getSAYpStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void COMMSTelemetryContainer::setSAYpStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool COMMSTelemetryContainer::getSAYmStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void COMMSTelemetryContainer::setSAYmStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool COMMSTelemetryContainer::getSAXpStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void COMMSTelemetryContainer::setSAXpStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool COMMSTelemetryContainer::getSAXmStatus()
{
    return ((telemetry[7] & 0x20) != 0);
}

void COMMSTelemetryContainer::setSAXmStatus(bool bval)
{
    telemetry[7] &= (~0x20);
    telemetry[7] |= bval ? 0x20 : 0x00;
}

bool COMMSTelemetryContainer::getBattStatus()
{
    return ((telemetry[7] & 0x40) != 0);
}

void COMMSTelemetryContainer::setBattStatus(bool bval)
{
    telemetry[7] &= (~0x40);
    telemetry[7] |= bval ? 0x40 : 0x00;
}

bool COMMSTelemetryContainer::getSAYpTmpStatus()
{
    return ((telemetry[8] & 0x01) != 0);
}

void COMMSTelemetryContainer::setSAYpTmpStatus(bool bval)
{
    telemetry[8] &= (~0x01);
    telemetry[8] |= bval ? 0x01 : 0x00;
}

bool COMMSTelemetryContainer::getSAYmTmpStatus()
{
    return ((telemetry[8] & 0x02) != 0);
}

void COMMSTelemetryContainer::setSAYmTmpStatus(bool bval)
{
    telemetry[8] &= (~0x02);
    telemetry[8] |= bval ? 0x02 : 0x00;
}

bool COMMSTelemetryContainer::getSAXpTmpStatus()
{
    return ((telemetry[8] & 0x04) != 0);
}

void COMMSTelemetryContainer::setSAXpTmpStatus(bool bval)
{
    telemetry[8] &= (~0x04);
    telemetry[8] |= bval ? 0x04 : 0x00;
}

bool COMMSTelemetryContainer::getSAXmTmpStatus()
{
    return ((telemetry[8] & 0x08) != 0);
}

void COMMSTelemetryContainer::setSAXmTmpStatus(bool bval)
{
    telemetry[8] &= (~0x08);
    telemetry[8] |= bval ? 0x08 : 0x00;
}

signed short COMMSTelemetryContainer::getIntBCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[58];
    ((unsigned char *)&ushort)[0] = telemetry[59];
    return ushort;
}

void COMMSTelemetryContainer::setIntBCurrent(signed short ushort)
{
    telemetry[58] = ((unsigned char *)&ushort)[1];
    telemetry[59] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getIntBVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[60];
    ((unsigned char *)&ushort)[0] = telemetry[61];
    return ushort;
}

void COMMSTelemetryContainer::setIntBVoltage(unsigned short ushort)
{
    telemetry[60] = ((unsigned char *)&ushort)[1];
    telemetry[61] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getURBCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[62];
    ((unsigned char *)&ushort)[0] = telemetry[63];
    return ushort;
}

void COMMSTelemetryContainer::setURBCurrent(signed short ushort)
{
    telemetry[62] = ((unsigned char *)&ushort)[1];
    telemetry[63] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getURBVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[64];
    ((unsigned char *)&ushort)[0] = telemetry[65];
    return ushort;
}

void COMMSTelemetryContainer::setURBVoltage(unsigned short ushort)
{
    telemetry[64] = ((unsigned char *)&ushort)[1];
    telemetry[65] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getB1Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[33];
    ((unsigned char *)&ushort)[0] = telemetry[34];
    return ushort;
}

void COMMSTelemetryContainer::setB1Current(signed short ushort)
{
    telemetry[33] = ((unsigned char *)&ushort)[1];
    telemetry[34] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getB1Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[41];
    ((unsigned char *)&ushort)[0] = telemetry[42];
    return ushort;
}

void COMMSTelemetryContainer::setB1Voltage(unsigned short ushort)
{
    telemetry[41] = ((unsigned char *)&ushort)[1];
    telemetry[42] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getB2Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[35];
    ((unsigned char *)&ushort)[0] = telemetry[36];
    return ushort;
}

void COMMSTelemetryContainer::setB2Current(signed short ushort)
{
    telemetry[35] = ((unsigned char *)&ushort)[1];
    telemetry[36] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getB2Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[43];
    ((unsigned char *)&ushort)[0] = telemetry[44];
    return ushort;
}

void COMMSTelemetryContainer::setB2Voltage(unsigned short ushort)
{
    telemetry[43] = ((unsigned char *)&ushort)[1];
    telemetry[44] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getB3Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[37];
    ((unsigned char *)&ushort)[0] = telemetry[38];
    return ushort;
}

void COMMSTelemetryContainer::setB3Current(signed short ushort)
{
    telemetry[37] = ((unsigned char *)&ushort)[1];
    telemetry[38] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getB3Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[45];
    ((unsigned char *)&ushort)[0] = telemetry[46];
    return ushort;
}

void COMMSTelemetryContainer::setB3Voltage(unsigned short ushort)
{
    telemetry[45] = ((unsigned char *)&ushort)[1];
    telemetry[46] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getB4Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[39];
    ((unsigned char *)&ushort)[0] = telemetry[40];
    return ushort;
}

void COMMSTelemetryContainer::setB4Current(signed short ushort)
{
    telemetry[39] = ((unsigned char *)&ushort)[1];
    telemetry[40] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getB4Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[47];
    ((unsigned char *)&ushort)[0] = telemetry[48];
    return ushort;
}

void COMMSTelemetryContainer::setB4Voltage(unsigned short ushort)
{
    telemetry[47] = ((unsigned char *)&ushort)[1];
    telemetry[48] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAYpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[17];
    ((unsigned char *)&ushort)[0] = telemetry[18];
    return ushort;
}

void COMMSTelemetryContainer::setSAYpCurrent(signed short ushort)
{
    telemetry[17] = ((unsigned char *)&ushort)[1];
    telemetry[18] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getSAYpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[25];
    ((unsigned char *)&ushort)[0] = telemetry[26];
    return ushort;
}

void COMMSTelemetryContainer::setSAYpVoltage(unsigned short ushort)
{
    telemetry[25] = ((unsigned char *)&ushort)[1];
    telemetry[26] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAYmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[19];
    ((unsigned char *)&ushort)[0] = telemetry[20];
    return ushort;
}

void COMMSTelemetryContainer::setSAYmCurrent(signed short ushort)
{
    telemetry[19] = ((unsigned char *)&ushort)[1];
    telemetry[20] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getSAYmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[27];
    ((unsigned char *)&ushort)[0] = telemetry[28];
    return ushort;
}

void COMMSTelemetryContainer::setSAYmVoltage(unsigned short ushort)
{
    telemetry[27] = ((unsigned char *)&ushort)[1];
    telemetry[28] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAXpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[21];
    ((unsigned char *)&ushort)[0] = telemetry[22];
    return ushort;
}

void COMMSTelemetryContainer::setSAXpCurrent(signed short ushort)
{
    telemetry[21] = ((unsigned char *)&ushort)[1];
    telemetry[22] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getSAXpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[29];
    ((unsigned char *)&ushort)[0] = telemetry[30];
    return ushort;
}

void COMMSTelemetryContainer::setSAXpVoltage(unsigned short ushort)
{
    telemetry[29] = ((unsigned char *)&ushort)[1];
    telemetry[30] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAXmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[23];
    ((unsigned char *)&ushort)[0] = telemetry[24];
    return ushort;
}

void COMMSTelemetryContainer::setSAXmCurrent(signed short ushort)
{
    telemetry[23] = ((unsigned char *)&ushort)[1];
    telemetry[24] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getSAXmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[31];
    ((unsigned char *)&ushort)[0] = telemetry[32];
    return ushort;
}

void COMMSTelemetryContainer::setSAXmVoltage(unsigned short ushort)
{
    telemetry[31] = ((unsigned char *)&ushort)[1];
    telemetry[32] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getBattVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[49];
    ((unsigned char *)&ushort)[0] = telemetry[50];
    return ushort;
}

void COMMSTelemetryContainer::setBattVoltage(unsigned short ushort)
{
    telemetry[49] = ((unsigned char *)&ushort)[1];
    telemetry[50] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getBattTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[53];
    ((unsigned char *)&ushort)[0] = telemetry[54];
    return ushort;
}

void COMMSTelemetryContainer::setBattTemperature(signed short ushort)
{
    telemetry[53] = ((unsigned char *)&ushort)[1];
    telemetry[54] = ((unsigned char *)&ushort)[0];
}

unsigned short COMMSTelemetryContainer::getBattCapacity()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[51];
    ((unsigned char *)&ushort)[0] = telemetry[52];
    return ushort;
}

void COMMSTelemetryContainer::setBattCapacity(unsigned short ushort)
{
    telemetry[51] = ((unsigned char *)&ushort)[1];
    telemetry[52] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAYpTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[9];
    ((unsigned char *)&ushort)[0] = telemetry[10];
    return ushort;
}

void COMMSTelemetryContainer::setSAYpTemperature(signed short ushort)
{
    telemetry[9] = ((unsigned char *)&ushort)[1];
    telemetry[10] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAYmTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[11];
    ((unsigned char *)&ushort)[0] = telemetry[12];
    return ushort;
}

void COMMSTelemetryContainer::setSAYmTemperature(signed short ushort)
{
    telemetry[11] = ((unsigned char *)&ushort)[1];
    telemetry[12] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAXpTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[13];
    ((unsigned char *)&ushort)[0] = telemetry[14];
    return ushort;
}

void COMMSTelemetryContainer::setSAXpTemperature(signed short ushort)
{
    telemetry[13] = ((unsigned char *)&ushort)[1];
    telemetry[14] = ((unsigned char *)&ushort)[0];
}

signed short COMMSTelemetryContainer::getSAXmTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[15];
    ((unsigned char *)&ushort)[0] = telemetry[16];
    return ushort;
}

void COMMSTelemetryContainer::setSAXmTemperature(signed short ushort)
{
    telemetry[15] = ((unsigned char *)&ushort)[1];
    telemetry[16] = ((unsigned char *)&ushort)[0];
}
