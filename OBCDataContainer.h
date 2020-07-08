/*
 *  OBCDataContainer.h
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */

#ifndef OBCDATACONTAINER_H_
#define OBCDATACONTAINER_H_

#define OBC_DATACONTAINER_SIZE  64

typedef enum Mode {ACTIVATION, DEPLOYMENT, SAFE, ADCS, NOMINAL} Mode;
typedef enum ActivationState  {UNEXPIRED, EXPIRED} ActivationState;
typedef enum DeployState  {NORMAL, FORCED, DELAYING, DEPLOYED} DeployState;
typedef enum ADCSState {IDLE, DETUMBLE, DISABLED} ADCSState;

// Can be used in every mode for power line V2, V3 and V4
typedef enum PowerState {INITIALIZING, INITIALIZED, CYCLING, OFF} PowerState;

class OBCDataContainer
{
protected:
    unsigned char data[OBC_DATACONTAINER_SIZE];

public:
    int size();
    unsigned char * getArray();

    // Variables which are used in every mode

    Mode getMode();
    void setMode(Mode currentMode);

    unsigned long getBootCount();
    void setBootCount(unsigned long count);

    unsigned long getUpTime(); // Uptime since the last boot
    void setUpTime(unsigned long uplong);

    unsigned long getTotalUpTime(); // Uptime since the first boot
    void setTotalUpTime(unsigned long uplong);

    unsigned short getBatteryVoltage();
    void setBatteryVoltage(unsigned short battvolt);

    //TODO: something for health check results, make enum probably

    // Variables in the activation mode

    ActivationState getActivationState();
    void setActivationState(ActivationState state);

    unsigned long getEndOfActivation();
    void setEndOfActivation(unsigned long uplong);

    // Variables in the deployment mode

    DeployState getDeployState();
    void setDeployState(DeployState state);

    unsigned long getEndOfDeployState();
    void setEndOfDeployState(unsigned long uplong);

    unsigned short getDeployVoltage();
    void setDeployVoltage(unsigned short deployvolt);

    unsigned long getForcedDeployParameter();
    void setForcedDeployParameter(unsigned long uplong);

    unsigned long getDeployDelayParameter();
    void setDeployDelayParameter(unsigned long uplong);



    // Variables in the safe mode

    unsigned short getSMVoltage();
    void setSMVoltage(unsigned short safevolt);

    // Variables in the ADCS mode

    ADCSState getADCSState();
    void setADCSState(ADCSState state);

    unsigned long getEndOfADCSState();
    void setEndOfADCSState(unsigned long uplong);

    unsigned short getRotateSpeed(); // TODO: signed or unsigned?
    void setRotateSpeed(unsigned short value);

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

    unsigned long getDeployDelayTime();
    void setDeployDelayTime(unsigned long uplong);

    //this is set after the timer is done, this can be checked during future boots
    //instead of the two times(totaluptime and waittime).
    bool getTimerDone();
    void setTimerDone(bool done);
};

//Datacontainer
OBCDataContainer OBCDC;

#endif /* OBCDATACONTAINER_H_ */
