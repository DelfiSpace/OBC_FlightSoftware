/*
 * SLOT_SELECT.h
 *
 *  Created on: Feb 27, 2020
 *      Author: casper
 */
#ifndef SLOT_SELECT_H_
#define SLOT_SELECT_H_

#define xtr(s) str(s)
#define str(s) #s

#ifndef SW_VERSION
#define HAS_SW_VERSION 0
#define GET_SW_VERSION xtr(SW_VERSION)
#else
#define HAS_SW_VERSION 1
#define GET_SW_VERSION xtr(SW_VERSION);
#endif

#define SELECTED_SLOT (SLOT1)
#define SLOT0    (0x00000000)
#define SLOT1    (0x00100000)
#define SLOT2    (0x00180000)
#define SLOTSIZE (0x00080000)



#endif /* SLOT_SELECT_H_ */
