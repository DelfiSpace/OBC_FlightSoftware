/*
 * StateMachine.h
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "OBCDataContainer.h"

void StateMachineInit();
void StateMachine();

/*
 * Here the taskmanager class will be specified,
 * this class will inherit from virtual class Task
 * and will therefore inherit functions like execute
 * task and notified. A notified check is therefore not needed
 */
class StateMachine : public Task
{
private:

public:
    //constructor
    StateMachine( const unsigned int count, void (*function)( void ), void (&init)( void ) );
    //actual function that will be run to determine state
    void run();
    States CurrentState;
    void SetUp();

    uint64_t upTime;
    //destructor
    ~StateMachine();
    //value to keep track of how many times this task has run
    //if this is needed is questionable
    uint64_t StateMachineCount = 0;
};

//initiate an instance of Statemachine class
extern StateMachine* StateMachineTask;

#endif /* STATEMACHINE_H_ */
