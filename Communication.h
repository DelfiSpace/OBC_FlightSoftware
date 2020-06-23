/*
 *  Communication.h
 *
 *  Created on: 8 June 2020
 *      Author: Zhuoheng Li
 */

#include "TelemetryContainer.h"
#include "DataFrame.h"

#define SERVICE_RESPONSE_ERROR      0
#define SERVICE_RESPONSE_REQUEST    1
#define SERVICE_RESPONSE_REPLY      2
#define SERVICE_NO_RESPONSE         3

// Address number for pq9bus
typedef enum Address {OBC = 1, EPS = 2, ADB = 3, COMMS = 4,
    ADCS = 5, PROP = 6, DEBUG = 7, EGSE = 8, HPI = 100} Address;

/**
 *
 *  Interrupt service routine when OBC gets a reply
 *  It's registered by pq9bus.setReceiveHandler() in main.cpp
 *
 *  Parameters:
 *      DataFrame &newFrame         Reference of the reply
 *  External variables used:
 *      bool cmdReceivedFlag        It indicates whether OBC gets a reply
 *      PQ9Frame *receivedFrame     Address of the reply
 *
 */
void receivedCommand(DataFrame &newFrame);


/**
 *
 *   Send a frame over the bus and get the reply
 *
 *   Parameters:
 *      Address destination             Address of the target board except OBC
 *      unsigned char sentSize          Size of the payload in the sent frame
 *      unsigned char *sentPayload      Payload in the sent frame
 *      unsigned long timeLimitMS       If timeLimitMS expires and OBC doesn't get a reply,
 *                                      return SERVICE_NO_RESPONSE
 *   Returns:
 *      RequestReply()                  SERVICE_RESPONSE_REPLY or
 *                                      SERVICE_RESPONSE_ERROR or
 *                                      SERVICE_NO_RESPONSE
 *      unsigned char *receivedSize     Size of the payload in the received frame
 *      unsigned char **receivedPayload Address for the pointer for the received payload.
 *                                      RequestReply() will change this pointer, so it points to the payload.
 *
 */
int RequestReply(Address destination, unsigned char sentSize, unsigned char *sentPayload,
                 unsigned char *receivedSize, unsigned char **receivedPayload,
                 unsigned long timeLimitMS);

/**
 *
 *   Ping a module
 *
 *   Parameters:
 *      Address destination             Address of the target board except OBC
 *   Returns:
 *      PingModule()                    SERVICE_RESPONSE_REPLY or
 *                                      SERVICE_RESPONSE_ERROR or
 *                                      SERVICE_NO_RESPONSE
 *
 */
int PingModule(Address destination);

/**
 *
 *  Request telemetry from a specific module
 *
 *   Parameters:
 *      Address destination             Address of the target board except OBC
 *   Returns:
 *      RequestTelemetry                SERVICE_RESPONSE_REPLY or
 *                                      SERVICE_RESPONSE_ERROR or
 *                                      SERVICE_NO_RESPONSE
 *      TelemetryContainer *container   The retrieved telemetry will be copied to the container
 *
 */
int RequestTelemetry(Address destination, TelemetryContainer *container);

