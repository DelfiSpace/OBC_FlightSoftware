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
ResetService reset( GPIO_PORT_P4, GPIO_PIN0);
//ResetService reset( GPIO_PORT_P4, GPIO_PIN0, GPIO_PORT_P4, GPIO_PIN2 );
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
    unsigned int SDDetect = MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN4);
    //Console::log("SD Detect: %d", SDDetect);

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
 */void main(void)
 {
    // initialize the MCU:
    // - clock source
    // - clock tree
    DelfiPQcore::initMCU();

//    // initialize the ADC
//    // - ADC14 and FPU Module
//    // - MEM0 for internal temperature measurements
//    ADCManager::initADC();
//
//    // Initialize I2C master
//    I2Cinternal.setFastMode();
//    I2Cinternal.begin();
//
//    // Initialize SPI master
//    spi.initMaster(DSPI::MODE0, DSPI::MSBFirst, 1000000);
//    fram.init();
//
//    // initialize the shunt resistor
//    powerBus.setShuntResistor(40);
//
//    // initialize temperature sensor
//    temp.init();
//
//    // initialize the console
    Console::init( 115200 );                        // baud rate: 9600 bps
//    pq9bus.begin(115200, 1);     // baud rate: 115200 bps
//                                            // address OBC (1)
//
//    //InitBootLoader!
//    bootLoader.JumpSlot();
//
//    // initialize the reset handler:
//    // - prepare the watch-dog
//    // - initialize the pins for the hardware watch-dog
//    // - prepare the pin for power cycling the system
//    reset.init();
//
//    // initialize Task Notifier
//    taskNotifier.init();
//
//    // initialize HWMonitor readings
//    hwMonitor.readResetStatus();
//    hwMonitor.readCSStatus();
//
//    // link the command handler to the PQ9 bus:
//    // every time a new command is received, it will be forwarded to the command handler
//    // TODO: put back the lambda function after bug in CCS has been fixed
//    pq9bus.setReceiveHandler([](DataFrame &newFrame){ cmdHandler.received(newFrame); });
//    //pq9bus.setReceiveHandler(&receivedCommand);
//
//    // every time a command is correctly processed, call the watch-dog
//    // TODO: put back the lambda function after bug in CCS has been fixed
//    cmdHandler.onValidCommand([]{ reset.kickInternalWatchDog(); });
//    //cmdHandler.onValidCommand(&validCmd);

    Console::log("OBC booting...SLOT: %d", (int) Bootloader::getCurrentSlot());

    if(HAS_SW_VERSION == 1){
        Console::log("SW_VERSION: %s", (const char*)xtr(SW_VERSION));
    }

    Console::log("Configure SD-Card Pins");

    //Sd On
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5);
    //sd detect
    //MAP_GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN4);

    //DISABLE TIMER32 IE
    Timer32_disableInterrupt(TIMER32_0_BASE);
    Timer32_disableInterrupt(TIMER32_1_BASE);


    DSPI_A SPISD;
    SPISD.initMaster(200000);

    SDCard sdcard(&SPISD, GPIO_PORT_P2, GPIO_PIN0);
    sdcard.init();

    //CMD10: Read CID!
    uint8_t CID[17] = {0};
    Console::log(" * Sending CMD10: Read CID");
    sdcard.select();
    uint8_t R2_1 = sdcard.sendCmd(10,0);
    uint8_t R2_2;
    sdcard.getArray(&R2_2, 1);
    sdcard.getArray((uint8_t*)CID, 17);
    sdcard.unselect();
    Console::log(" * R2: %x %x", R2_1, R2_2);
    Console::log(" * CID: %x %x %x %x", CID[0], CID[1], CID[2], CID[3]);
    Console::log(" * CID: %x %x %x %x", CID[4], CID[5], CID[6], CID[7]);
    Console::log(" * CID: %x %x %x %x", CID[8], CID[9], CID[10], CID[11]);
    Console::log(" * CID: %x %x %x %x", CID[12], CID[13], CID[14], CID[15]);

    //Data Sheet Table 3-9
    Console::log(" * Manufacturer ID: %x", CID[1]);
    Console::log(" * OEM/Application ID: %x %x", CID[2], CID[3]);
    Console::log(" * Product Name: %c%c%c%c%c", CID[4], CID[5], CID[6], CID[7], CID[8]);
    Console::log(" * Product Revision: %d", CID[9]);
    Console::log(" * Product Serial Number: %d", ( ((uint32_t)CID[10] << 24)|((uint32_t)CID[11] << 16)|((uint32_t)CID[12] << 8)|((uint32_t)CID[13]) ) );
    Console::log(" * Manufacture Date: %x - %x", 0x2000 | ((CID[15]&0xf0) >> 4) | ((CID[14]&0x0f) << 4), (CID[15]&0x0f));
    Console::log("");

    //SD Card BootCounter Test!
    Console::log("Creating FS Object");
    LittleFS fs;
    int err = fs.mount(&sdcard);
    if(err < 0){
        Console::log("Mounting SD Card - Error Code: -%d", -err);
    }else{
        Console::log("Mounting SD Card - Error Code: %d", err);
    }
//    uint8_t coolBuf[512];
//    const char progBuf[5] = {"yolo"};
//    int a = sdcard.read(coolBuf, 512, 512);
//    Console::log("READ -%d", -a);
//    a = sdcard.program(progBuf, 0, 5);
//    Console::log("PROG -%d", -a);
//    a = sdcard.trim(0, 5);
//    Console::log("TRIM -%d", -a);
//    a = sdcard.get_read_size();
//    Console::log("READSize %d", a);
//    a = sdcard.get_program_size();
//    Console::log("PROGSize %d", a);
//    a = sdcard.size();
//    Console::log("Size %d", a);
//    a = sdcard.frequency(400000);
//    Console::log("Freq %d", a);

    err = fs.format();
//    if(err){
//        err = fs.format();
//        if(err < 0){
//            Console::log("Format SD Card - Error Code: -%d", -err);
//        }else{
//            Console::log("Format SD Card - Error Code: %d", err);
//        }
//    }


    TaskManager::start(tasks, 2);
}
