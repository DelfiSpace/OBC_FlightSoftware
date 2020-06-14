/*
 * SDContainer.h
 *
 *  Created on: Jun 10, 2020
 *      Author: tom-h
 */

/*
 * Purpose of this container is to store variables that
 * are to be read/written from/to the SD card, this
 * way not every process will have to load the SD card themselves
 * but the sd card will be read/written periodically and its
 * contents will be saved in this container
 */
#ifndef DATACONTAINER_H_
#define DATACONTAINER_H_

#define CONTAINER_SIZE  40  //to be defined

// define the states, im not entirely sure if this is the right place
// but will do for now.
enum States {ACTIVATION, SAFE, DEPLOYMENT, ADCS, NOMINAL};

//if we want to know in which way deployment has happened
enum DeployState  {DELAYING, FORCED, NORMAL};
//OFF = not initialized, INITIALIZED= normal
enum PowerState {INITIALIZED, OFF, INITIALIZING, CYCLING};
//If this stays only 2 different states, would be better to use a boolean
//i.e. false = idle, true = detumbling
enum ADCSState {IDLE, DETUMBLE};

class DataContainer
{
protected:
    unsigned char Data[CONTAINER_SIZE];

public:
    int size();
    unsigned char * getArray();

    States getModeFlag();
    void setModeFlag(States State);

    //not sure if this should be long or short
    unsigned long getBootCount();
    void setBootCount(unsigned long count);

    unsigned long getUpTime();
    void setUpTime(unsigned long uplong);

    unsigned short getBatteryVoltage();
    void setBatteryVoltage(unsigned short battvolt);

    //something for health check results, make enum probably

    bool getTimerDone();
    void setTimerDone(bool timerdone);

    bool getDeployment();
    void setDeployment(bool deploy);

    unsigned short getDeployVoltage();
    void setDeployVoltage(unsigned short deployvolt);

    DeployState getDeployStatus();
    void setDeployStatus(DeployState state);

    //time assigned for normal deployment after which special measures will be taken
    unsigned short getDeployEnd();
    void setDeployEnd(unsigned short deployend);

    //delay parameter set to alter the deploytime
    unsigned short getDeployTime();
    void setDeployTime(unsigned short delaytime);

    unsigned short getDelayEnd();
    void setDelayEnd(unsigned short delayend);

    unsigned short getDeployDelay();
    void setDeployDelay(unsigned short deploydelay);

    unsigned short getSMVoltage();
    void setSMVoltage(unsigned short safevolt);

    bool getADCSEnable();
    void setADCSEnable(bool flag);

    ADCSState getADCSStatus();
    void setADCSStatus(ADCSState state);

    PowerState getADCSPowerStatus();
    void setADCSPowerStatus(PowerState state);

    //set variable to set time to detumble
    unsigned short getPowerCycleTime();
    void setPowerCycleTime(unsigned short time);

    //time set to indicate power cycling can end, why?
    unsigned short getPowerEnd();
    void setPowerEnd(unsigned short time);

    //time set to indicate initialization time is done
    unsigned short getInitEnd();
    void setInitEnd(unsigned short time);

    signed short getOmega();
    void setOmega(signed short value);

    //treshold set when the rotational speed is acceptable
    signed short getMaxOmega();
    void setMaxOmega(signed short value);

    //Time which the spacecraft will detumble for
    unsigned short getDetumbleTime();
    void setDetumbleTime(unsigned short time);

    //time at which detumbling will end
    unsigned short getDetumbleEnd();
    void setDetumbleEnd(unsigned short time);
};

DataContainer DC;


#endif /* DATACONTAINER_H_ */
