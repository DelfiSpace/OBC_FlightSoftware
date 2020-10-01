#include "Service.h"
#include "Console.h"

#ifndef BOOTLOADEROVERRIDESERVICE_H_
#define BOOTLOADEROVERRIDESERVICE_H_

#define BOOTLOADEROVERRIDE_SERVICE         85
#define BOOTLOADEROVERRIDE_OVERRIDE        5




class BootloaderOverrideService: public Service
{
protected:

public:
    BootloaderOverrideService();//StateMachine &stateMachine_in);

    virtual bool process( DataMessage &command, DataMessage &workingBbuffer );

    void jumpSlot(uint8_t target);
};

#endif
