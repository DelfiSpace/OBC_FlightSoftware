/*
 * ADCS.h
 *
 *  Created on: 23 Jul 2019
 *      Author: stefanosperett
 */

#ifndef OBC_H_
#define OBC_H_

#include <driverlib.h>
#include "SLOT_SELECT.h"
#include "SoftwareUpdateService.h"
#include "Bootloader.h"
#include "msp.h"
#include "DelfiPQcore.h"
#include "PQ9Bus.h"
#include "PQ9Frame.h"
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
#include "OBCDataContainer.h"
#include "Communication.h"

#define FCLOCK 48000000

#endif /* OBC_H_ */
