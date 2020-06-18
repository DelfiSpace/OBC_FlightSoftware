/*
 * ActivationMode.cpp
 *
 *  Created on: May 20, 2020
 *      Author: tom-h
 */

#include "Activationmode.h"

void SetUpFirstBoot() {
    BootCount = 0;
}
void LoadSD() {
    return;
}
//void UpdateBootCount(OBCDatacontainer *c) {
//    c->setBootCount((c->getBootCount())++);
//}

/*
 * Function to check OBC, EPS, Comms
 * and ADB. But I think it will be nice
 * if the things it checks is changable
 *
 * input:
 *          Components to check
 *
 * Output:
 *          for now 1 if correct 0 if not correct
 */
uint8_t HealthCheck() {
    return 42;
}

bool CheckFlag(OBCDataContainer *c) {
#ifdef DEBUG
    c->setMode(DEPLOYMENT);
#else
    if (c.getTimerDone()) {
        c->setMode(DEPLOYMENT);
        return true;
    }
    else
        return false;
#endif
    return true;
}

void TimerDoneFunc(OBCDataContainer *c) {
    uint64_t Remaining = 1800 - c->getUpTime();

    if(Remaining<1) {
        c->setMode(DEPLOYMENT);
        TimerDone = true;
        }
    else
        return;


}
//c is the contianer
void ActivationMode(OBCDataContainer *c) {
    SetUpFirstBoot();
    LoadSD();

    //UpdateBootCount();

    HealthCheck();

    if (CheckFlag(c))
        return;
    else
        TimerDoneFunc(c);
}




