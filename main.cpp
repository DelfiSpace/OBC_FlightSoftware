#include "OBC.h"

// I2C bus
DWire I2Cinternal(0);
INA226 powerBus(I2Cinternal, 0x40);
TMP100 temp(I2Cinternal, 0x48);

// SPI bus
DSPI spi(3);
//MB85RS fram(spi, GPIO_PORT_P1, GPIO_PIN0, MB85RS::CY15B104QN50SXI);
MB85RS fram(spi, GPIO_PORT_P1, GPIO_PIN0, MB85RS::MB85RS1MT);


// HardwareMonitor
HWMonitor hwMonitor(&fram);

// Bootloader
Bootloader bootLoader = Bootloader(fram);

// CDHS bus handler
PQ9Bus pq9bus(3, GPIO_PORT_P9, GPIO_PIN0);
BusMaster<PQ9Frame, PQ9Message> busHandler(pq9bus);

// SD CARD
DSPI_A SPISD;
SDCard sdcard(&SPISD, GPIO_PORT_P2, GPIO_PIN0);
LittleFS fs;

// services running in the system
TelemetryRequestService tlmReqServ(fs, sdcard);
FRAMService framServ(fram);
PingService ping;
ResetService reset( GPIO_PORT_P4, GPIO_PIN0, GPIO_PORT_P4, GPIO_PIN2 );
StateMachineService stateMachineService;

HousekeepingService<OBCTelemetryContainer> hk;
#ifndef SW_VERSION
SoftwareUpdateService SWupdate(fram);
#else
SoftwareUpdateService SWupdate(fram, (uint8_t*)xtr(SW_VERSION));
#endif

Service* services[] = { &ping, &reset, &hk, &SWupdate, &framServ, &tlmReqServ, &stateMachineService, &bootLoaderOverrideServ };


// OBC board tasks
CommandHandler<PQ9Frame, PQ9Message> cmdHandler(pq9bus, services, 8);
InternalCommandHandler<PQ9Frame,PQ9Message> internalCmdHandler(services, 8);
PeriodicTask timerTask(1000, periodicTask);
StateMachine stateMachine(fram, busHandler, internalCmdHandler);
PeriodicTask* periodicTasks[] = {&timerTask, &stateMachine};
PeriodicTaskNotifier taskNotifier = PeriodicTaskNotifier(periodicTasks, 2);
Task* tasks[] = { &timerTask, &stateMachine, &cmdHandler, &fs };

// system uptime
unsigned long uptime = 0;
FRAMBackedVar<unsigned long> totalUptime;

//Telemetry Container Buffers
EPSTelemetryContainer EPSTlmBuffer;

// TODO: remove when bug in CCS has been solved
void receivedCommand(DataFrame &newFrame)
{
    if(!busHandler.received(newFrame)){
        cmdHandler.received(newFrame);
    }
}

void validCmd(void)
{
    reset.kickInternalWatchDog();
}

void periodicTask()
{
    // increase the timer, this happens every second
    uptime += 1;
    totalUptime += 1;

    // collect telemetry
    hk.acquireTelemetry(acquireTelemetry);

    // refresh the watch-dog configuration to make sure that, even in case of internal
    // registers corruption, the watch-dog is capable of recovering from an error
    reset.refreshConfiguration();

    // kick hardware watch-dog after every telemetry collection happens
    reset.kickExternalWatchDog();
    reset.kickInternalWatchDog();

    // pingFriends
//    pingModules();
//
//    retrieveCommCommandsReply();

}

void acquireTelemetry(OBCTelemetryContainer *tc)
{
    unsigned short v;
    signed short i, t;
    unsigned char uc;

    //HouseKeeping Header:
    tc->setStatus(Bootloader::getCurrentSlot() | ((stateMachine.currentState << 4) & 0xF0));
    fram.read(FRAM_RESET_COUNTER + Bootloader::getCurrentSlot(), &uc, 1);
    tc->setBootCounter(uc);
    tc->setResetCause(hwMonitor.getResetStatus());
    tc->setUptime(uptime);
    tc->setTotalUptime((unsigned long) totalUptime);
    tc->setVersionNumber(2);
    tc->setMCUTemp(hwMonitor.getMCUTemp());

    // measure the power bus (INA226)
    tc->setINAStatus((!powerBus.getVoltage(v)) & (!powerBus.getCurrent(i)));
    tc->setVoltage(v);
    tc->setCurrent(i);

    // acquire board temperature (TMP100)
    tc->setTMPStatus(!temp.getTemperature(t));
    tc->setTemperature(t);
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

    //init FRAM and FRAM Variables
    fram.init();
    totalUptime.init(fram, FRAM_TOTAL_UPTIME, true, true);

    // initialize the shunt resistor
    powerBus.setShuntResistor(40);

    // initialize temperature sensor
    temp.init();

    // initialize the console
    Console::init( 115200 );     // baud rate: 9600 bps
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
    //pq9bus.setReceiveHandler([](PQ9Frame &newFrame){ cmdHandler.received(newFrame); });
    pq9bus.setReceiveHandler(&receivedCommand);

    // every time a command is correctly processed, call the watch-dog
    // TODO: put back the lambda function after bug in CCS has been fixed
    //cmdHandler.onValidCommand([]{ reset.kickInternalWatchDog(); });
    //cmdHandler.onValidCommand(&validCmd);

    Console::log("OBC booting...SLOT: %d", (int) Bootloader::getCurrentSlot());

    if(HAS_SW_VERSION == 1){
        Console::log("SW_VERSION: %s", (const char*)xtr(SW_VERSION));
    }
    stateMachine.init();

    //sd detect
    MAP_GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN4);
    if(MAP_GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4) == GPIO_INPUT_PIN_LOW){
        Console::log("SDCard Present");
        //Sd On
        Console::log("Configure SD-Card Pins");
        MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
        MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5);
        int err = sdcard.init();
        if(err){
        Console::log("SDCard Init: -%d",-err);
        }
        Console::log("Mounting SD....");

        fs.mount_async(&sdcard);
    }



    TaskManager::start(tasks, 4);
}
