/*
 * LineControl.h
 *
 *  Created on: July 11th, 2020
 *      Author: Johan Monster
 *
 *  Generalizes control over EPS power lines.
 */


// Include guard for LINECONTROL_H_
#ifndef LINECONTROL_H_
#define LINECONTROL_H_

#include "Communication.h"

bool LineControl(bool Line1, bool Line2, bool Line3, bool Line4);
/**
 *
 *  Commands EPS to activate/deactivate certain power lines.
 *  Takes 4 boolean values as input, corresponding to power line V1 through V4.
 *  Bool value of 1 activates the line, bool value of 0 deactivates the line.
 *
 *  Outputs 0 when no faults occur.
 *  Outputs 1 when faults occur.
 *
 *  Example:
 *  bool test = LineControl(1, 1, 0, 0);
 *
 *  WARNING: Uses that designate the first argument as 0 WILL lead to a satellite power cycle, i.e.:
 *  bool test = Linecontrol(0, x, x, x);
 *
 */


// End include guard for LINECONTROL_H_
#endif
