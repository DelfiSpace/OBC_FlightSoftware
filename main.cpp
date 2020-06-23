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

volatile bool recordingEnabled = true;
RecordingService record(&recordingEnabled);

Service* services[] = { &record, &ping, &reset, &hk, &test, &SWupdate };

// OBC board tasks
DSPI_A SPISD;
SDCard sdcard(&SPISD, GPIO_PORT_P2, GPIO_PIN0);
LittleFS fs;

CommandHandler<PQ9Frame> cmdHandler(pq9bus, services, 6);
PeriodicTask timerTask(100, periodicTask);
PeriodicTask* periodicTasks[] = {&timerTask};
PeriodicTaskNotifier taskNotifier = PeriodicTaskNotifier(periodicTasks, 1);
Task* tasks[] = { &fs, &timerTask, &cmdHandler };

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


uint8_t TelemetryBuffer[125];

void periodicTask()
{
    if(fs._mounted){
        int err = fs.file_open(&fs.workfile, "uptime", LFS_O_RDWR | LFS_O_CREAT);
        if(!err){
           fs.file_read(&fs.workfile, &uptime, sizeof(uptime));
            // increase the timer, this happens every second
            uptime++;
            fs.file_seek(&fs.workfile, 0, 0);
            fs.file_write(&fs.workfile, &uptime, sizeof(uptime));
            fs.file_close(&fs.workfile);
        }
    }

    //check if FS is corrupt, if so, format
    if(!fs._mounted && fs._err == -84){
        int err = fs.format(&sdcard);
        Console::log("SD Formatted: -%d", err);
        err = fs.mount_async(&sdcard);
        Console::log("SD Mounted: -%d", err);
    }


    // collect telemetry
    hk.acquireTelemetry(acquireTelemetry);



    // refresh the watch-dog configuration to make sure that, even in case of internal
    // registers corruption, the watch-dog is capable of recovering from an error
    reset.refreshConfiguration();

//    char namebuf[50];
//    int got_len = snprintf(namebuf, sizeof(namebuf), "OBC/TELEMETRY_%d", uptime);
//    Console::log("Creating File: %s with Telemetry Size: %d", namebuf, hk.getTelemetry()->size());
//
//        int error = fs.file_open(&file, namebuf, LFS_O_RDWR | LFS_O_CREAT);
//
//        if(error){
//            fs.mkdir("OBC");
//            error = fs.file_open(&file, namebuf, LFS_O_RDWR | LFS_O_CREAT);
//        }
//
//        if(error){
//            Console::log("File open Error: -%d", -error);
//            fs.file_close(&file);
//        }else{
//            fs.file_write(&file, hk.getTelemetry()->getArray(), hk.getTelemetry()->size());
//            fs.file_close(&file);
//        }
    // kick hardware watch-dog after every telemetry collection happens
    reset.kickExternalWatchDog();
    reset.kickInternalWatchDog(); // To avoid system reset.

    // get EPS Housekeeping
    if(recordingEnabled && fs._mounted){
        int telemetrySize = getEPSTelemetry(TelemetryBuffer);
        char namebuf[64];
        char folderbuf[64];
        int got_len = snprintf(namebuf, sizeof(namebuf), "LOG/%d/%d/EPS_%d", uptime/1000,uptime/100, uptime);
        Console::log("Creating File: %s with Telemetry Size: %d", namebuf, telemetrySize);

        int error = fs.file_open(&fs.workfile, namebuf, LFS_O_RDWR | LFS_O_CREAT);

        if(error){
            snprintf(folderbuf, sizeof(folderbuf), "LOG/%d", uptime/1000);
            Console::log("Creating Folder: %s",folderbuf);
            fs.mkdir("LOG");
            fs.mkdir(folderbuf);
            snprintf(folderbuf, sizeof(folderbuf), "LOG/%d/%d",  uptime/1000,uptime/100);
            fs.mkdir(folderbuf);
            Console::log("Creating Folder: %s",folderbuf);
            error = fs.file_open(&fs.workfile, namebuf, LFS_O_RDWR | LFS_O_CREAT);
        }

        if(error){
            Console::log("File open Error: -%d", -error);
            fs.file_close(&fs.workfile);
        }else{
//            Console::log("Write");
            fs.file_write(&fs.workfile, &TelemetryBuffer, telemetrySize);
//            Console::log("Close");
            fs.file_close(&fs.workfile);
        }
    }else if (!fs._mounted){
        pingEPS();
    }
}

void pingEPS(){
    //Console::log("Ping EPS!");
    PQ9Frame requestFrame;

    requestFrame.setDestination(2); //Destination: EPS
    requestFrame.setSource(1); // OBC
    requestFrame.getPayload()[0] = PING_SERVICE; //target HouseKeeping Service
    requestFrame.getPayload()[1] = SERVICE_RESPONSE_REQUEST;
    requestFrame.setPayloadSize(2);
    pq9bus.transmit(requestFrame);
}

//get EPSTelemetry, copy to Buf and return size
int getEPSTelemetry(uint8_t buf[]){
    PQ9Frame requestFrame;

    requestFrame.setDestination(2); //Destination: EPS
    requestFrame.setSource(1); // OBC
    requestFrame.getPayload()[0] = HOUSEKEEPING_SERVICE; //target HouseKeeping Service
    requestFrame.getPayload()[1] = SERVICE_RESPONSE_REQUEST;
    requestFrame.setPayloadSize(2);

    cmdEPSReceivedFlag = false;
    pq9bus.transmit(requestFrame);
    while(!cmdEPSReceivedFlag);

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

    int err = sdcard.init();
    Console::log("SDCard Init: -%d",-err);
    Console::log("Mounting SD...");
    //SD Card BootCounter Test!

    // variables used by the filesystem
//    LittleFS fs;
//    lfs_file_t file;
    // mount the filesystem

    err = fs.mount_async(&sdcard);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
//    if (err) {
//        fs.format(&sdcard);
//        err = fs.mount(&sdcard);
//        Console::log("SDCard formatted.");
//    }

    if(fs._mounted) {
        // read current uptime
        err = fs.file_open(&fs.workfile, "uptime", LFS_O_RDWR | LFS_O_CREAT);
        if(err){
            Console::log("File open Error: -%d",-err);
            fs.file_close(&fs.workfile);
        }else{
            fs.file_read(&fs.workfile, &uptime, sizeof(uptime));
            // remember the storage is not updated until the file is closed successfully
            fs.file_close(&fs.workfile);
            // print the boot count
            Console::log("Uptime in Memory: %d\n", uptime);
        }
    }
    //Console::log("DID THIS WORK?!");
    TaskManager::start(tasks, 3);
}
