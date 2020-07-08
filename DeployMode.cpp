/*
 * DeployMode.cpp
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

#include <DeployMode.h>

/*
 * Send a command to the antenna to deploy
 */
bool DeployAntenna() {
    bool done = false;
    char execute = 0x01;
    char request = 0x01;

    //char to store received command
    unsigned char* Reply;
    unsigned char ReplySize;
    unsigned char Payload[6];

    //Set upd data struture according to data structure defined in xml file
    Payload[0] = execute;
    Payload[1] = request;
    Payload[2] = 0x31; //fixed value of 49
    Payload[3] = 0x01; //switch. Todo check this value
    Payload[4] = 0x01; //Feedback on if 1, off if 0
    Payload[5] = 0x11; //todo set this value
    //Send to ADB
    char Success = RequestReply(ADB, 6, Payload, &ReplySize, &Reply, 500);

    if (Success) {
        done = true;
    }
    Console::log("Sending command to ADB.");
    return done;

}

/*
 * Check variable container of OBC to see if antenna has been deployed
 */
bool CheckDeploy(DeployState DS) {
    if(DS == DEPLOYED)
        return true;
    else
        return false;
}

/*
 * Check sensor data from ADB to see if antenna has been deployed
 * todo Sensordata has not been added to ADBtelemetry yet
 */
bool CheckDeployTelem(OBCVariableContainer *OBCVC, ADBTelemetryContainer *ADBTC) {
    if(0==1) {
        OBCVC->setDeployState(DEPLOYED);
        return true;
    }
    else
        return false;
}

/*
 * Check to see if the voltage is high enough for deployment
 * 3 scenarios:
 * - Normal deployment when BatVolt > threshold
 * - After 10h of no deployment forced deployment is done
 * - If deployment is unsuccessfull one will check after a time to see if it has deployed
 */
void CheckVoltTime(OBCVariableContainer *OBCVC, Mode *currentMode, unsigned long totalUptime,DeployState DS) {
#ifdef STATEMACHINE_DEBUG
    totalUptime = 360000;
#endif
    if((DS == NORMAL) && (totalUptime < OBCVC->getEndOfDeployState())) {
        if (OBCVC->getBatteryVoltage() > OBCVC->getDeployVoltage()) {
            if(DeployAntenna()) {
                *currentMode = SAFEMODE;
                OBCVC->setDeployState(DEPLOYED);
            }
            //Run if deployment was not successfull
            else {
                OBCVC->setDeployState(DELAYING);
                OBCVC->setDeployDelayTime(totalUptime + OBCVC->getDeployDelayParameter());
            }
        }
        else
            return;
    }
    else if((DS == NORMAL) && (totalUptime >= OBCVC->getEndOfDeployState())) {
        OBCVC->setDeployState(FORCED);
    }
    else if((DS == DELAYING) && (totalUptime > OBCVC->getDeployDelayTime())) {
        if(DeployAntenna()) {
            *currentMode = SAFEMODE;
            OBCVC->setDeployState(DEPLOYED);
        }
        else {
            OBCVC->setDeployState(DELAYING);
            OBCVC->setDeployDelayTime(totalUptime + OBCVC->getDeployDelayParameter());
        }
        //todo Check in telemetry if sensors say it has been deployed
        return;
    }
    else if((DS == DELAYING) && (totalUptime < OBCVC->getDeployDelayTime())) {
        return;
    }
    else if((DS == FORCED)) {
        if(DeployAntenna()) {
            *currentMode = SAFEMODE;
            OBCVC->setDeployState(DEPLOYED);
        }
        else {
            OBCVC->setDeployState(DELAYING);
            OBCVC->setDeployDelayTime(totalUptime + OBCVC->getDeployDelayParameter());
        }
    }
    return;
}

void DeployMode(OBCVariableContainer *OBCVC, ADBTelemetryContainer *ADBTC, Mode *currentMode, unsigned long totalUptime) {
    //Save variable to avoid multiple calls
#ifdef STATEMACHINE_DEBUG
    OBCVC->setBatteryVoltage(3600);
#endif
    DeployState DS = OBCVC->getDeployState();
    //first check if deployment already done by checking telemetry of ADB and OBC
    //todo check if this should be checked realtime
    if(CheckDeploy(DS) || CheckDeployTelem(OBCVC,ADBTC)) {
        *currentMode = SAFEMODE;
        return;
    }

    //then check if voltage is high enough and if so deploy
    else
        CheckVoltTime(OBCVC, currentMode, totalUptime, DS);
}




