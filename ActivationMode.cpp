/*
 * ActivationMode.cpp
 *
 *  Created on: May 20, 2020
 *      Author: tom-h
 */

#include "Activationmode.h"


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

bool CheckFlag(OBCDataContainer *c, variables *var) {
    if (c->getTimerDone()) {
        var->currentMode = DEPLOYMENT;
        return true;
    }
    else
        return false;

}

void TimerDoneFunc(OBCDataContainer *c, variables *var) {
    uint64_t Remaining = 1800 - c->getTotalUpTime();

    if(Remaining<1) {
        var->currentMode = DEPLOYMENT;
        c->setTimerDone(true);
    }
    else
        return;


}
//c is the contianer
void ActivationMode(OBCDataContainer *c, variables *var) {
    var->OBCBootCount++;
    LoadSD();

    HealthCheck();

    if (CheckFlag(c, var))
        return;
    else
        TimerDoneFunc(c, var);
}




