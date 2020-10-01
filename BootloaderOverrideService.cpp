#include "BootloaderOverrideService.h"

BootloaderOverrideService::BootloaderOverrideService()//StateMachine &stateMachine_in)
{
};

bool BootloaderOverrideService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getService() == BOOTLOADEROVERRIDE_SERVICE)
    {
//        Console::log("RadioService: Service started");
        workingBuffer.setService(BOOTLOADEROVERRIDE_SERVICE);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        switch(command.getDataPayload()[0])
        {
        case BOOTLOADEROVERRIDE_OVERRIDE:
            if(command.getPayloadSize() == 2){
                Console::log("BootloaderOverrideService: OVERRIDE SLOT (JUMP)");
                workingBuffer.getDataPayload()[0] = BOOTLOADEROVERRIDE_OVERRIDE;
                workingBuffer.getDataPayload()[1] = 0; //OK
                this->jumpSlot(command.getDataPayload()[1]);
                workingBuffer.setPayloadSize(2);
            }else{
                goto UNKNOWNCOMMAND;
            }
            break;
UNKNOWNCOMMAND:
        default:
            Console::log("BootloaderOverrideService: Unknown command!");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = 1; //UNKNOWN CMD ERROR
            break;
        }

        return true;
    }
    else
    {
        // this command is related to another service,
        // report the command was not processed
        return false;
    }
}

void BootloaderOverrideService::jumpSlot(uint8_t targetSlot){
    Console::log("= Target slot: %d", (int)(targetSlot));

    uint32_t* resetPtr = 0;
    switch(targetSlot) {
        case 0:
            resetPtr = (uint32_t*)(0x00000 + 4);
            break;
        case 1:
            resetPtr = (uint32_t*)(0x100000 + 4);
            break;
        case 2:
            resetPtr = (uint32_t*)(0x180000 + 4);
            break;
        default:
            Console::log("+ BOOTLOADER - Error: target slot not valid!");
    //#if defined (__MSP432P401R__)
    //                        MAP_SysCtl_rebootDevice();
    //#elif defined (__MSP432P4111__)
    //                        MAP_SysCtl_A_rebootDevice();
    //#endif

            break;
        }
        MAP_Interrupt_disableMaster();
        MAP_WDT_A_holdTimer();

        Console::log("Jumping to: 0x%x", (int) *resetPtr);
        Console::log("=============================================");

        void (*slotPtr)(void) = (void (*)())(*resetPtr);

        slotPtr();  //This is the jump!

}
