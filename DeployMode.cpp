/*
 * DeployMode.cpp
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

//#include "PowerBusControl.h"

/*
 * Send a command to the antenna to deploy
 * TODO
 */
bool DeployAntenna() {
    bool done = false;
    char execute = 0x01;
    char request = 0x01;

    //char to store received command
    unsigned char* Reply;
    unsigned char ReplySize;
    unsigned char Payload[7];

    //Set data structure according to data structure defined in xml file
    Payload[0] = execute;
    Payload[1] = request;
    Payload[2] = 0x31; //fixed value of 49
    Payload[3] = 0x01; //switch. Todo check this value
    Payload[4] = 0x01; //Feedback on if 1, off if 0
    Payload[5] = 0x11; //todo set this value
    Payload[6] = 0x11; //todo set this value

    //Send to ADB
//    if (RequestReply(ADB, 6, Payload, &ReplySize, &Reply, 500) == SERVICE_RESPONSE_REPLY)
//    {
//        done = true;
//    }
    return done;

}

/**
 *
 *  Please refer to DeployMode.h
 *
 */
//void DeployMode(OBCTelemetryContainer *OBCContainer, ADBTelemetryContainer ADBContainer)
//{
//    //Command EPS to turn off all power lines except V1
//    PowerBusControl(1, 0, 0, 0);
//
//    // Check whether deployment is done. If it's done, switch state
//    switch(OBCContainer->getDeployState())
//    {
//    case DEPLOYED:
//        OBCContainer->setMode(SAFEMODE);
//        return;
//    case PREPARING_UL:
//    case DELAYING_UL:
//        if (1) // TODO: if (CheckUplinkDeployment(ADBContainer))
//        {
//            OBCContainer->setDeployState(DEPLOYED);
//            OBCContainer->setMode(SAFEMODE);
//            return;
//        }
//        break;
//    case PREPARING_DL:
//    case DELAYING_DL:
//        if (1) // TODO: if(CheckDownlinkDeployment(ADBContainer))
//        {
//            OBCContainer->setDeployState(PREPARING_UL);
//        }
//        break;
//    }
//
//    // Take operations according to current state
//    switch(OBCContainer->getDeployState())
//    {
//    case PREPARING_DL:
//        if (OBCContainer->getBusVoltage() > OBCContainer->getDeployVoltage() || OBCContainer->getTotalUpTime() > OBCContainer->getEndOfDeployState())
//        {
//            // DeployAntenna(); // TODO
//            OBCContainer->setDeployState(DELAYING_DL);
//            OBCContainer->setEndOfDeployState(OBCContainer->getTotalUpTime() + OBCContainer->getDelayingDeployPeriod());
//        }
//        break;
//    case DELAYING_DL:
//        if (OBCContainer->getTotalUpTime() > OBCContainer->getEndOfDeployState())
//        {
//            OBCContainer->setDeployState(PREPARING_DL);
//            OBCContainer->setEndOfDeployState(OBCContainer->getTotalUpTime() + OBCContainer->getForcedDeployPeriod());
//        }
//        break;
//    case PREPARING_UL:
//        if (OBCContainer->getBusVoltage() > OBCContainer->getDeployVoltage() || OBCContainer->getTotalUpTime() > OBCContainer->getEndOfDeployState())
//        {
//            // DeployAntenna(); // TODO
//            OBCContainer->setDeployState(DELAYING_UL);
//            OBCContainer->setEndOfDeployState(OBCContainer->getTotalUpTime() + OBCContainer->getDelayingDeployPeriod());
//        }
//        break;
//    case DELAYING_UL:
//        if (OBCContainer->getTotalUpTime() > OBCContainer->getEndOfDeployState())
//        {
//            OBCContainer->setDeployState(PREPARING_UL);
//            OBCContainer->setEndOfDeployState(OBCContainer->getTotalUpTime() + OBCContainer->getForcedDeployPeriod());
//        }
//        break;
//    }
//
//    return;
//}
