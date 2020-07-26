/*
 * PowerBusControl.h
 *
 *  Created on: July 11th, 2020
 *      Author: Johan Monster
 *
 *  Generalizes control over EPS power lines.
 */


// Include guard for PowerBusControl_H_
#ifndef POWERBUSCONTROL_H_
#define POWERBUSCONTROL_H_

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
 *  bool test = PowerBusControl(1, 1, 0, 0);
 *
 *  WARNING: Uses that designate the first argument as 0 WILL lead to a reset of OBC itself,
 *  i.e.
 *  bool test = PowerBusControl(0, x, x, x);
 *
 */
bool PowerBusControl(bool Line1, bool Line2, bool Line3, bool Line4);

// End include guard for PowerBusControl_H_
#endif
