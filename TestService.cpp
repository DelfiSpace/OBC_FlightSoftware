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
    if (command.getService() == 0)
     {
        Console::log("TestService");

        // prepare response frame
        workingBuffer.setService(0);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        //no payload to the reply, normally you would response with NO_ERROR indicator or any other status preferably.
        workingBuffer.setPayloadSize(0);

        switch(command.getDataPayload()[0]){
        case 0:
            Console::log("Ping: %d",(int) fram.ping());
            unsigned long id = fram.getID();
            Console::log("ID: %x", id);
            break;
        case 1:
            Console::log("Write");
            Console::log("Address: %d | Value: %d", command.getPayload()[2], command.getPayload()[3]);
            fram.write(command.getPayload()[2], &command.getPayload()[3], 1);
            break;
        case 2:
            unsigned char v;
            fram.read(command.getPayload()[2], &v, 1);
            Console::log("Read");
            Console::log("Address: %d | Value: %d", command.getPayload()[2], v);
            break;
        case 3:
            Console::log("Erase all");
            fram.erase();
            break;
        }
        //command processed
        return true;
     }
     return false;
}


