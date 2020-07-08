/*
 * ActivationMode.cpp
 *
 *  Created on: May 20, 2020
 *      Author: tom-h
 */

#include <ActivationMode.h>

/*
 * As part of the start up routine, the EPS is commanded to
 * shut down all lines except from line 4. If statement
 * with respect to EPS telemetry can be implemented to send fewer
 * messages when already on/off
 *
 */
bool CommandEPS() {
    bool fault = true;
    char execute = 0x01;
    char request = 0x01;
    char V1 = 0x01;
    char V2 = 0x02;
    char V3 = 0x03;
    char V4 = 0x04;
    char stateon = 0x01; //on
    char stateoff = 0x00;

    //char to store received command
    unsigned char* Reply;
    unsigned char ReplySize;
    unsigned char on1[4];

    on1[0] = stateon;
    on1[1] = request;
    on1[2] = V1;
    on1[3] = execute;

    unsigned char* off2 = on1;
    off2[0] = stateoff;
    off2[2] = V2;

    unsigned char* off3 = off2;
    unsigned char* off4 = off2;
    off3[2] = V3;
    off4[2] = V4;

    //Send to EPS
    char Success1 = RequestReply(EPS, 4, on1, &ReplySize, &Reply, 500);
    char Success2 = RequestReply(EPS, 4, off2, &ReplySize, &Reply, 500);
    char Success3 = RequestReply(EPS, 4, off3, &ReplySize, &Reply, 500);
    char Success4 = RequestReply(EPS, 4, off4, &ReplySize, &Reply, 500);

    if (Success1 ==1 && Success2 ==1 && Success3 ==1 && Success4 ==1) {
        fault = false;
    }
    return fault;

}

bool CheckTimer(unsigned long uptime) {
    unsigned long Remaining = 1801 - uptime;

#ifdef STATEMACHINE_DEBUG
    Remaining = 0;
#endif
    if(Remaining<1) {
        return true;
        }
    else
        return false;
}

void ActivationMode(Mode *currentMode, unsigned long totalUptime) {
    //command EPS to turn off all power lines except V1, return true if a fault occurs
    //bool TimerDone = CheckTimer(totalUptime);

    //check if current total uptime is longer than the specified time for deployment
    if (CheckTimer(totalUptime)) {
        *currentMode = DEPLOYMENTMODE;
        return;
    }
    else
        return;
}



