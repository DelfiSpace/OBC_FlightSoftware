/*
 *  OBCTelemetryContainer.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */

#include <OBCTelemetryContainer.h>

// Initialization functions

void OBCTelemetryContainer::NormalInit()
{
    // These variables are read from FRAM but modified
    setMode(ACTIVATIONMODE);
    setUpTime(0);
    setBootCount(getBootCount() + 1);

    setADCSPowerState(UNINITIALIZED);
    setEndOfADCSPowerState(0);
}

void OBCTelemetryContainer::FirstBootInit()
{
    setMode(ACTIVATIONMODE);
    setUpTime(0);
    setTotalUpTime(0);
    setBootCount(1);

    setBusStatus(false);
    setTMPStatus(false);
    setBusVoltage(0);
    setBusCurrent(0);
    setTemperature(0);

    setADBResponse(3); // SERVICE_NO_RESPONSE: 3
    setADCSResponse(3);
    setCOMMSResponse(3);
    setEPSResponse(3);
    setPROPResponse(3);

    setEndOfActivation(1800); // 30 mins

    setDeployState(PREPARING_DL);
    setEndOfDeployState(36000); // 10 hrs
    setDeployVoltage(3300); // mV
    setForcedDeployPeriod(36000); // 10 hrs
    setDelayingDeployPeriod(180); // 3 mins

    setSMVoltage(3600); // mV

    setADCSState(IDLE);
    setEndOfADCSState(0);
    setRotateSpeedLimit(5); // deg/s
    setDetumblingPeriod(3600); // Not sure! TODO!
    setADCSPowerState(UNINITIALIZED);
    setEndOfADCSPowerState(0);
    setADCSPowerCyclePeriod(16); // seconds
}

// The whole telemetry array

int OBCTelemetryContainer::size()
{
    return OBC_CONTAINER_SIZE;
}

unsigned char* OBCTelemetryContainer::getArray()
{
    return &telemetry[0];
}

// The second part of the telemetry array (only including changeable variables)

int OBCTelemetryContainer::VariablesSize()
{
    return OBC_VARIABLE_SIZE;
}

unsigned char* OBCTelemetryContainer::getVariablesArray()
{
    return &telemetry[OBC_CONTAINER_SIZE - OBC_VARIABLE_SIZE];
}

// Telemetry (not changable)

unsigned long OBCTelemetryContainer::getBootCount()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[0];
    ((unsigned char *)&ulong)[2] = telemetry[1];
    ((unsigned char *)&ulong)[1] = telemetry[2];
    ((unsigned char *)&ulong)[0] = telemetry[3];
    return ulong;
}

void OBCTelemetryContainer::setBootCount(unsigned long count)
{
    telemetry[0] = ((unsigned char *)&count)[3];
    telemetry[1] = ((unsigned char *)&count)[2];
    telemetry[2] = ((unsigned char *)&count)[1];
    telemetry[3] = ((unsigned char *)&count)[0];
}

unsigned long OBCTelemetryContainer::getUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[4];
    ((unsigned char *)&ulong)[2] = telemetry[5];
    ((unsigned char *)&ulong)[1] = telemetry[6];
    ((unsigned char *)&ulong)[0] = telemetry[7];
    return ulong;
}

void OBCTelemetryContainer::setUpTime(unsigned long count)
{
    telemetry[4] = ((unsigned char *)&count)[3];
    telemetry[5] = ((unsigned char *)&count)[2];
    telemetry[6] = ((unsigned char *)&count)[1];
    telemetry[7] = ((unsigned char *)&count)[0];
}

unsigned long OBCTelemetryContainer::getTotalUpTime()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[8];
    ((unsigned char *)&ulong)[2] = telemetry[9];
    ((unsigned char *)&ulong)[1] = telemetry[10];
    ((unsigned char *)&ulong)[0] = telemetry[11];
    return ulong;
}

void OBCTelemetryContainer::setTotalUpTime(unsigned long count)
{
    telemetry[8] = ((unsigned char *)&count)[3];
    telemetry[9] = ((unsigned char *)&count)[2];
    telemetry[10] = ((unsigned char *)&count)[1];
    telemetry[11] = ((unsigned char *)&count)[0];
}

bool OBCTelemetryContainer::getBusStatus()
{
    return ((telemetry[12] & 0x80) != 0);
}

void OBCTelemetryContainer::setBusStatus(bool bval)
{
    telemetry[12] &= (~0x80);
    telemetry[12] |= bval ? 0x80 : 0x00;
}

bool OBCTelemetryContainer::getTMPStatus()
{
    return ((telemetry[12] & 0x40) != 0);
}

void OBCTelemetryContainer::setTMPStatus(bool bval)
{
    telemetry[12] &= (~0x40);
    telemetry[12] |= bval ? 0x40 : 0x00;
}

unsigned short OBCTelemetryContainer::getBusVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[13];
    ((unsigned char *)&ushort)[0] = telemetry[14];
    return ushort;
}

void OBCTelemetryContainer::setBusVoltage(unsigned short battvolt)
{
    telemetry[13] = ((unsigned char *)&battvolt)[1];
    telemetry[14] = ((unsigned char *)&battvolt)[0];
}

signed short OBCTelemetryContainer::getBusCurrent()
{
    signed short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[15];
    ((unsigned char *)&ushort)[0] = telemetry[16];
    return ushort;
}

void OBCTelemetryContainer::setBusCurrent(signed short current)
{
    telemetry[15] = ((unsigned char *)&current)[1];
    telemetry[16] = ((unsigned char *)&current)[0];
}

signed short OBCTelemetryContainer::getTemperature()
{
    signed short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[17];
    ((unsigned char *)&ushort)[0] = telemetry[18];
    return ushort;
}

void OBCTelemetryContainer::setTemperature(signed short temp)
{
    telemetry[17] = ((unsigned char *)&temp)[1];
    telemetry[18] = ((unsigned char *)&temp)[0];
}

unsigned char OBCTelemetryContainer::getADBResponse()
{
    return telemetry[19];
}

void OBCTelemetryContainer::setADBResponse(unsigned char res)
{
    telemetry[19] = res;
}

unsigned char OBCTelemetryContainer::getADCSResponse()
{
    return telemetry[20];
}

void OBCTelemetryContainer::setADCSResponse(unsigned char res)
{
    telemetry[20] = res;
}

unsigned char OBCTelemetryContainer::getCOMMSResponse()
{
    return telemetry[21];
}

void OBCTelemetryContainer::setCOMMSResponse(unsigned char res)
{
    telemetry[21] = res;
}

unsigned char OBCTelemetryContainer::getEPSResponse()
{
    return telemetry[22];
}

void OBCTelemetryContainer::setEPSResponse(unsigned char res)
{
    telemetry[22] = res;
}

unsigned char OBCTelemetryContainer::getPROPResponse()
{
    return telemetry[23];
}

void OBCTelemetryContainer::setPROPResponse(unsigned char res)
{
    telemetry[23] = res;
}

// Variables in every mode

Mode OBCTelemetryContainer::getMode()
{
    return (Mode)telemetry[24];
}

void OBCTelemetryContainer::setMode(Mode currentMode)
{
    telemetry[24] = (unsigned char)currentMode;
}

// Variables in the activation mode

unsigned long OBCTelemetryContainer::getEndOfActivation()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[25];
    ((unsigned char *)&ulong)[2] = telemetry[26];
    ((unsigned char *)&ulong)[1] = telemetry[27];
    ((unsigned char *)&ulong)[0] = telemetry[28];
    return ulong;
}

void OBCTelemetryContainer::setEndOfActivation(unsigned long uplong)
{
    telemetry[25] = ((unsigned char *)&uplong)[3];
    telemetry[26] = ((unsigned char *)&uplong)[2];
    telemetry[27] = ((unsigned char *)&uplong)[1];
    telemetry[28] = ((unsigned char *)&uplong)[0];
}

// Variables in the deployment mode

DeployState OBCTelemetryContainer::getDeployState()
{
    return (DeployState)telemetry[29];
}

void OBCTelemetryContainer::setDeployState(DeployState state)
{
    telemetry[29] = (unsigned char)state;
}

unsigned long OBCTelemetryContainer::getEndOfDeployState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[30];
    ((unsigned char *)&ulong)[2] = telemetry[31];
    ((unsigned char *)&ulong)[1] = telemetry[32];
    ((unsigned char *)&ulong)[0] = telemetry[33];
    return ulong;
}

void OBCTelemetryContainer::setEndOfDeployState(unsigned long uplong)
{
    telemetry[30] = ((unsigned char *)&uplong)[3];
    telemetry[31] = ((unsigned char *)&uplong)[2];
    telemetry[32] = ((unsigned char *)&uplong)[1];
    telemetry[33] = ((unsigned char *)&uplong)[0];
}

unsigned short OBCTelemetryContainer::getDeployVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[34];
    ((unsigned char *)&ushort)[0] = telemetry[35];
    return ushort;
}

void OBCTelemetryContainer::setDeployVoltage(unsigned short deployvolt)
{
    telemetry[34] = ((unsigned char *)&deployvolt)[1];
    telemetry[35] = ((unsigned char *)&deployvolt)[0];
}

unsigned long OBCTelemetryContainer::getForcedDeployPeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[36];
    ((unsigned char *)&ulong)[2] = telemetry[37];
    ((unsigned char *)&ulong)[1] = telemetry[38];
    ((unsigned char *)&ulong)[0] = telemetry[39];
    return ulong;
}

void OBCTelemetryContainer::setForcedDeployPeriod(unsigned long uplong)
{
    telemetry[36] = ((unsigned char *)&uplong)[3];
    telemetry[37] = ((unsigned char *)&uplong)[2];
    telemetry[38] = ((unsigned char *)&uplong)[1];
    telemetry[39] = ((unsigned char *)&uplong)[0];
}

unsigned long OBCTelemetryContainer::getDelayingDeployPeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[40];
    ((unsigned char *)&ulong)[2] = telemetry[41];
    ((unsigned char *)&ulong)[1] = telemetry[42];
    ((unsigned char *)&ulong)[0] = telemetry[43];
    return ulong;
}

void OBCTelemetryContainer::setDelayingDeployPeriod(unsigned long uplong)
{
    telemetry[40] = ((unsigned char *)&uplong)[3];
    telemetry[41] = ((unsigned char *)&uplong)[2];
    telemetry[42] = ((unsigned char *)&uplong)[1];
    telemetry[43] = ((unsigned char *)&uplong)[0];
}

// Variables in the safe mode

unsigned short OBCTelemetryContainer::getSMVoltage()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[45];
    ((unsigned char *)&ushort)[0] = telemetry[46];
    return ushort;
}

void OBCTelemetryContainer::setSMVoltage(unsigned short safevoltage)
{
    telemetry[45] = ((unsigned char *)&safevoltage)[1];
    telemetry[46] = ((unsigned char *)&safevoltage)[0];
}

// Variables in the ADCS mode

ADCSState OBCTelemetryContainer::getADCSState()
{
    return (ADCSState)telemetry[47];
}

void OBCTelemetryContainer::setADCSState(ADCSState state)
{
    telemetry[47] = (unsigned char)state;
}

unsigned long OBCTelemetryContainer::getEndOfADCSState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[48];
    ((unsigned char *)&ulong)[2] = telemetry[49];
    ((unsigned char *)&ulong)[1] = telemetry[50];
    ((unsigned char *)&ulong)[0] = telemetry[51];
    return ulong;
}

void OBCTelemetryContainer::setEndOfADCSState(unsigned long uplong)
{
    telemetry[48] = ((unsigned char *)&uplong)[3];
    telemetry[49] = ((unsigned char *)&uplong)[2];
    telemetry[50] = ((unsigned char *)&uplong)[1];
    telemetry[51] = ((unsigned char *)&uplong)[0];
}

unsigned short OBCTelemetryContainer::getRotateSpeedLimit()
{
    unsigned short ushort;
    ((unsigned char *)&ushort)[1] = telemetry[52];
    ((unsigned char *)&ushort)[0] = telemetry[53];
    return ushort;
}

void OBCTelemetryContainer::setRotateSpeedLimit(unsigned short value)
{
    telemetry[52] = ((unsigned char *)&value)[1];
    telemetry[53] = ((unsigned char *)&value)[0];
}


unsigned long OBCTelemetryContainer::getDetumblingPeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[54];
    ((unsigned char *)&ulong)[2] = telemetry[55];
    ((unsigned char *)&ulong)[1] = telemetry[56];
    ((unsigned char *)&ulong)[0] = telemetry[57];
    return ulong;
}

void OBCTelemetryContainer::setDetumblingPeriod(unsigned long uplong)
{
    telemetry[54] = ((unsigned char *)&uplong)[3];
    telemetry[55] = ((unsigned char *)&uplong)[2];
    telemetry[56] = ((unsigned char *)&uplong)[1];
    telemetry[57] = ((unsigned char *)&uplong)[0];
}

PowerState OBCTelemetryContainer::getADCSPowerState()
{
    return (PowerState)telemetry[58];
}

void OBCTelemetryContainer::setADCSPowerState(PowerState state)
{
    telemetry[58] = (unsigned char)state;
}


unsigned long OBCTelemetryContainer::getEndOfADCSPowerState()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[59];
    ((unsigned char *)&ulong)[2] = telemetry[60];
    ((unsigned char *)&ulong)[1] = telemetry[61];
    ((unsigned char *)&ulong)[0] = telemetry[62];
    return ulong;
}

void OBCTelemetryContainer::setEndOfADCSPowerState(unsigned long uplong)
{
    telemetry[59] = ((unsigned char *)&uplong)[3];
    telemetry[60] = ((unsigned char *)&uplong)[2];
    telemetry[61] = ((unsigned char *)&uplong)[1];
    telemetry[62] = ((unsigned char *)&uplong)[0];
}

unsigned long OBCTelemetryContainer::getADCSPowerCyclePeriod()
{
    unsigned long ulong;
    ((unsigned char *)&ulong)[3] = telemetry[63];
    ((unsigned char *)&ulong)[2] = telemetry[64];
    ((unsigned char *)&ulong)[1] = telemetry[65];
    ((unsigned char *)&ulong)[0] = telemetry[66];
    return ulong;
}

void OBCTelemetryContainer::setADCSPowerCyclePeriod(unsigned long uplong)
{
    telemetry[63] = ((unsigned char *)&uplong)[3];
    telemetry[64] = ((unsigned char *)&uplong)[2];
    telemetry[65] = ((unsigned char *)&uplong)[1];
    telemetry[66] = ((unsigned char *)&uplong)[0];
}
