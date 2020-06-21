/*
 *  OBCDataContainer.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */


#include <OBCDataContainer.h>

int OBCDataContainer::size()
{
    return OBC_DATACONTAINER_SIZE;
}

unsigned char* OBCDataContainer::getArray()
{
    return &data[0];
}

Mode OBCDataContainer::getMode()
{
    return (Mode)data[0];
}

void OBCDataContainer::setMode(Mode currentMode)
{
    data[0] = (unsigned char)currentMode;
}

ActivationState OBCDataContainer::getActivationState()
{
    return (ActivationState)data[1];
}

void OBCDataContainer::setActivationState(ActivationState state)
{
    data[1] = (unsigned char)state;
}

DeployState OBCDataContainer::getDeployState()
{
    return (DeployState)data[2];
}

void OBCDataContainer::setDeployState(DeployState state)
{
    data[2] = (unsigned char)state;
}

ADCSState OBCDataContainer::getADCSState()
{
    return (ADCSState)data[3];
}

void OBCDataContainer::setADCSState(ADCSState state)
{
    data[3] = (unsigned char)state;
}

PowerState OBCDataContainer::getADCSPowerState()
{
    return (PowerState)data[4];
}

void OBCDataContainer::setADCSPowerState(PowerState state)
{
    data[4] = (unsigned char)state;
}

unsigned short OBCDataContainer::getBatteryVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[5];
    ((unsigned char *)&ushort)[0] = data[6];
    return ushort;
}

void OBCDataContainer::setBatteryVoltage(unsigned short battvolt)
{
    data[5] = ((unsigned char *)&battvolt)[1];
    data[6] = ((unsigned char *)&battvolt)[0];
}

unsigned short OBCDataContainer::getDeployVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[7];
    ((unsigned char *)&ushort)[0] = data[8];
    return ushort;
}

void OBCDataContainer::setDeployVoltage(unsigned short deployvolt)
{
    data[7] = ((unsigned char *)&deployvolt)[1];
    data[8] = ((unsigned char *)&deployvolt)[0];
}

unsigned short OBCDataContainer::getSMVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[9];
    ((unsigned char *)&ushort)[0] = data[10];
    return ushort;
}

void OBCDataContainer::setSMVoltage(unsigned short safevoltage)
{
    data[9] = ((unsigned char *)&safevoltage)[1];
    data[10] = ((unsigned char *)&safevoltage)[0];
}

unsigned short OBCDataContainer::getRotateSpeed()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[11];
    ((unsigned char *)&ushort)[0] = data[12];
    return ushort;
}

void OBCDataContainer::setRotateSpeed(unsigned short value)
{
    data[11] = ((unsigned char *)&value)[1];
    data[12] = ((unsigned char *)&value)[0];
}

unsigned short OBCDataContainer::getRotateSpeedLimit()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[13];
    ((unsigned char *)&ushort)[0] = data[14];
    return ushort;
}

void OBCDataContainer::setRotateSpeedLimit(unsigned short value)
{
    data[13] = ((unsigned char *)&value)[1];
    data[14] = ((unsigned char *)&value)[0];
}

unsigned long OBCDataContainer::getBootCount()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[15];
    ((unsigned char *)&ulong)[2] = data[16];
    ((unsigned char *)&ulong)[1] = data[17];
    ((unsigned char *)&ulong)[0] = data[18];
    return ulong;
}

void OBCDataContainer::setBootCount(unsigned long count)
{
    data[15] = ((unsigned char *)&count)[4];
    data[16] = ((unsigned char *)&count)[3];
    data[17] = ((unsigned char *)&count)[2];
    data[18] = ((unsigned char *)&count)[1];
}

unsigned long OBCDataContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[19];
    ((unsigned char *)&ulong)[2] = data[20];
    ((unsigned char *)&ulong)[1] = data[21];
    ((unsigned char *)&ulong)[0] = data[22];
    return ulong;
}

void OBCDataContainer::setUpTime(unsigned long count)
{
    data[19] = ((unsigned char *)&count)[4];
    data[20] = ((unsigned char *)&count)[3];
    data[21] = ((unsigned char *)&count)[2];
    data[22] = ((unsigned char *)&count)[1];
}

unsigned long OBCDataContainer::getTotalUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[23];
    ((unsigned char *)&ulong)[2] = data[24];
    ((unsigned char *)&ulong)[1] = data[25];
    ((unsigned char *)&ulong)[0] = data[26];
    return ulong;
}

void OBCDataContainer::setTotalUpTime(unsigned long count)
{
    data[23] = ((unsigned char *)&count)[4];
    data[24] = ((unsigned char *)&count)[3];
    data[25] = ((unsigned char *)&count)[2];
    data[26] = ((unsigned char *)&count)[1];
}

unsigned long OBCDataContainer::getEndOfActivation()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[27];
    ((unsigned char *)&ulong)[2] = data[28];
    ((unsigned char *)&ulong)[1] = data[29];
    ((unsigned char *)&ulong)[0] = data[30];
    return ulong;
}

void OBCDataContainer::setEndOfActivation(unsigned long uplong)
{
    data[27] = ((unsigned char *)&uplong)[4];
    data[28] = ((unsigned char *)&uplong)[3];
    data[29] = ((unsigned char *)&uplong)[2];
    data[30] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getEndOfDeployState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[31];
    ((unsigned char *)&ulong)[2] = data[32];
    ((unsigned char *)&ulong)[1] = data[33];
    ((unsigned char *)&ulong)[0] = data[34];
    return ulong;
}

void OBCDataContainer::setEndOfDeployState(unsigned long uplong)
{
    data[31] = ((unsigned char *)&uplong)[4];
    data[32] = ((unsigned char *)&uplong)[3];
    data[33] = ((unsigned char *)&uplong)[2];
    data[34] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getForcedDeployParameter()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[35];
    ((unsigned char *)&ulong)[2] = data[36];
    ((unsigned char *)&ulong)[1] = data[37];
    ((unsigned char *)&ulong)[0] = data[38];
    return ulong;
}

void OBCDataContainer::setForcedDeployParameter(unsigned long uplong)
{
    data[35] = ((unsigned char *)&uplong)[4];
    data[36] = ((unsigned char *)&uplong)[3];
    data[37] = ((unsigned char *)&uplong)[2];
    data[38] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getDeployDelayParameter()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[39];
    ((unsigned char *)&ulong)[2] = data[40];
    ((unsigned char *)&ulong)[1] = data[41];
    ((unsigned char *)&ulong)[0] = data[42];
    return ulong;
}

void OBCDataContainer::setDeployDelayParameter(unsigned long uplong)
{
    data[39] = ((unsigned char *)&uplong)[4];
    data[40] = ((unsigned char *)&uplong)[3];
    data[41] = ((unsigned char *)&uplong)[2];
    data[42] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getEndOfADCSState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[43];
    ((unsigned char *)&ulong)[2] = data[44];
    ((unsigned char *)&ulong)[1] = data[45];
    ((unsigned char *)&ulong)[0] = data[46];
    return ulong;
}

void OBCDataContainer::setEndOfADCSState(unsigned long uplong)
{
    data[43] = ((unsigned char *)&uplong)[4];
    data[44] = ((unsigned char *)&uplong)[3];
    data[45] = ((unsigned char *)&uplong)[2];
    data[46] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getDetumblingPeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[47];
    ((unsigned char *)&ulong)[2] = data[48];
    ((unsigned char *)&ulong)[1] = data[49];
    ((unsigned char *)&ulong)[0] = data[50];
    return ulong;
}

void OBCDataContainer::setDetumblingPeriod(unsigned long uplong)
{
    data[47] = ((unsigned char *)&uplong)[4];
    data[48] = ((unsigned char *)&uplong)[3];
    data[49] = ((unsigned char *)&uplong)[2];
    data[50] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getEndOfADCSPowerState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[51];
    ((unsigned char *)&ulong)[2] = data[52];
    ((unsigned char *)&ulong)[1] = data[53];
    ((unsigned char *)&ulong)[0] = data[54];
    return ulong;
}

void OBCDataContainer::setEndOfADCSPowerState(unsigned long uplong)
{
    data[51] = ((unsigned char *)&uplong)[4];
    data[52] = ((unsigned char *)&uplong)[3];
    data[53] = ((unsigned char *)&uplong)[2];
    data[54] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCDataContainer::getADCSPowerCyclePeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[55];
    ((unsigned char *)&ulong)[2] = data[56];
    ((unsigned char *)&ulong)[1] = data[57];
    ((unsigned char *)&ulong)[0] = data[58];
    return ulong;
}

void OBCDataContainer::setADCSPowerCyclePeriod(unsigned long uplong)
{
    data[55] = ((unsigned char *)&uplong)[4];
    data[56] = ((unsigned char *)&uplong)[3];
    data[57] = ((unsigned char *)&uplong)[2];
    data[58] = ((unsigned char *)&uplong)[1];
}


