/*
 * EPSTelemetryContainer.cpp
 *
 *  Created on: 5 Aug 2019
 *      Author: stefanosperett
 */

#include <EPSTelemetryContainer.h>

int EPSTelemetryContainer::size()
{
    return EPS_CONTAINER_SIZE;
}

unsigned char* EPSTelemetryContainer::getArray()
{
    return &telemetry[0];
}

unsigned long EPSTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void EPSTelemetryContainer::setUpTime(unsigned long ulong)
{
    *((unsigned long *)&(telemetry[0])) = ulong;
    telemetry[0] = ((unsigned char *)&ulong)[3];
    telemetry[1] = ((unsigned char *)&ulong)[2];
    telemetry[2] = ((unsigned char *)&ulong)[1];
    telemetry[3] = ((unsigned char *)&ulong)[0];
}

bool EPSTelemetryContainer::getSPYpStatus()
{
    return ((telemetry[9] & 0x01) != 0);
}

void EPSTelemetryContainer::setSPYpStatus(bool bval)
{
    telemetry[9] &= (~0x01);
    telemetry[9] |= bval ? 0x01 : 0x00;
}

bool EPSTelemetryContainer::getSPYmStatus()
{
    return ((telemetry[9] & 0x02) != 0);
}

void EPSTelemetryContainer::setSPYmStatus(bool bval)
{
    telemetry[9] &= (~0x02);
    telemetry[9] |= bval ? 0x02 : 0x00;
}

bool EPSTelemetryContainer::getSPXpStatus()
{
    return ((telemetry[9] & 0x04) != 0);
}

void EPSTelemetryContainer::setSPXpStatus(bool bval)
{
    telemetry[9] &= (~0x04);
    telemetry[9] |= bval ? 0x04 : 0x00;
}

bool EPSTelemetryContainer::getSPXmStatus()
{
    return ((telemetry[9] & 0x08) != 0);
}

void EPSTelemetryContainer::setSPXmStatus(bool bval)
{
    telemetry[9] &= (~0x08);
    telemetry[9] |= bval ? 0x08 : 0x00;
}

bool EPSTelemetryContainer::getB1Status()
{
    return ((telemetry[8] & 0x10) != 0);
}

void EPSTelemetryContainer::setB1Status(bool bval)
{
    telemetry[8] &= (~0x10);
    telemetry[8] |= bval ? 0x10 : 0x00;
}

bool EPSTelemetryContainer::getB2Status()
{
    return ((telemetry[8] & 0x20) != 0);
}

void EPSTelemetryContainer::setB2Status(bool bval)
{
    telemetry[8] &= (~0x20);
    telemetry[8] |= bval ? 0x20 : 0x00;
}

bool EPSTelemetryContainer::getB3Status()
{
    return ((telemetry[8] & 0x40) != 0);
}

void EPSTelemetryContainer::setB3Status(bool bval)
{
    telemetry[8] &= (~0x40);
    telemetry[8] |= bval ? 0x40 : 0x00;
}

bool EPSTelemetryContainer::getB4Status()
{
    return ((telemetry[8] & 0x80) != 0);
}

void EPSTelemetryContainer::setB4Status(bool bval)
{
    telemetry[8] &= (~0x80);
    telemetry[8] |= bval ? 0x80 : 0x00;
}

bool EPSTelemetryContainer::getIntBStatus()
{
    return ((telemetry[7] & 0x01) != 0);
}

void EPSTelemetryContainer::setIntBStatus(bool bval)
{
    telemetry[7] &= (~0x01);
    telemetry[7] |= bval ? 0x01 : 0x00;
}

bool EPSTelemetryContainer::getURBStatus()
{
    return ((telemetry[7] & 0x02) != 0);
}

void EPSTelemetryContainer::setURBStatus(bool bval)
{
    telemetry[7] &= (~0x02);
    telemetry[7] |= bval ? 0x02 : 0x00;
}

bool EPSTelemetryContainer::getSAYpStatus()
{
    return ((telemetry[7] & 0x04) != 0);
}

void EPSTelemetryContainer::setSAYpStatus(bool bval)
{
    telemetry[7] &= (~0x04);
    telemetry[7] |= bval ? 0x04 : 0x00;
}

bool EPSTelemetryContainer::getSAYmStatus()
{
    return ((telemetry[7] & 0x08) != 0);
}

void EPSTelemetryContainer::setSAYmStatus(bool bval)
{
    telemetry[7] &= (~0x08);
    telemetry[7] |= bval ? 0x08 : 0x00;
}

bool EPSTelemetryContainer::getSAXpStatus()
{
    return ((telemetry[7] & 0x10) != 0);
}

void EPSTelemetryContainer::setSAXpStatus(bool bval)
{
    telemetry[7] &= (~0x10);
    telemetry[7] |= bval ? 0x10 : 0x00;
}

bool EPSTelemetryContainer::getSAXmStatus()
{
    return ((telemetry[7] & 0x20) != 0);
}

void EPSTelemetryContainer::setSAXmStatus(bool bval)
{
    telemetry[7] &= (~0x20);
    telemetry[7] |= bval ? 0x20 : 0x00;
}

bool EPSTelemetryContainer::getBattStatus()
{
    return ((telemetry[7] & 0x40) != 0);
}

void EPSTelemetryContainer::setBattStatus(bool bval)
{
    telemetry[7] &= (~0x40);
    telemetry[7] |= bval ? 0x40 : 0x00;
}

bool EPSTelemetryContainer::getBattINAStatus()
{
    return ((telemetry[7] & 0x80) != 0);
}

void EPSTelemetryContainer::setBattINAStatus(bool bval)
{
    telemetry[7] &= (~0x80);
    telemetry[7] |= bval ? 0x80 : 0x00;
}

bool EPSTelemetryContainer::getSAYpTmpStatus()
{
    return ((telemetry[8] & 0x01) != 0);
}

void EPSTelemetryContainer::setSAYpTmpStatus(bool bval)
{
    telemetry[8] &= (~0x01);
    telemetry[8] |= bval ? 0x01 : 0x00;
}

bool EPSTelemetryContainer::getSAYmTmpStatus()
{
    return ((telemetry[8] & 0x02) != 0);
}

void EPSTelemetryContainer::setSAYmTmpStatus(bool bval)
{
    telemetry[8] &= (~0x02);
    telemetry[8] |= bval ? 0x02 : 0x00;
}

bool EPSTelemetryContainer::getSAXpTmpStatus()
{
    return ((telemetry[8] & 0x04) != 0);
}

void EPSTelemetryContainer::setSAXpTmpStatus(bool bval)
{
    telemetry[8] &= (~0x04);
    telemetry[8] |= bval ? 0x04 : 0x00;
}

bool EPSTelemetryContainer::getSAXmTmpStatus()
{
    return ((telemetry[8] & 0x08) != 0);
}

void EPSTelemetryContainer::setSAXmTmpStatus(bool bval)
{
    telemetry[8] &= (~0x08);
    telemetry[8] |= bval ? 0x08 : 0x00;
}

signed short EPSTelemetryContainer::getIntBCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[79];
    ((unsigned char *)&ushort)[0] = telemetry[80];
    return ushort;
}

void EPSTelemetryContainer::setIntBCurrent(signed short ushort)
{
    telemetry[79] = ((unsigned char *)&ushort)[1];
    telemetry[80] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getIntBVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[81];
    ((unsigned char *)&ushort)[0] = telemetry[82];
    return ushort;
}

void EPSTelemetryContainer::setIntBVoltage(unsigned short ushort)
{
    telemetry[81] = ((unsigned char *)&ushort)[1];
    telemetry[82] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getURBCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[83];
    ((unsigned char *)&ushort)[0] = telemetry[84];
    return ushort;
}

void EPSTelemetryContainer::setURBCurrent(signed short ushort)
{
    telemetry[83] = ((unsigned char *)&ushort)[1];
    telemetry[84] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getURBVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[85];
    ((unsigned char *)&ushort)[0] = telemetry[86];
    return ushort;
}

void EPSTelemetryContainer::setURBVoltage(unsigned short ushort)
{
    telemetry[85] = ((unsigned char *)&ushort)[1];
    telemetry[86] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getB1Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[50];
    ((unsigned char *)&ushort)[0] = telemetry[51];
    return ushort;
}

void EPSTelemetryContainer::setB1Current(signed short ushort)
{
    telemetry[50] = ((unsigned char *)&ushort)[1];
    telemetry[51] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getB1Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[58];
    ((unsigned char *)&ushort)[0] = telemetry[59];
    return ushort;
}

void EPSTelemetryContainer::setB1Voltage(unsigned short ushort)
{
    telemetry[58] = ((unsigned char *)&ushort)[1];
    telemetry[59] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getB2Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[52];
    ((unsigned char *)&ushort)[0] = telemetry[53];
    return ushort;
}

void EPSTelemetryContainer::setB2Current(signed short ushort)
{
    telemetry[52] = ((unsigned char *)&ushort)[1];
    telemetry[53] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getB2Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[60];
    ((unsigned char *)&ushort)[0] = telemetry[61];
    return ushort;
}

void EPSTelemetryContainer::setB2Voltage(unsigned short ushort)
{
    telemetry[60] = ((unsigned char *)&ushort)[1];
    telemetry[61] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getB3Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[54];
    ((unsigned char *)&ushort)[0] = telemetry[55];
    return ushort;
}

void EPSTelemetryContainer::setB3Current(signed short ushort)
{
    telemetry[54] = ((unsigned char *)&ushort)[1];
    telemetry[55] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getB3Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[62];
    ((unsigned char *)&ushort)[0] = telemetry[63];
    return ushort;
}

void EPSTelemetryContainer::setB3Voltage(unsigned short ushort)
{
    telemetry[62] = ((unsigned char *)&ushort)[1];
    telemetry[63] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getB4Current()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[56];
    ((unsigned char *)&ushort)[0] = telemetry[57];
    return ushort;
}

void EPSTelemetryContainer::setB4Current(signed short ushort)
{
    telemetry[56] = ((unsigned char *)&ushort)[1];
    telemetry[57] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getB4Voltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[64];
    ((unsigned char *)&ushort)[0] = telemetry[65];
    return ushort;
}

void EPSTelemetryContainer::setB4Voltage(unsigned short ushort)
{
    telemetry[64] = ((unsigned char *)&ushort)[1];
    telemetry[65] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAYpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[18];
    ((unsigned char *)&ushort)[0] = telemetry[19];
    return ushort;
}

void EPSTelemetryContainer::setSAYpCurrent(signed short ushort)
{
    telemetry[18] = ((unsigned char *)&ushort)[1];
    telemetry[19] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSAYpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[26];
    ((unsigned char *)&ushort)[0] = telemetry[27];
    return ushort;
}

void EPSTelemetryContainer::setSAYpVoltage(unsigned short ushort)
{
    telemetry[26] = ((unsigned char *)&ushort)[1];
    telemetry[27] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAYmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[20];
    ((unsigned char *)&ushort)[0] = telemetry[21];
    return ushort;
}

void EPSTelemetryContainer::setSAYmCurrent(signed short ushort)
{
    telemetry[20] = ((unsigned char *)&ushort)[1];
    telemetry[21] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSAYmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[28];
    ((unsigned char *)&ushort)[0] = telemetry[29];
    return ushort;
}

void EPSTelemetryContainer::setSAYmVoltage(unsigned short ushort)
{
    telemetry[28] = ((unsigned char *)&ushort)[1];
    telemetry[29] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAXpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[22];
    ((unsigned char *)&ushort)[0] = telemetry[23];
    return ushort;
}

void EPSTelemetryContainer::setSAXpCurrent(signed short ushort)
{
    telemetry[22] = ((unsigned char *)&ushort)[1];
    telemetry[23] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSAXpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[30];
    ((unsigned char *)&ushort)[0] = telemetry[31];
    return ushort;
}

void EPSTelemetryContainer::setSAXpVoltage(unsigned short ushort)
{
    telemetry[30] = ((unsigned char *)&ushort)[1];
    telemetry[31] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAXmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[24];
    ((unsigned char *)&ushort)[0] = telemetry[25];
    return ushort;
}

void EPSTelemetryContainer::setSAXmCurrent(signed short ushort)
{
    telemetry[24] = ((unsigned char *)&ushort)[1];
    telemetry[25] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSAXmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[32];
    ((unsigned char *)&ushort)[0] = telemetry[33];
    return ushort;
}

void EPSTelemetryContainer::setSAXmVoltage(unsigned short ushort)
{
    telemetry[32] = ((unsigned char *)&ushort)[1];
    telemetry[33] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSPYpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[34];
    ((unsigned char *)&ushort)[0] = telemetry[35];
    return ushort;
}

void EPSTelemetryContainer::setSPYpCurrent(signed short ushort)
{
    telemetry[34] = ((unsigned char *)&ushort)[1];
    telemetry[35] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSPYpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[42];
    ((unsigned char *)&ushort)[0] = telemetry[43];
    return ushort;
}

void EPSTelemetryContainer::setSPYpVoltage(unsigned short ushort)
{
    telemetry[42] = ((unsigned char *)&ushort)[1];
    telemetry[43] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSPYmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[36];
    ((unsigned char *)&ushort)[0] = telemetry[37];
    return ushort;
}

void EPSTelemetryContainer::setSPYmCurrent(signed short ushort)
{
    telemetry[36] = ((unsigned char *)&ushort)[1];
    telemetry[37] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSPYmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[44];
    ((unsigned char *)&ushort)[0] = telemetry[45];
    return ushort;
}

void EPSTelemetryContainer::setSPYmVoltage(unsigned short ushort)
{
    telemetry[44] = ((unsigned char *)&ushort)[1];
    telemetry[45] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSPXpCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[38];
    ((unsigned char *)&ushort)[0] = telemetry[39];
    return ushort;
}

void EPSTelemetryContainer::setSPXpCurrent(signed short ushort)
{
    telemetry[38] = ((unsigned char *)&ushort)[1];
    telemetry[39] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSPXpVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[46];
    ((unsigned char *)&ushort)[0] = telemetry[47];
    return ushort;
}

void EPSTelemetryContainer::setSPXpVoltage(unsigned short ushort)
{
    telemetry[46] = ((unsigned char *)&ushort)[1];
    telemetry[47] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSPXmCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[40];
    ((unsigned char *)&ushort)[0] = telemetry[41];
    return ushort;
}

void EPSTelemetryContainer::setSPXmCurrent(signed short ushort)
{
    telemetry[40] = ((unsigned char *)&ushort)[1];
    telemetry[41] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getSPXmVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[48];
    ((unsigned char *)&ushort)[0] = telemetry[49];
    return ushort;
}

void EPSTelemetryContainer::setSPXmVoltage(unsigned short ushort)
{
    telemetry[48] = ((unsigned char *)&ushort)[1];
    telemetry[49] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getBattVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[66];
    ((unsigned char *)&ushort)[0] = telemetry[67];
    return ushort;
}

void EPSTelemetryContainer::setBattVoltage(unsigned short ushort)
{
    telemetry[66] = ((unsigned char *)&ushort)[1];
    telemetry[67] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getBattVoltage1()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[68];
    ((unsigned char *)&ushort)[0] = telemetry[69];
    return ushort;
}

void EPSTelemetryContainer::setBattVoltage1(unsigned short ushort)
{
    telemetry[68] = ((unsigned char *)&ushort)[1];
    telemetry[69] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getBattCurrent()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[70];
    ((unsigned char *)&ushort)[0] = telemetry[71];
    return ushort;
}

void EPSTelemetryContainer::setBattCurrent(signed short ushort)
{
    telemetry[70] = ((unsigned char *)&ushort)[1];
    telemetry[71] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getBattTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[74];
    ((unsigned char *)&ushort)[0] = telemetry[75];
    return ushort;
}

void EPSTelemetryContainer::setBattTemperature(signed short ushort)
{
    telemetry[74] = ((unsigned char *)&ushort)[1];
    telemetry[75] = ((unsigned char *)&ushort)[0];
}

unsigned short EPSTelemetryContainer::getBattCapacity()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[72];
    ((unsigned char *)&ushort)[0] = telemetry[73];
    return ushort;
}

void EPSTelemetryContainer::setBattCapacity(unsigned short ushort)
{
    telemetry[72] = ((unsigned char *)&ushort)[1];
    telemetry[73] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAYpTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[10];
    ((unsigned char *)&ushort)[0] = telemetry[11];
    return ushort;
}

void EPSTelemetryContainer::setSAYpTemperature(signed short ushort)
{
    telemetry[10] = ((unsigned char *)&ushort)[1];
    telemetry[11] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAYmTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[12];
    ((unsigned char *)&ushort)[0] = telemetry[13];
    return ushort;
}

void EPSTelemetryContainer::setSAYmTemperature(signed short ushort)
{
    telemetry[12] = ((unsigned char *)&ushort)[1];
    telemetry[13] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAXpTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[14];
    ((unsigned char *)&ushort)[0] = telemetry[15];
    return ushort;
}

void EPSTelemetryContainer::setSAXpTemperature(signed short ushort)
{
    telemetry[14] = ((unsigned char *)&ushort)[1];
    telemetry[15] = ((unsigned char *)&ushort)[0];
}

signed short EPSTelemetryContainer::getSAXmTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[16];
    ((unsigned char *)&ushort)[0] = telemetry[17];
    return ushort;
}

void EPSTelemetryContainer::setSAXmTemperature(signed short ushort)
{
    telemetry[16] = ((unsigned char *)&ushort)[1];
    telemetry[17] = ((unsigned char *)&ushort)[0];
}

unsigned char EPSTelemetryContainer::getBusStatus()
{
    return (telemetry[76] & 0x0F);
}

void EPSTelemetryContainer::setBusStatus(unsigned char uchar)
{
    telemetry[76] &= (~0x0F);
    telemetry[76] |= uchar & 0x0F;
}

unsigned char EPSTelemetryContainer::getBusErrorStatus()
{
    return (telemetry[76] & 0xF0);
}

void EPSTelemetryContainer::setBusErrorStatus(unsigned char uchar)
{
    telemetry[76] &= (~0xF0);
    telemetry[76] |= (uchar << 4) & 0xF0;
}

signed short EPSTelemetryContainer::getMCUTemperature()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[77];
    ((unsigned char *)&ushort)[0] = telemetry[78];
    return ushort;
}

void EPSTelemetryContainer::setMCUTemperature(signed short ushort)
{
    telemetry[77] = ((unsigned char *)&ushort)[1];
    telemetry[78] = ((unsigned char *)&ushort)[0];
}
