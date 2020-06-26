/*
 *  OBCVariableContainer.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */


#include <OBCVariableContainer.h>

int OBCVariableContainer::size()
{
    return OBC_VARIABLECONTAINER_SIZE;
}

unsigned char* OBCVariableContainer::getArray()
{
    return &data[0];
}

Mode OBCVariableContainer::getMode()
{
    return (Mode)data[0];
}

void OBCVariableContainer::setMode(Mode currentMode)
{
    data[0] = (unsigned char)currentMode;
}

ActivationState OBCVariableContainer::getActivationState()
{
    return (ActivationState)data[1];
}

void OBCVariableContainer::setActivationState(ActivationState state)
{
    data[1] = (unsigned char)state;
}

DeployState OBCVariableContainer::getDeployState()
{
    return (DeployState)data[2];
}

void OBCVariableContainer::setDeployState(DeployState state)
{
    data[2] = (unsigned char)state;
}

ADCSState OBCVariableContainer::getADCSState()
{
    return (ADCSState)data[3];
}

void OBCVariableContainer::setADCSState(ADCSState state)
{
    data[3] = (unsigned char)state;
}

PowerState OBCVariableContainer::getADCSPowerState()
{
    return (PowerState)data[4];
}

void OBCVariableContainer::setADCSPowerState(PowerState state)
{
    data[4] = (unsigned char)state;
}

char getADBResponse()
{
    return data[5];
}

void setADBResponse(char res)
{
    data[5] = res;
}

char getADCSResponse()
{
    return data[6];
}

void setADCSResponse(char res)
{
    data[6] = res;
}

char getCOMMSResponse()
{
    return data[7];
}

void setCOMMSResponse(char res)
{
    data[7] = res;
}

char getEPSResponse()
{
    return data[8];
}

void setEPSResponse(char res)
{
    data[8] = res;
}

char getPROPResponse()
{
    return data[9];
}

void setPROPResponse(char res)
{
    data[9] = res;
}

unsigned short OBCVariableContainer::getBatteryVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[10];
    ((unsigned char *)&ushort)[0] = data[11];
    return ushort;
}

void OBCVariableContainer::setBatteryVoltage(unsigned short battvolt)
{
    data[10] = ((unsigned char *)&battvolt)[1];
    data[11]] = ((unsigned char *)&battvolt)[0];
}

unsigned short OBCVariableContainer::getDeployVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[12];
    ((unsigned char *)&ushort)[0] = data[13];
    return ushort;
}

void OBCVariableContainer::setDeployVoltage(unsigned short deployvolt)
{
    data[12] = ((unsigned char *)&deployvolt)[1];
    data[13] = ((unsigned char *)&deployvolt)[0];
}

unsigned short OBCVariableContainer::getSMVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[14];
    ((unsigned char *)&ushort)[0] = data[15];
    return ushort;
}

void OBCVariableContainer::setSMVoltage(unsigned short safevoltage)
{
    data[14] = ((unsigned char *)&safevoltage)[1];
    data[15] = ((unsigned char *)&safevoltage)[0];
}

unsigned short OBCVariableContainer::getRotateSpeed()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[16];
    ((unsigned char *)&ushort)[0] = data[17];
    return ushort;
}

void OBCVariableContainer::setRotateSpeed(unsigned short value)
{
    data[16] = ((unsigned char *)&value)[1];
    data[17] = ((unsigned char *)&value)[0];
}

unsigned short OBCVariableContainer::getRotateSpeedLimit()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = data[18];
    ((unsigned char *)&ushort)[0] = data[19];
    return ushort;
}

void OBCVariableContainer::setRotateSpeedLimit(unsigned short value)
{
    data[18] = ((unsigned char *)&value)[1];
    data[19] = ((unsigned char *)&value)[0];
}

unsigned long OBCVariableContainer::getBootCount()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[20];
    ((unsigned char *)&ulong)[2] = data[21];
    ((unsigned char *)&ulong)[1] = data[22];
    ((unsigned char *)&ulong)[0] = data[23];
    return ulong;
}

void OBCVariableContainer::setBootCount(unsigned long count)
{
    data[20] = ((unsigned char *)&count)[4];
    data[21] = ((unsigned char *)&count)[3];
    data[22] = ((unsigned char *)&count)[2];
    data[23] = ((unsigned char *)&count)[1];
}

unsigned long OBCVariableContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[24];
    ((unsigned char *)&ulong)[2] = data[25];
    ((unsigned char *)&ulong)[1] = data[26];
    ((unsigned char *)&ulong)[0] = data[27];
    return ulong;
}

void OBCVariableContainer::setUpTime(unsigned long count)
{
    data[24] = ((unsigned char *)&count)[4];
    data[25] = ((unsigned char *)&count)[3];
    data[26] = ((unsigned char *)&count)[2];
    data[27] = ((unsigned char *)&count)[1];
}

unsigned long OBCVariableContainer::getTotalUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[28];
    ((unsigned char *)&ulong)[2] = data[29];
    ((unsigned char *)&ulong)[1] = data[30];
    ((unsigned char *)&ulong)[0] = data[31];
    return ulong;
}

void OBCVariableContainer::setTotalUpTime(unsigned long count)
{
    data[28] = ((unsigned char *)&count)[4];
    data[29] = ((unsigned char *)&count)[3];
    data[30] = ((unsigned char *)&count)[2];
    data[31] = ((unsigned char *)&count)[1];
}

unsigned long OBCVariableContainer::getEndOfActivation()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[32];
    ((unsigned char *)&ulong)[2] = data[33];
    ((unsigned char *)&ulong)[1] = data[34];
    ((unsigned char *)&ulong)[0] = data[35];
    return ulong;
}

void OBCVariableContainer::setEndOfActivation(unsigned long uplong)
{
    data[32] = ((unsigned char *)&uplong)[4];
    data[33] = ((unsigned char *)&uplong)[3];
    data[34] = ((unsigned char *)&uplong)[2];
    data[35] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getEndOfDeployState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[36];
    ((unsigned char *)&ulong)[2] = data[37];
    ((unsigned char *)&ulong)[1] = data[38];
    ((unsigned char *)&ulong)[0] = data[39];
    return ulong;
}

void OBCVariableContainer::setEndOfDeployState(unsigned long uplong)
{
    data[36] = ((unsigned char *)&uplong)[4];
    data[37] = ((unsigned char *)&uplong)[3];
    data[38] = ((unsigned char *)&uplong)[2];
    data[39] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getForcedDeployParameter()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[40];
    ((unsigned char *)&ulong)[2] = data[41];
    ((unsigned char *)&ulong)[1] = data[42];
    ((unsigned char *)&ulong)[0] = data[43];
    return ulong;
}

void OBCVariableContainer::setForcedDeployParameter(unsigned long uplong)
{
    data[40] = ((unsigned char *)&uplong)[4];
    data[41] = ((unsigned char *)&uplong)[3];
    data[42] = ((unsigned char *)&uplong)[2];
    data[43] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getDeployDelayParameter()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[44];
    ((unsigned char *)&ulong)[2] = data[45];
    ((unsigned char *)&ulong)[1] = data[46];
    ((unsigned char *)&ulong)[0] = data[47];
    return ulong;
}

void OBCVariableContainer::setDeployDelayParameter(unsigned long uplong)
{
    data[44] = ((unsigned char *)&uplong)[4];
    data[45] = ((unsigned char *)&uplong)[3];
    data[46] = ((unsigned char *)&uplong)[2];
    data[47] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getEndOfADCSState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[48];
    ((unsigned char *)&ulong)[2] = data[49];
    ((unsigned char *)&ulong)[1] = data[50];
    ((unsigned char *)&ulong)[0] = data[51];
    return ulong;
}

void OBCVariableContainer::setEndOfADCSState(unsigned long uplong)
{
    data[48] = ((unsigned char *)&uplong)[4];
    data[49] = ((unsigned char *)&uplong)[3];
    data[50] = ((unsigned char *)&uplong)[2];
    data[51] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getDetumblingPeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[52];
    ((unsigned char *)&ulong)[2] = data[53];
    ((unsigned char *)&ulong)[1] = data[54];
    ((unsigned char *)&ulong)[0] = data[55];
    return ulong;
}

void OBCVariableContainer::setDetumblingPeriod(unsigned long uplong)
{
    data[52] = ((unsigned char *)&uplong)[4];
    data[53] = ((unsigned char *)&uplong)[3];
    data[54] = ((unsigned char *)&uplong)[2];
    data[55] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getEndOfADCSPowerState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[56];
    ((unsigned char *)&ulong)[2] = data[57];
    ((unsigned char *)&ulong)[1] = data[58];
    ((unsigned char *)&ulong)[0] = data[59];
    return ulong;
}

void OBCVariableContainer::setEndOfADCSPowerState(unsigned long uplong)
{
    data[56] = ((unsigned char *)&uplong)[4];
    data[57] = ((unsigned char *)&uplong)[3];
    data[58] = ((unsigned char *)&uplong)[2];
    data[59] = ((unsigned char *)&uplong)[1];
}

unsigned long OBCVariableContainer::getADCSPowerCyclePeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = data[60];
    ((unsigned char *)&ulong)[2] = data[61];
    ((unsigned char *)&ulong)[1] = data[62];
    ((unsigned char *)&ulong)[0] = data[63];
    return ulong;
}

void OBCVariableContainer::setADCSPowerCyclePeriod(unsigned long uplong)
{
    data[60] = ((unsigned char *)&uplong)[4];
    data[61] = ((unsigned char *)&uplong)[3];
    data[62] = ((unsigned char *)&uplong)[2];
    data[63] = ((unsigned char *)&uplong)[1];
}
