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

bool CheckDeploy(variables *var) {
    if(var->currentMode == DEPLOYED)
        return true;
    else
        return false;
}

bool CheckDeployTelem() {
    //check telemetry??
    return true;
}

void DeployAntenna(OBCDataContainer *container, variables *var, long uptime, long endtime) {
    if((var->currentMode == NORMAL) & (uptime < endtime)) {
        if (container->getBatteryVoltage() > container->getDeployVoltage()) {
            //command ADB to deploy antenna
            var->currentMode = SAFE;
            container->setDeployState(DELAYING); //why????
            //I think we need to add an extra variable for the end of delay time
            container->setDeployDelayTime(uptime + container->getDeployDelayParameter());
        }
    }
    else if((var->currentMode == NORMAL) & (uptime >= endtime)) {
        container->setDeployState(FORCED);
        container->setDeployDelayTime(uptime + container->getDeployDelayParameter());
    }
//    else if(CurrentState == FORCED) {
//        //command ADB to deploy
//        container.setDeployState(DELAYING);
//
//    }
    else if((var->currentMode == FORCED) & (uptime < container->getDeployDelayTime())) {
        return;
    }
    else if((var->currentMode == FORCED) & (uptime >= container->getDeployDelayTime())) {
        //Command ADB to deploy
        var->currentMode = SAFE;
        container->setDeployState(NORMAL);
    }
    return;
}

void DeployMode (OBCDataContainer *container, variables *var) {
    //Here I save it in a variable to avoid consecutive calls to the same function
    long UpTime = container->getTotalUpTime();
    long EndTime = container->getEndOfDeployState();
    if (CheckDeploy(var) && CheckDeployTelem()) {
        var->currentMode = SAFE;
        //add deployment done to database
        container->setDeployState(DEPLOYED);
        return;
    }
    else
        DeployAntenna(container,var,UpTime,EndTime);
}




