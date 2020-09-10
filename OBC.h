/*
 * OBC.h
 *
 *  Created on: 23 Jul 2019
 *      Author: stefanosperett
 */

#ifndef OBC_H_
#define OBC_H_

#include <driverlib.h>
#include <OBCTelemetryContainer.h>
#include "SLOT_SELECT.h"
#include "SoftwareUpdateService.h"
#include "Bootloader.h"
#include "msp.h"
#include "DelfiPQcore.h"
#include "PQ9Bus.h"
#include "PQ9Frame.h"
#include "PQ9Message.h"
#include "DWire.h"
#include "INA226.h"
#include "Console.h"
#include "CommandHandler.h"
#include "PingService.h"
#include "ResetService.h"
#include "Task.h"
#include "PeriodicTask.h"
#include "TaskManager.h"
#include "HousekeepingService.h"
#include "TMP100.h"
#include "DSPI.h"
#include "MB85RS.h"
#include "PeriodicTaskNotifier.h"
#include "HWMonitor.h"
#include "ADCManager.h"
#include "StateMachine.h"
#include "OBCTelemetryContainer.h"
#include "BusMaster.h"
#include "FRAMService.h"
#include "FRAMVar.h"
#include "FRAMMap.h"
#include "SDCard.h"
#include "LittleFS.h"
#include "DSPI_A.h"
#include "TelemetryRequestService.h"
#include "Telemetry/EPSTelemetryContainer.h"

#define FCLOCK 48000000

// callback functions
void acquireTelemetry(OBCTelemetryContainer *tc);
void periodicTask();

#endif /* OBC_H_ */
