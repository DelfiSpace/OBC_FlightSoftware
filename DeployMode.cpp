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

void DeployAntenna(OBCDataContainer *container, DeployState CurrentState, long uptime, long endtime) {
    if((CurrentState == NORMAL) & (uptime < endtime)) {
        if (container->getBatteryVoltage() > container->getDeployVoltage()) {
            //command ADB to deploy antenna
            container->setMode(SAFE);
            container->setDeployState(DELAYING); //why????
            //I think we need to add an extra variable for the end of delay time
            container->setDeployDelayTime(uptime + container->getDeployDelayParameter());
        }
    }
    else if((CurrentState == NORMAL) & (uptime >= endtime)) {
        container->setDeployState(FORCED);
        container->setDeployDelayTime(uptime + container->getDeployDelayParameter());
    }
//    else if(CurrentState == FORCED) {
//        //command ADB to deploy
//        container.setDeployState(DELAYING);
//
//    }
    else if((CurrentState == FORCED) & (uptime < container->getDeployDelayTime())) {
        return;
    }
    else if((CurrentState == FORCED) & (uptime >= container->getDeployDelayTime())) {
        //Command ADB to deploy
        container->setMode(SAFE);
        container->setDeployState(NORMAL);
    }
    return;
}

void DeployMode (OBCDataContainer *container) {
    //Here I save it in a variable to avoid consecutive calls to the same function
    DeployState CurrentState = container->getDeployState();
    long UpTime = container->getTotalUpTime();
    long EndTime = container->getEndOfDeployState();
    if (CheckDeploy(CurrentState) && CheckDeployTelem()) {
        container->setMode(SAFE);
        //add deployment done to database
        container->setDeployState(DEPLOYED);
        return;
    }
    else
        DeployAntenna(container,CurrentState,UpTime,EndTime);
}




