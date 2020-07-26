/*
 *  OBCTelemetryContainer.h
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */

#ifndef OBCTELEMETRYCONTAINER_H_
#define OBCTELEMETRYCONTAINER_H_

#include "TelemetryContainer.h"

#define OBC_CONTAINER_SIZE  67
#define OBC_VARIABLE_SIZE      43

typedef enum Mode {ACTIVATIONMODE, DEPLOYMENTMODE, SAFEMODE, ADCSMODE, NOMINALMODE} Mode;

// States of Delfi-PQ in different mode (DL: downlink antanna, UL: uplink antenna)
typedef enum DeployState  {PREPARING_DL, DELAYING_DL, PREPARING_UL, DELAYING_UL, DEPLOYED} DeployState;
typedef enum ADCSState {IDLE, DETUMBLE, DISABLED} ADCSState;

// Can be used in every mode for power line V2, V3 and V4
typedef enum PowerState {UNINITIALIZED, INITIALIZED, CYCLED, OFF} PowerState;

// Health check results of subsystems (TODO)
typedef enum ADBHealthResult {ADB_BAD, ADB_GOOD} ADBHealthResult;
typedef enum ADCSHealthResult {ADCS_BAD, ADCS_GOOD} ADCSHealthResult;
typedef enum COMMSHealthResult {COMMS_BAD, COMMS_GOOD} COMMSHealthResult;
typedef enum OBCHealthResult {OBC_BAD, OBC_GOOD} OBCHealthResult;
typedef enum EPSHealthResult {EPS_BAD, EPS_GOOD} EPSHealthResult;
typedef enum PROPHealthResult {PROP_BAD, PROP_GOOD} PROPHealthResult;

class OBCTelemetryContainer : public TelemetryContainer
{
protected:
    unsigned char telemetry[OBC_CONTAINER_SIZE];

public:

    // Initialization functions

    void NormalInit();
    void FirstBootInit();

    // The whole telemetry array

    int size();
    unsigned char * getArray();

    // The second part of the telemetry array (only including changeable variables)

    int VariablesSize();
    unsigned char * getVariablesArray();

    // Telemetry (not changable)

    unsigned long getBootCount();
    void setBootCount(unsigned long count);

    unsigned long getUpTime(); // Uptime since the last boot
    void setUpTime(unsigned long uplong);

    unsigned long getTotalUpTime(); // Uptime since the first boot
    void setTotalUpTime(unsigned long uplong);

    bool getBusStatus();
    void setBusStatus(bool bval);

    bool getTMPStatus();
    void setTMPStatus(bool bval);

    unsigned short getBusVoltage();
    void setBusVoltage(unsigned short battvolt);

    signed short getBusCurrent();
    void setBusCurrent(signed short current);

    signed short getTemperature();
    void setTemperature(signed short temp);

    unsigned char getADBResponse();
    void setADBResponse(unsigned char res);

    unsigned char getADCSResponse();
    void setADCSResponse(unsigned char res);

    unsigned char getCOMMSResponse();
    void setCOMMSResponse(unsigned char res);

    unsigned char getEPSResponse();
    void setEPSResponse(unsigned char res);

    unsigned char getPROPResponse();
    void setPROPResponse(unsigned char res);

    // Variables in every mode

    Mode getMode();
    void setMode(Mode currentMode);

    // Variables in the activation mode

    unsigned long getEndOfActivation();
    void setEndOfActivation(unsigned long uplong);

    // Variables in the deployment mode

    DeployState getDeployState();
    void setDeployState(DeployState state);

    unsigned long getEndOfDeployState();
    void setEndOfDeployState(unsigned long uplong);

    unsigned short getDeployVoltage();
    void setDeployVoltage(unsigned short deployvolt);

    unsigned long getForcedDeployPeriod();
    void setForcedDeployPeriod(unsigned long uplong);

    unsigned long getDelayingDeployPeriod();
    void setDelayingDeployPeriod(unsigned long uplong);

    // Variables in the safe mode

    unsigned short getSMVoltage();
    void setSMVoltage(unsigned short safevolt);

    // Variables in the ADCS mode

    ADCSState getADCSState();
    void setADCSState(ADCSState state);

    unsigned long getEndOfADCSState();
    void setEndOfADCSState(unsigned long uplong);

    unsigned short getRotateSpeedLimit(); // TODO: signed or unsigned?
    void setRotateSpeedLimit(unsigned short value);

    unsigned long getDetumblingPeriod();
    void setDetumblingPeriod(unsigned long uplong);

    PowerState getADCSPowerState();
    void setADCSPowerState(PowerState state);

    unsigned long getEndOfADCSPowerState();
    void setEndOfADCSPowerState(unsigned long uplong);

    unsigned long getADCSPowerCyclePeriod();
    void setADCSPowerCyclePeriod(unsigned long uplong);
};

#endif /* OBCTELEMETRYCONTAINER_H_ */
