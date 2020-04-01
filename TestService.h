/*
 * TestService.h
 *
 *  Created on: 6 Aug 2019
 *      Author: stefanosperett
 */

#ifndef TESTSERVICE_H_
#define TESTSERVICE_H_

#include "Service.h"
#include "Console.h"
#include "MB85RS.h"


class TestService: public Service
{
 public:
     virtual bool process( DataMessage &command, DataMessage &workingBbuffer );
};
#endif /* TESTSERVICE_H_ */
