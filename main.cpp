#include "OBC.h"

// I2C bus
DWire I2Cinternal(0);
INA226 powerBus(I2Cinternal, 0x40);
TMP100 temp(I2Cinternal, 0x48);

// SPI bus
DSPI spi(3);
DSPI_A SPISD;
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
ResetService reset( GPIO_PORT_P4, GPIO_PIN0, GPIO_PORT_P4, GPIO_PIN2 );
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

char CRC7Table[256] = {
    0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F,
    0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77,
    0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26,
    0x51, 0x58, 0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E,
    0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D,
    0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45,
    0x2B, 0x22, 0x39, 0x30, 0x0F, 0x06, 0x1D, 0x14,
    0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C,
    0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B,
    0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01, 0x1A, 0x13,
    0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42,
    0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A,
    0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69,
    0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21,
    0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70,
    0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38,
    0x41, 0x48, 0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E,
    0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36,
    0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67,
    0x10, 0x19, 0x02, 0x0B, 0x34, 0x3D, 0x26, 0x2F,
    0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C,
    0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04,
    0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47, 0x5C, 0x55,
    0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D,
    0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A,
    0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52,
    0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03,
    0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B,
    0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28,
    0x5F, 0x56, 0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60,
    0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31,
    0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79
};

char appendCRC7(char CRC, char message_byte)
{
    return CRC7Table[(CRC << 1) ^ message_byte];
}

char getCRC7(char message[], int length)
{
  int i;
  char CRC = 0;

  for (i = 0; i < length; ++i)
    CRC = appendCRC7(CRC, message[i]);

  return CRC;
}


uint8_t SDsendCmd(uint8_t cmdNumber, uint32_t payload){

    char CMD[6] = {0x40 + cmdNumber, (uint8_t)(payload >> 24), (uint8_t)(payload >> 16), (uint8_t)(payload >> 8), (uint8_t)(payload), (uint8_t) 0};
    CMD[5] = (getCRC7(CMD, 5) << 1) | 1;
    Console::log(" #CMD: %x %x %x %x %x %x", CMD[0],CMD[1],CMD[2],CMD[3],CMD[4],CMD[5]);

    for(int j = 0; j < 6; j++){
        SPISD.transfer(CMD[j]);
    }

    uint8_t R1;
    do {
        R1 = SPISD.transfer(0xff);
    } while ((R1 & 0x80) != 0);

    return R1;
}

void SDwaitReady(){
    uint8_t reply;
    do {
        reply = SPISD.transfer(0xff);
    } while ((reply) != 0xff);
}

void SDgetArray(uint8_t Buff[], int size){
    for(int k = 0; k < size; k++){
        Buff[k] = SPISD.transfer(0xff);
    }
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

    Console::log("Configure SD-Card Pins");

    //chip select
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    //Sd On
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5);
    //sd detect
    MAP_GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN4);

    SPISD.initMaster(200000);

    Console::log("Dummy Clock >74 cycles");
    for(int k = 0; k < 20;  k++)
    {
//        while (!(MAP_SPI_getInterruptStatus(EUSCI_A1_SPI_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT )));
//        MAP_SPI_transmitData(EUSCI_A1_SPI_BASE, 0xff);//Write FF for HIGH DI (10 times = 80 cycles)
//        while (!(MAP_SPI_getInterruptStatus(EUSCI_A1_SPI_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT )));
        SPISD.transfer(0xff);
    }

    char R1 = 0;
    char OCR[4] = {0};
    Console::log("");
    Console::log("=== SD Initialization Routine! ===");
    //Routine based on : http://elm-chan.org/docs/mmc/mmc_e.html
    // Specifically: http://elm-chan.org/docs/mmc/i/sdinit.png


    //CMD0: Go to idle!
    Console::log(" * Sending CMD0: Go to Idle");
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    SDwaitReady();
    R1 = SDsendCmd(0,0);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    Console::log(" * R1: %x", R1);
    Console::log("");

    //CMD8: Check voltage range!
    Console::log(" * Sending CMD8: Check voltage range");
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    SDwaitReady();
    R1 = SDsendCmd(8,0x1AA);
    SDgetArray((uint8_t*)OCR, 4);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    Console::log(" * R1: %x", R1);
    Console::log(" * - R7: %x %x %x %x", OCR[0], OCR[1], OCR[2], OCR[3]);
    Console::log("");

    //ACMD41: Initiate initialization process!
    Console::log(" * Sending ACMD41: Initiate initialization process");
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    do {
    SDwaitReady();
    R1 = SDsendCmd(55,0);
    Console::log(" * R1: %x", R1);
    SDwaitReady();
    R1 = SDsendCmd(41,0x40000000);
    Console::log(" * R1: %x", R1);
    } while (R1 != 0x00);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    Console::log("");

    //CMD58: Read OCR!
    Console::log(" * Sending CMD58: Read OCR");
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    SDwaitReady();
    R1 = SDsendCmd(58,0);
    SDgetArray((uint8_t*)OCR, 4);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    Console::log(" * R1: %x", R1);
    Console::log(" * OCR: %x %x %x %x", OCR[0], OCR[1], OCR[2], OCR[3]);
    Console::log("");

    //CMD10: Read CID!
    uint8_t CID[17] = {0};
    Console::log(" * Sending CMD10: Read CID");
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    SDwaitReady();
    uint8_t R2_1 = SDsendCmd(10,0);
    uint8_t R2_2;
    SDgetArray(&R2_2, 1);
    SDgetArray((uint8_t*)CID, 17);
    MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
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

    TaskManager::start(tasks, 2);
}
