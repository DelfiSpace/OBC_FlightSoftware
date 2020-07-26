/*
 * SafeMode.h
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// Include guard for SAFEMODE_H_
#ifndef SAFEMODE_H_
#define SAFEMODE_H_

#include "OBCTelemetryContainer.h"

/**
 *
 *  Fixed logic of the safe mode
 *
 *  Input:
 *      OBCContainer->getBusVoltage()
 *      OBCContainer->getSMVoltage()
 *  Output:
 *      OBCContainer->setMode()
 *
 */
void SafeMode(OBCTelemetryContainer *OBCContainer);

// End include guard for SAFEMODE_H_
#endif
