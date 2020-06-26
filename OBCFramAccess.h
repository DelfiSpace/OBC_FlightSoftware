/*
 * OBCFRAMAccess.h
 *
 *  3 problems are identified:
 *      1. The program doesn't know whether the FRAM is written before
 *      2. If we update onboard software, the size of telemetry / container may be changed
 *      3. We need to make sure that both FRAM and SD card is erased before the launch
 *  OBCFRAMAccess provides a simple solution to problem 1 and 2.
 *      - It checks 4 bytes to see whether the FRAM is written (which means the error
 *      probability is 1/2^32).
 *      - It gives 200 bytes for each containers, which is enough for expansion.
 *  However, problem 3 remains unresolved (TODO)!
 *
 *  Created on: June 26, 2020
 *      Author: Zhuoheng
 *
 */

#ifndef OBCFRAMACCESS_H_
#define OBCFRAMACCESS_H_

#include "MB85RS.h"

#define FRAM_NOT_AVAILABLE      0
#define FRAM_OPERATION_SUCCESS  1
#define FRAM_NOT_WRITTEN        2
#define FRAM_WRONG_SIZE         3

#define OBCFRAM_ADBTM_ADDR      5000
#define OBCFRAM_ADCSTM_ADDR     5200
#define OBCFRAM_COMMSTM_ADDR    5400
#define OBCFRAM_EPSTM_ADDR      5600
#define OBCFRAM_PROPTM_ADDR     5800
#define OBCFRAM_VARIABLES_ADDR  6000

int OBCFramRead(MB85RS &fram, unsigned long startAddress, unsigned char *array, unsigned long arraySize);
int OBCFramWrite(MB85RS &fram, unsigned long startAddress, unsigned char *array, unsigned long arraySize);

#endif /* OBCFRAMACCESS_H_ */
