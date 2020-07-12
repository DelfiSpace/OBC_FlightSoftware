/*
 * NominalMode.h
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// TODO:
// [ ]

// Include guard for NOMINALMODE_H_
#ifndef NOMINALMODE_H_
#define NOMINALMODE_H_

#include "Console.h"
#include "StateMachine.h"
#include "Communication.h"
#include "LineControl.h" // Makes line control easier.


// These guys should not be needed, but CCS tells me it is.
#include "EPSTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"


// Declaration of NominalMode()
void NominalMode();


// End include guard for NOMINALMODE_H_
#endif
