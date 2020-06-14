/*
 * DeployMode.cpp
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

#include <DeployMode.h>

bool CheckDeploy() {
    if(DC.getDeployment())
        return true;
    else
        return false;
}

bool CheckDeployTelem() {
    //check telemetry??
    return true;
}

void DeployAntenna() {
//    if “deployment status” == “normal deployment” AND total uptime < “end of deployment status”:
//        if battery voltage > “deploying voltage”:
//            command ADB to deploy the antenna;
//            “deployment status” = “delaying”;
//        “end of deployment status” = total uptime + “delaying parameter”;
//    else if “deployment status” == “normal deployment” AND total uptime >= “end of deployment status”:
//    “deployment status” = “forced deployment”;
//    else if “deployment status” == “forced deployment”:
//        command ADB to deploy the antenna;
//        “deployment status” = “delaying”;
//    “end of deployment status” = total uptime + “delaying parameter”;
//    else if “deployment status” == “delaying” AND total uptime < “end of deployment status”:
//    do nothing;
//    else if “deployment status” == “delaying” AND total uptime >= “end of deployment status”:
//    “deployment status” = “normal deployment”;
//    “end of deployment status” = total uptime + “forced deployment parameter”;
    return;
}
void DeployMode () {
    if (CheckDeploy() && CheckDeployTelem()) {
        DC.setModeFlag(SAFE);
        return;
    }
    else
        DeployAntenna();
}

