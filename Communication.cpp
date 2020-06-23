/*
 *  Communication.cpp
 *
 *  Created on: 8 June 2020
 *      Author: Zhuoheng Li
 */

#define COMMUNICATION_DEBUG

#include "Communication.h"
//TODO: #include "OBCDataContainer.h"
#include "PQ9Frame.h"
#include "PQ9Bus.h"
#include "Console.h"
#include "ResetService.h"
#include "DelfiPQcore.h"

#define MAX_PAYLOAD_SIZE            255
#define PING_SERVICE                17
#define HOUSEKEEPING_SERVICE        3

volatile bool cmdReceivedFlag;
DataFrame *receivedFrame;
extern PQ9Bus pq9bus; // Defined in main.cpp
extern ResetService reset;


/**
 *
 *  Interrupt service routine when OBC gets a reply
 *  It's registered by pq9bus.setReceiveHandler() in main.cpp
 *  Please read Communication.h
 *
 */
void receivedCommand(DataFrame &newFrame)
{
    cmdReceivedFlag = true;
    receivedFrame = &newFrame;
}


/**
 *
 *  Transmit a pq9frame within the time limit
 *
 *  Parameters:
 *      PQ9Frame sentFrame          The frame which is transmitted
 *      unsigned long timeLimitMS   The time limit
 *  External variables used:
 *      bool cmdReceivedFlag        It indicates whether OBC gets a reply
 *      PQ9Frame *receivedFrame     Address of the reply
 *
 */
void TransmitWithTimeLimit(PQ9Frame sentFrame, unsigned long timeLimitMS)
{
    unsigned long count;

    // Send the frame
    cmdReceivedFlag = false;
    pq9bus.transmit(sentFrame);

    // Wait until timeLimitMS passes or the reply arrives
    count = FCLOCK/(1000L/timeLimitMS); // The timer uses FCLOCK
    MAP_Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    MAP_Timer32_setCount(TIMER32_1_BASE, count);
    MAP_Timer32_startTimer(TIMER32_1_BASE, true);
    while ((MAP_Timer32_getValue(TIMER32_1_BASE) > 0) && (cmdReceivedFlag == false));

#ifdef COMMUNICATION_DEBUG
    Console::log("TransmitWithTimeLimit(): response count: %d", count - MAP_Timer32_getValue(TIMER32_1_BASE));
#endif
}


/**
 *
 *  Send a frame over the bus and get the reply
 *  Please read Communication.h
 *
 */
int RequestReply(Address destination, unsigned char sentSize, unsigned char *sentPayload,
                 unsigned char *receivedSize, unsigned char **receivedPayload,
                 unsigned long timeLimitMS)
{
    PQ9Frame sentFrame;
    unsigned char serviceNum;

    if (sentSize > MAX_PAYLOAD_SIZE)
    {
#ifdef COMMUNICATION_DEBUG
        Console::log("SendFrame(): size of payload is too big: %d bytes", sentSize);
#endif
    }

    sentFrame.setSource(OBC);
    sentFrame.setDestination(destination);
    sentFrame.setPayloadSize(sentSize);
    serviceNum = sentPayload[0];

    // Copy payload to sentframe
    for (int i = 0; i < sentSize; i++)
    {
        sentFrame.getPayload()[i] = sentPayload[i];
    }

    // Sent the frame
    TransmitWithTimeLimit(sentFrame, timeLimitMS);

    // Check the reply
    if ((cmdReceivedFlag == true)
        && (receivedFrame->getPayloadSize() > 1)
        && (receivedFrame->getSource() == destination)
        && (receivedFrame->getPayload()[0] == serviceNum)
        && (receivedFrame->getPayload()[1] == SERVICE_RESPONSE_REPLY))
    {
        // Kick internal watchdog (time window: 178s)
        reset.kickInternalWatchDog();

        *receivedPayload = receivedFrame->getPayload();
        *receivedSize = receivedFrame->getPayloadSize();
        return SERVICE_RESPONSE_REPLY;
    }
    else if (cmdReceivedFlag == false)
        return SERVICE_NO_RESPONSE;
    else
        return SERVICE_RESPONSE_ERROR;
}


/**
 *
 *  Ping a specific module
 *  Please read Communication.h
 *
 */
int PingModule(Address destination)
{
    unsigned char sentPayload[2];
    unsigned char receivedSize;
    unsigned char *receivedPayload;

    sentPayload[0] = PING_SERVICE;
    sentPayload[1] = SERVICE_RESPONSE_REQUEST;

    // The time limit is set to 10ms
    return RequestReply(destination, 2, sentPayload, &receivedSize, &receivedPayload, 10);
}


/**
 *
 *  Request telemetry from a specific module
 *  Please read Communication.h
 *
 */
int RequestTelemetry(Address destination, TelemetryContainer *container)
{
    unsigned char sentPayload[2];
    unsigned char receivedSize;
    unsigned char *receivedPayload;
    int ret;

    sentPayload[0] = HOUSEKEEPING_SERVICE;
    sentPayload[1] = SERVICE_RESPONSE_REQUEST;

    // The time limit is set to 100ms
    ret = RequestReply(destination, 2, sentPayload, &receivedSize, &receivedPayload, 100);

    // Copy the received telemetry to the container
    if (ret == SERVICE_RESPONSE_REPLY)
    {
        for (int i = 0; i < receivedSize - 2; i++)
            container->getArray()[i] = receivedPayload[i + 2];
    }

    return ret;
}
