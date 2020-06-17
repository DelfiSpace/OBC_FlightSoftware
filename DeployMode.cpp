/*
 * DeployMode.cpp
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

#include <DeployMode.h>

void Deploy() {
    Console::log("Sending command to ADB.");
}

bool CheckDeploy(DeployState CurrentState) {
    if(CurrentState == DEPLOYED)
        return true;
    else
        return false;
}

bool CheckDeployTelem() {
    //check telemetry??
    return true;
}

void DeployAntenna(DeployState CurrentState, long uptime, long endtime) {
    if((CurrentState == NORMAL) & (uptime < endtime)) {
        if (DC.getBatteryVoltage() > DC.getDeployVoltage()) {
            //command ADB to deploy antenna
            DC.setMode(SAFE);
            DC.setDeployState(DELAYING); //why????
            //I think we need to add an extra variable for the end of delay time
            DC.setDeployDelayTime(uptime + DC.getDeployDelayParameter());
        }
    }
    else if((CurrentState == NORMAL) & (uptime >= endtime)) {
        DC.setDeployState(FORCED);
        DC.setDeployDelayTime(uptime + DC.getDeployDelayParameter());
    }
//    else if(CurrentState == FORCED) {
//        //command ADB to deploy
//        DC.setDeployState(DELAYING);
//
//    }
    else if((CurrentState == FORCED) & (uptime < DC.getDeployDelayTime())) {
        return;
    }
    else if((CurrentState == FORCED) & (uptime >= DC.getDeployDelayTime())) {
        //Command ADB to deploy
        DC.setMode(SAFE);
        DC.setDeployState(NORMAL);
    }
    return;
}

void DeployMode () {
    //Here I save it in a variable to avoid consecutive calls to the same function
    DeployState CurrentState = DC.getDeployState();
    long UpTime = DC.getTotalUpTime();
    long EndTime = DC.getEndOfDeployState();
    if (CheckDeploy(CurrentState) && CheckDeployTelem()) {
        DC.setMode(SAFE);
        //add deployment done to database
        DC.setDeployState(DEPLOYED);
        return;
    }
    else
        DeployAntenna(CurrentState,UpTime,EndTime);
}




