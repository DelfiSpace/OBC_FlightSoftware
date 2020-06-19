#include "OBC.h"
#include <stdio.h>

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

volatile bool recordingEnabled = false;
RecordingService record(&recordingEnabled);

Service* services[] = { &record, &ping, &reset, &hk, &test, &SWupdate };

// ADCS board tasks
CommandHandler<PQ9Frame> cmdHandler(pq9bus, services, 6);
PeriodicTask timerTask(1000, periodicTask);
PeriodicTask* periodicTasks[] = {&timerTask};
PeriodicTaskNotifier taskNotifier = PeriodicTaskNotifier(periodicTasks, 1);
Task* tasks[] = { &timerTask, &cmdHandler };

volatile bool cmdEPSReceivedFlag = false;
DataFrame* receivedFrame;

// system uptime
unsigned long uptime = 0;

// TODO: remove when bug in CCS has been solved
void receivedCommand(DataFrame &newFrame)
{
    if(newFrame.getSource() == 2){ //Catch messages from EPS
        //Console::log("EPS Reply!");
        cmdEPSReceivedFlag = true;
        receivedFrame = &newFrame;
    }else{
        receivedFrame = &newFrame;
        cmdHandler.received(newFrame);
    }
}

void validCmd(void)
{
    reset.kickInternalWatchDog();
}


LittleFS fs;
lfs_file_t file;
lfs_dir_t dir;
uint8_t TelemetryBuffer[125];

void periodicTask()
{
    // increase the timer, this happens every second
    uptime++;

    //rewind back to the beginning of the file with Seek and Save Uptime
    int err = fs.file_open(&file, "uptime", LFS_O_RDWR | LFS_O_CREAT);
    fs.file_seek(&file, 0, 0);
    fs.file_write(&file, &uptime, sizeof(uptime));
    fs.file_close(&file);


    // collect telemetry
    hk.acquireTelemetry(acquireTelemetry);

    // refresh the watch-dog configuration to make sure that, even in case of internal
    // registers corruption, the watch-dog is capable of recovering from an error
    reset.refreshConfiguration();

    // kick hardware watch-dog after every telemetry collection happens
    reset.kickExternalWatchDog();
    reset.kickInternalWatchDog(); // To avoid system reset.

    // get EPS Housekeeping
    if(recordingEnabled){
        int telemetrySize = getEPSTelemetry(TelemetryBuffer);
        char namebuf[50];
        int got_len = snprintf(namebuf, sizeof(namebuf), "EPS/TELEMETRY_%d", uptime);
        Console::log("Creating File: %s with Telemetry Size: %d", namebuf, telemetrySize);

        int error = fs.dir_open(&dir, "EPS");

        if(error == -2){ //dir does not exist
            Console::log("Creating EPS directory...");
            fs.mkdir("EPS");
            Console::log("Opening EPS directory...");
            error = fs.dir_open(&dir, "EPS");
            fs.dir_close(&dir);
        }else{
            fs.dir_close(&dir);
        }


        if(!error){
            error = fs.file_open(&file, namebuf, LFS_O_RDWR | LFS_O_CREAT);
            if(error){
                Console::log("File open Error: %d", error);
            }else{
                fs.file_write(&file, &TelemetryBuffer, telemetrySize);
                fs.file_close(&file);
            }
        }else{
            Console::log("Folder open Error: -%d", -error);
        }
    }
}

//get EPSTelemetry, copy to Buf and return size
int getEPSTelemetry(uint8_t buf[]){
    PQ9Frame requestFrame;

    requestFrame.setDestination(2); //Destination: EPS
    requestFrame.setSource(1); // OBC
    requestFrame.getPayload()[0] = HOUSEKEEPING_SERVICE; //target HouseKeeping Service
    requestFrame.getPayload()[1] = SERVICE_RESPONSE_REQUEST;
    requestFrame.setPayloadSize(2);
    pq9bus.transmit(requestFrame);

    while(!cmdEPSReceivedFlag);
    cmdEPSReceivedFlag = false;

    //get Housekeeping size
    int EPSHouseKeepingSize = receivedFrame->getPayloadSize() - 2; //payloadsize - (ServiceNumber + Reply Byte)

    //copy Housekeeping to buffer
    for(int i = 0; i < EPSHouseKeepingSize;i++){
        buf[i] = receivedFrame->getPayload()[2+i];
    }

    //notify back with size
    return EPSHouseKeepingSize;
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

DSPI_A SPISD;
SDCard sdcard(&SPISD, GPIO_PORT_P2, GPIO_PIN0);

/**
 * main.c
 */void main(void)
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
    //pq9bus.setReceiveHandler([](DataFrame &newFrame){ cmdHandler.received(newFrame); });
    pq9bus.setReceiveHandler(&receivedCommand);

    // every time a command is correctly processed, call the watch-dog
    // TODO: put back the lambda function after bug in CCS has been fixed
    // cmdHandler.onValidCommand([]{ reset.kickInternalWatchDog(); });
    cmdHandler.onValidCommand(&validCmd);

    Console::log("OBC booting...SLOT: %d", (int) Bootloader::getCurrentSlot());

    if(HAS_SW_VERSION == 1){
        Console::log("SW_VERSION: %s  -  EPS Recorder", (const char*)xtr(SW_VERSION));
    }

    Console::log("Configure SD-Card Pins");

    //Sd On
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5);
    //sd detect
    MAP_GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN4);

    SPISD.initMaster(200000);
    int err = sdcard.init();
    Console::log("SDCard Init: %d",err);

    //SD Card BootCounter Test!

    // variables used by the filesystem
//    LittleFS fs;
//    lfs_file_t file;

    // mount the filesystem
    err = fs.mount(&sdcard);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        fs.format(&sdcard);
        err = fs.mount(&sdcard);
        Console::log("SDCard formatted.");
    }

    if(!err) {
        // read current uptime
        err = fs.file_open(&file, "uptime", LFS_O_RDWR | LFS_O_CREAT);
        if(err){
            Console::log("File Read Error: -%d",-err);
        }else{
            fs.file_read(&file, &uptime, sizeof(uptime));
            // remember the storage is not updated until the file is closed successfully
            fs.file_close(&file);
            // print the boot count
            Console::log("Uptime in Memory: %d\n", uptime);
        }
    }
    //Console::log("DID THIS WORK?!");
    TaskManager::start(tasks, 2);
}
