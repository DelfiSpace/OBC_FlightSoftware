/*
 * TestService.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: stefanosperett
 */

#include "TestService.h"

extern DSerial serial;
extern MB85RS fram;

bool TestService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getPayload()[0] == 0)
    {
        serial.println("TestService");

        if (command.getPayload()[1] == 0)
        {
            serial.print("Ping: ");
            serial.print(fram.ping(), DEC);
            serial.println();

            unsigned long id = fram.getID();
                        serial.print("ID: ");
                        serial.print(id, HEX);
                        serial.println();

        } else if (command.getPayload()[1] == 1)
        {
            serial.println("Write");
            serial.print("Address: ");
            serial.print(command.getPayload()[2], DEC);
            serial.println();
            serial.print("Value: ");
            serial.print(command.getPayload()[3], DEC);
            serial.println();
            fram.write(command.getPayload()[2], &command.getPayload()[3], 1);

        } else if (command.getPayload()[1] == 2)
        {
            unsigned char v;
            fram.read(command.getPayload()[2], &v, 1);
            serial.println("Read");
            serial.print("Address: ");
            serial.print(command.getPayload()[2], DEC);
            serial.println();
            serial.print("Value: ");
            serial.print(v, DEC);
            serial.println();
        } else if (command.getPayload()[1] == 3)
        {
            serial.println("Erase all");
            fram.erase();
        }

        return true;
    }
    return false;
}
