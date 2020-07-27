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
#define OBCFRAM_ADCSTM_ADDR     5300
#define OBCFRAM_COMMSTM_ADDR    5600
#define OBCFRAM_EPSTM_ADDR      5900
#define OBCFRAM_PROPTM_ADDR     6200
#define OBCFRAM_VARIABLES_ADDR  6500

/**
 *
 *  Read an array from FRAM.
 *  It's safer than directly using read() in MB85RS
 *
 *  Parameter:
 *      MB85RS &fram                    The FRAM object
 *      unsigned long startAddress      Staring address of a block.
 *                                      It should be one of macros define above.
 *      int arraySize                   It's used to compared with the actual size of the
 *                                      array in FRAM.
 *
 *  Returns:
 *      OBCFramRead()                   FRAM_NOT_AVAILABLE or
 *                                      FRAM_OPERATION_SUCCESS or
 *                                      FRAM_NOT_WRITTEN or
 *                                      FRAM_WRONG_SIZE
 *      unsigned char *array            The array from FRAM will be saved here
 *
 */
int OBCFramRead(MB85RS &fram, unsigned long startAddress, unsigned char *array, int arraySize);

/**
 *
 *  Write an array to FRAM.
 *  It's safer than directly using write() in MB85RS
 *
 *  Parameter:
 *      MB85RS &fram                    The FRAM object
 *      unsigned long startAddress      Staring address of a block.
 *                                      It should be one of macros define above.
 *      unsigned char *array            The array
 *      int arraySize                   The size of the array
 *
 *  Returns:
 *      OBCFramWrite()                  FRAM_NOT_AVAILABLE or
 *                                      FRAM_OPERATION_SUCCESS or
 *                                      FRAM_NOT_WRITTEN or
 *                                      FRAM_WRONG_SIZE
 *
 */
int OBCFramWrite(MB85RS &fram, unsigned long startAddress, unsigned char *array, int arraySize);

#endif /* OBCFRAMACCESS_H_ */
