/*
 * TestService.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: stefanosperett
 */

#include "TestService.h"

extern MB85RS fram;

bool TestService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getPayload()[0] == 0)
     {
        Console::log("TestService");

         if (command.getPayload()[1] == 0)
         {

             Console::log("Ping: %d",(int) fram.ping());

             unsigned long id = fram.getID();
             Console::log("ID: %x", id);


         } else if (command.getPayload()[1] == 1)
         {
             Console::log("Write");
             Console::log("Address: %d | Value: %d", command.getPayload()[2], command.getPayload()[3]);
             fram.write(command.getPayload()[2], &command.getPayload()[3], 1);

         } else if (command.getPayload()[1] == 2)
         {
             unsigned char v;
             fram.read(command.getPayload()[2], &v, 1);
             Console::log("Read");
             Console::log("Address: %d | Value: %d", command.getPayload()[2], v);
         } else if (command.getPayload()[1] == 3)
         {
             Console::log("Erase all");
             fram.erase();
         }

         return true;
     }
     return false;
}


