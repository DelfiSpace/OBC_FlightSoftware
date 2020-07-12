/*
 * SafeMode.h
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// TODO:
// [ ]


// Include guard for SAFEMODE_H_
#ifndef SAFEMODE_H_
#define SAFEMODE_H_

#include "Console.h"
#include "StateMachine.h"
#include "Communication.h"
#include "LineControl.h" // Makes line control easier.


// This guy should not be needed, but CCS tells me it is.
#include "EPSTelemetryContainer.h"


// Declaration of SafeMode()
void SafeMode();

// End include guard for SAFEMODE_H_
#endif
