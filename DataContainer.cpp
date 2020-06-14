/*
 * SDContainer.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */


#include <DataContainer.h>

int DataContainer::size()
{
    return CONTAINER_SIZE;
}

unsigned char* DataContainer::getArray()
{
    return &Data[0];
}

States DataContainer::getModeFlag()
{
    States catchstate;
    ((unsigned char *)&catchstate)[0] = Data[0];
    return catchstate;
}

void DataContainer::setModeFlag(States flag)
{
    Data[0] = flag;
}

DeployState DataContainer::getDeployStatus()
{
    DeployState catchstate;
    ((unsigned char *)&catchstate)[0] = Data[1];
    return catchstate;
}

void DataContainer::setDeployStatus(DeployState state)
{
    Data[1] = state;
}

ADCSState DataContainer::getADCSStatus()
{
    ADCSState catchstate;
    ((unsigned char *)&catchstate)[0] = Data[2];
    return catchstate;
}

void DataContainer::setADCSStatus(ADCSState state)
{
    Data[2] = state;
}

PowerState DataContainer::getADCSPowerStatus()
{
    PowerState catchstate;
    ((unsigned char *)&catchstate)[0] = Data[3];
    return catchstate;
}

void DataContainer::setADCSPowerStatus(PowerState state)
{
    Data[3] = state;
}

bool DataContainer::getTimerDone()
{
    return ((Data[4] & 0x02) != 0);
}

void DataContainer::setTimerDone(bool timerdone)
{
    Data[4] &= (~0x02);
    Data[4] |= timerdone ? 0x02 : 0x00;
}

bool DataContainer::getDeployment()
{
    return ((Data[5] & 0x02) != 0);
}

void DataContainer::setDeployment(bool deploy)
{
    Data[5] &= (~0x02);
    Data[5] |= deploy ? 0x02 : 0x00;
}

bool DataContainer::getADCSEnable()
{
    return ((Data[6] & 0x02) != 0);
}

void DataContainer::setADCSEnable(bool flag)
{
    Data[6] &= (~0x02);
    Data[6] |= flag ? 0x02 : 0x00;
}
unsigned long DataContainer::getBootCount()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = Data[7];
    ((unsigned char *)&ulong)[2] = Data[8];
    ((unsigned char *)&ulong)[1] = Data[9];
    ((unsigned char *)&ulong)[0] = Data[10];
    return ulong;
}

void DataContainer::setBootCount(unsigned long count)
{
    *((unsigned long *)&(Data[7])) = count;
    Data[7] = ((unsigned char *)&count)[4];
    Data[8] = ((unsigned char *)&count)[3];
    Data[9] = ((unsigned char *)&count)[2];
    Data[10] = ((unsigned char *)&count)[1];
}

unsigned long DataContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = Data[11];
    ((unsigned char *)&ulong)[2] = Data[12];
    ((unsigned char *)&ulong)[1] = Data[13];
    ((unsigned char *)&ulong)[0] = Data[14];
    return ulong;
}

void DataContainer::setUpTime(unsigned long count)
{
    *((unsigned long *)&(Data[11])) = count;
    Data[11] = ((unsigned char *)&count)[4];
    Data[12] = ((unsigned char *)&count)[3];
    Data[13] = ((unsigned char *)&count)[2];
    Data[14] = ((unsigned char *)&count)[1];
}

unsigned short DataContainer::getBatteryVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[15];
    ((unsigned char *)&ushort)[0] = Data[16];
    return ushort;
}

void DataContainer::setBatteryVoltage(unsigned short battvolt)
{
    Data[15] = ((unsigned char *)&battvolt)[1];
    Data[16] = ((unsigned char *)&battvolt)[0];
}

unsigned short DataContainer::getDeployVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[17];
    ((unsigned char *)&ushort)[0] = Data[18];
    return ushort;
}

void DataContainer::setDeployVoltage(unsigned short deployvolt)
{
    Data[17] = ((unsigned char *)&deployvolt)[1];
    Data[18] = ((unsigned char *)&deployvolt)[0];
}


unsigned short DataContainer::getDeployEnd()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[19];
    ((unsigned char *)&ushort)[0] = Data[20];
    return ushort;
}

void DataContainer::setDeployEnd(unsigned short deployend)
{
    Data[19] = ((unsigned char *)&deployend)[1];
    Data[20] = ((unsigned char *)&deployend)[0];
}

unsigned short DataContainer::getDeployTime()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[21];
    ((unsigned char *)&ushort)[0] = Data[22];
    return ushort;
}

void DataContainer::setDeployTime(unsigned short deploytime)
{
    Data[21] = ((unsigned char *)&deploytime)[1];
    Data[22] = ((unsigned char *)&deploytime)[0];
}

unsigned short DataContainer::getDeployDelay()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[23];
    ((unsigned char *)&ushort)[0] = Data[24];
    return ushort;
}

void DataContainer::setDeployDelay(unsigned short deploydelay)
{
    Data[23] = ((unsigned char *)&deploydelay)[1];
    Data[24] = ((unsigned char *)&deploydelay)[0];
}

unsigned short DataContainer::getSMVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[25];
    ((unsigned char *)&ushort)[0] = Data[26];
    return ushort;
}

void DataContainer::setSMVoltage(unsigned short safevoltage)
{
    Data[25] = ((unsigned char *)&safevoltage)[1];
    Data[26] = ((unsigned char *)&safevoltage)[0];
}

unsigned short DataContainer::getPowerCycleTime()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[27];
    ((unsigned char *)&ushort)[0] = Data[28];
    return ushort;
}

void DataContainer::setPowerCycleTime(unsigned short time)
{
    Data[27] = ((unsigned char *)&time)[1];
    Data[28] = ((unsigned char *)&time)[0];
}

unsigned short DataContainer::getPowerEnd()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[29];
    ((unsigned char *)&ushort)[0] = Data[30];
    return ushort;
}

void DataContainer::setPowerEnd(unsigned short time)
{
    Data[29] = ((unsigned char *)&time)[1];
    Data[30] = ((unsigned char *)&time)[0];
}

unsigned short DataContainer::getInitEnd()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[31];
    ((unsigned char *)&ushort)[0] = Data[32];
    return ushort;
}

void DataContainer::setInitEnd(unsigned short time)
{
    Data[31] = ((unsigned char *)&time)[1];
    Data[32] = ((unsigned char *)&time)[0];
}

signed short DataContainer::getOmega()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[33];
    ((unsigned char *)&ushort)[0] = Data[34];
    return ushort;
}

void DataContainer::setOmega(signed short value)
{
    Data[33] = ((unsigned char *)&value)[1];
    Data[34] = ((unsigned char *)&value)[0];
}

signed short DataContainer::getMaxOmega()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[35];
    ((unsigned char *)&ushort)[0] = Data[36];
    return ushort;
}

void DataContainer::setMaxOmega(signed short value)
{
    Data[35] = ((unsigned char *)&value)[1];
    Data[36] = ((unsigned char *)&value)[0];
}

unsigned short DataContainer::getDetumbleTime()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[37];
    ((unsigned char *)&ushort)[0] = Data[38];
    return ushort;
}

void DataContainer::setDetumbleTime(unsigned short time)
{
    Data[37] = ((unsigned char *)&time)[1];
    Data[38] = ((unsigned char *)&time)[0];
}

unsigned short DataContainer::getDetumbleEnd()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = Data[39];
    ((unsigned char *)&ushort)[0] = Data[40];
    return ushort;
}

void DataContainer::setDetumbleEnd(unsigned short time)
{
    Data[39] = ((unsigned char *)&time)[1];
    Data[40] = ((unsigned char *)&time)[0];
}
