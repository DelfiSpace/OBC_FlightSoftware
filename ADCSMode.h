/*
 * ADCSMode.h
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// TODO:
// [ ] Include function prototypes for all functions in ADCSMode.cpp
// [ ] Implement testing tools if needed
// [ ] Finalize documentation

// Include guard for ADCSMODE_H_
#ifndef ADCSMODE_H_
#define ADCSMODE_H_

#include "Console.h"
#include "StateMachine.h"
#include "Communication.h"
#include "LineControl.h" // Makes line control easier.

void ADCSMode(const int& upTime);

//bool ADCSMode_checkFlag();
bool ADCS_init(const int& upTime);
bool ADCS_work(const int& upTime);

// End include guard for ADCSMODE_H_
#endif
