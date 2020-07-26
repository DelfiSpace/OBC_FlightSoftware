/*
 * ADCSMode.h
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// Include guard for ADCSMODE_H_
#ifndef ADCSMODE_H_
#define ADCSMODE_H_

#include "ADCSTelemetryContainer.h"
#include "OBCTelemetryContainer.h"

/**
 *
 *  Fixed logic of the ADCS mode
 *
 *  Input:
 *      OBCContainer->getTotalUpTime()
 *      OBCContainer->getADCSState()
 *      OBCContainer->getADCSPowerState()
 *      OBCContainer->getRotateSpeedLimit()
 *      OBCContainer->getDetumblingPeriod()
 *      ADCSContainer
 *  Output:
 *      OBCContainer->setMode()
 *      OBCContainer->setADCSState()
 *      OBCContainer->setEndOfADCSState()
 *      OBCContainer->setADCSPowerState()
 *
 */
void ADCSMode(OBCTelemetryContainer *OBCContainer, ADCSTelemetryContainer ADCSContainer);

// End include guard for ADCSMODE_H_
#endif
