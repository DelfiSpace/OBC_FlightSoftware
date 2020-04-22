#include "OBC.h"

// I2C bus
DWire I2Cinternal(0);
INA226 powerBus(I2Cinternal, 0x40);
TMP100 temp(I2Cinternal, 0x48);

// SPI bus
DSPI spi(3);
MB85RS fram(spi, GPIO_PORT_P1, GPIO_PIN0 );

// HardwareMonitor
HWMonitor hwMonitor(&fram);

// Bootloader
Bootloader bootLoader = Bootloader(fram);

// CDHS bus handler
PQ9Bus pq9bus(3, GPIO_PORT_P9, GPIO_PIN0);

// services running in the system
TestService test;
PingService ping;
ResetService reset( GPIO_PORT_P4, GPIO_PIN0 );
HousekeepingService<OBCTelemetryContainer> hk;

#ifndef SW_VERSION
SoftwareUpdateService SWupdate(fram);
#else
SoftwareUpdateService SWupdate(fram, (uint8_t*)xtr(SW_VERSION));
#endif

Service* services[] = { &ping, &reset, &hk, &test, &SWupdate };

// ADCS board tasks
CommandHandler<PQ9Frame> cmdHandler(pq9bus, services, 5);
PeriodicTask timerTask(1000, periodicTask);
PeriodicTask* periodicTasks[] = {&timerTask};
PeriodicTaskNotifier taskNotifier = PeriodicTaskNotifier(periodicTasks, 1);
Task* tasks[] = { &timerTask, &cmdHandler };

volatile bool cmdReceivedFlag = false;
DataFrame* receivedFrame;

// system uptime
unsigned long uptime = 0;

// TODO: remove when bug in CCS has been solved
void receivedCommand(DataFrame &newFrame)
{
    cmdReceivedFlag = true;
    receivedFrame = &newFrame;
    cmdHandler.received(newFrame);
}

void validCmd(void)
{
    reset.kickInternalWatchDog();
}

void pingModules()
{
    //serial.println("Ping COMMS!");

    PQ9Frame pingFrame;

    pingFrame.setSource(1);
    pingFrame.setDestination(4); //ping COMMS
    pingFrame.setPayloadSize(2);
    pingFrame.getPayload()[0] = 17;
    pingFrame.getPayload()[1] = 1;

    pq9bus.transmit(pingFrame);
    //wait for reply
    while(cmdReceivedFlag == false);
    cmdReceivedFlag = false;

//    serial.print("Reply : ");
//    serial.print(receivedFrame->getSource(), DEC);
//    serial.print(" ");
//    serial.print(receivedFrame->getPayload()[0], DEC);
//    serial.print(" ");
//    serial.println(receivedFrame->getPayload()[1], DEC);
//
//
//    serial.println("Ping EPS!");
    pingFrame.setSource(1);
    pingFrame.setDestination(2); //ping EPS
    pingFrame.setPayloadSize(2);
    pingFrame.getPayload()[0] = 17;
    pingFrame.getPayload()[1] = 1;

    pq9bus.transmit(pingFrame);
    //wait for reply
    while(cmdReceivedFlag == false);
    cmdReceivedFlag = false;
//    serial.print("Reply : ");
//    serial.print(receivedFrame->getSource(), DEC);
//    serial.print(" ");
//    serial.print(receivedFrame->getPayload()[0], DEC);
//    serial.print(" ");
//    serial.println(receivedFrame->getPayload()[1], DEC);

}

void retrieveCommCommands(){
    PQ9Frame passFrame;
    passFrame.setSource(1);

    PQ9Frame requestFrame;
    requestFrame.setSource(1);
    requestFrame.setDestination(4); //ping COMMS
    requestFrame.setPayloadSize(2);
    requestFrame.getPayload()[0] = 20;
    requestFrame.getPayload()[1] = 4;

    bool allRetrieved = false;
    while(!allRetrieved){
        pq9bus.transmit(requestFrame);
        while(cmdReceivedFlag == false);
        cmdReceivedFlag = false;
        if(receivedFrame->getPayload()[1] == 0){
            Console::log("COMMS: No more GS commands-");
            allRetrieved = true;
        }else if(receivedFrame->getPayload()[2+2] == 99){
            passFrame.setDestination(receivedFrame->getPayload()[2+0]);
            passFrame.setPayloadSize(receivedFrame->getPayload()[2+1]);
            passFrame.setSource(1);
            for(int p = 0; p < passFrame.getPayloadSize(); p++){
                passFrame.getPayload()[p] = receivedFrame->getPayload()[5+p];
            }



            pq9bus.transmit(passFrame);
            while(cmdReceivedFlag == false);
            cmdReceivedFlag = false;
//            serial.print("  ===> Reply: ");
//            serial.print(receivedFrame->getDestination(), DEC);
//            serial.print(" ");
//            serial.print(receivedFrame->getPayloadSize(), DEC);
//            serial.print(" ");
//            serial.print(receivedFrame->getSource(), DEC);
//            serial.print(" ");
//            for(int k = 0; k < receivedFrame->getPayloadSize(); k++){
//                serial.print(receivedFrame->getPayload()[k], DEC);
//                serial.print(" ");
//            }
//
//            serial.println("");



        }
    }
}

void retrieveCommCommandsReply(){
    PQ9Frame passFrame;
    passFrame.setSource(1);

    PQ9Frame requestFrame;
    requestFrame.setSource(1);
    requestFrame.setDestination(4); //ping COMMS
    requestFrame.setPayloadSize(2);
    requestFrame.getPayload()[0] = 20;
    requestFrame.getPayload()[1] = 4;

    bool allRetrieved = false;
    while(!allRetrieved){
        pq9bus.transmit(requestFrame);
        while(cmdReceivedFlag == false);
        cmdReceivedFlag = false;
        if(receivedFrame->getPayload()[1] == 0){
            Console::log("COMMS: No more GS commands-");
            allRetrieved = true;
        }else if(receivedFrame->getPayload()[2+2] == 99){
            passFrame.setDestination(receivedFrame->getPayload()[2+0]);
            passFrame.setPayloadSize(receivedFrame->getPayload()[2+1]);
            passFrame.setSource(1);
            for(int p = 0; p < passFrame.getPayloadSize(); p++){
                passFrame.getPayload()[p] = receivedFrame->getPayload()[5+p];
            }

//            serial.print("Reply : ");
//                       for(int x = 0; x < passFrame.getPayloadSize(); x++){
//                           serial.print(passFrame.getPayload()[x], DEC);
//                           serial.print(" ");
//                       }
//                       serial.println("");


            pq9bus.transmit(passFrame);
            while(cmdReceivedFlag == false);
            cmdReceivedFlag = false;

            passFrame.setDestination(4);
            passFrame.setSource(1);
            passFrame.setPayloadSize(receivedFrame->getPayloadSize()+6);
            passFrame.getPayload()[0] = 20;
            passFrame.getPayload()[1] = 3;
            passFrame.getPayload()[2] = receivedFrame->getPayloadSize()+3;
            for(int y = 0; y < receivedFrame->getPayloadSize()+3; y++){
                passFrame.getPayload()[3+y] = receivedFrame->getFrame()[y];
            }

            pq9bus.transmit(passFrame);
            while(cmdReceivedFlag == false);
            cmdReceivedFlag = false;

//            serial.print("  ===> Reply: ");
//            serial.print(receivedFrame->getDestination(), DEC);
//            serial.print(" ");
//            serial.print(receivedFrame->getPayloadSize(), DEC);
//            serial.print(" ");
//            serial.print(receivedFrame->getSource(), DEC);
//            serial.print(" ");
//            for(int k = 0; k < receivedFrame->getPayloadSize(); k++){
//                serial.print(receivedFrame->getPayload()[k], DEC);
//                serial.print(" ");
//            }
//
//            serial.println("");



        }
    }
}

void periodicTask()
{
    // increase the timer, this happens every second
    uptime++;

    // collect telemetry
    hk.acquireTelemetry(acquireTelemetry);

    // refresh the watch-dog configuration to make sure that, even in case of internal
    // registers corruption, the watch-dog is capable of recovering from an error
    reset.refreshConfiguration();

    // kick hardware watch-dog after every telemetry collection happens
    reset.kickExternalWatchDog();

    // pingFriends
//    pingModules();
//
//    retrieveCommCommandsReply();

}

void acquireTelemetry(OBCTelemetryContainer *tc)
{
    unsigned short v;
    signed short i, t;


    tc->setUpTime(uptime);

    // measure the power bus
    tc->setBusStatus((!powerBus.getVoltage(v)) & (!powerBus.getCurrent(i)));
    tc->setBusVoltage(v);
    tc->setBusCurrent(i);

    tc->setTmpStatus(!temp.getTemperature(t));


}

/**
 * main.c
 */
void main(void)
{
    // initialize the MCU:
    // - clock source
    // - clock tree
    DelfiPQcore::initMCU();

    // initialize the ADC
    // - ADC14 and FPU Module
    // - MEM0 for internal temperature measurements
    ADCManager::initADC();

    // Initialize I2C master
    I2Cinternal.setFastMode();
    I2Cinternal.begin();

    // Initialize SPI master
    spi.initMaster(DSPI::MODE0, DSPI::MSBFirst, 1000000);
    fram.init();

    // initialize the shunt resistor
    powerBus.setShuntResistor(40);

    // initialize temperature sensor
    temp.init();

    // initialize the console
    Console::init( 115200 );                        // baud rate: 9600 bps
    pq9bus.begin(115200, 1);     // baud rate: 115200 bps
                                            // address OBC (1)

    //InitBootLoader!
    bootLoader.JumpSlot();

    // initialize the reset handler:
    // - prepare the watch-dog
    // - initialize the pins for the hardware watch-dog
    // - prepare the pin for power cycling the system
    reset.init();

    // initialize Task Notifier
    taskNotifier.init();

    // initialize HWMonitor readings
    hwMonitor.readResetStatus();
    hwMonitor.readCSStatus();

    // link the command handler to the PQ9 bus:
    // every time a new command is received, it will be forwarded to the command handler
    // TODO: put back the lambda function after bug in CCS has been fixed
    pq9bus.setReceiveHandler([](DataFrame &newFrame){ cmdHandler.received(newFrame); });
    //pq9bus.setReceiveHandler(&receivedCommand);

    // every time a command is correctly processed, call the watch-dog
    // TODO: put back the lambda function after bug in CCS has been fixed
    cmdHandler.onValidCommand([]{ reset.kickInternalWatchDog(); });
    //cmdHandler.onValidCommand(&validCmd);

    Console::log("OBC booting...SLOT: %d", (int) Bootloader::getCurrentSlot());

    if(HAS_SW_VERSION == 1){
        Console::log("SW_VERSION: %s", (const char*)xtr(SW_VERSION));
    }

    TaskManager::start(tasks, 2);
}
