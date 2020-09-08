/*
 *  Communication.h
 *
 *  Created on: 8 June 2020
 *      Author: Zhuoheng Li
 */
#ifndef BUSMASTER_H_
#define BUSMASTER_H_

#include "TelemetryContainer.h"
#include "DataFrame.h"
#include "DataBus.h"
#include "Service.h"
#include "OBC.h"

// Address number for pq9bus
typedef enum Address {OBC = 1, EPS = 2, ADB = 3, COMMS = 4,
    ADCS = 5, PROP = 6, DEBUG = 7, EGSE = 8, HPI = 100} Address;

typedef enum MsgType {Request = 0x01, Reply = 0x02 } MsgType;

typedef enum ServiceNumber {DeployService = 99, PowerBus = 0x01, Housekeeping = 3, Radio = 20} ServiceNumber;

template <class Frame_Type, class Message_Type>
class BusMaster{
    public:
        volatile bool cmdReceivedFlag;
        Frame_Type rxFrame, txFrame;
        Message_Type rxMsg, txMsg;
        DataBus* pq9bus;

        BusMaster(DataBus &bus){
            pq9bus = &bus;
            cmdReceivedFlag = false;
            rxMsg.setPointer(rxFrame.getPayload());
            txMsg.setPointer(txFrame.getPayload());
        };


        /**
         *
         *  Interrupt service routine when OBC gets a reply
         *  It's registered by pq9bus.setReceiveHandler() in main.cpp
         *
         *  Parameters:
         *      DataFrame &newFrame         Reference of the reply
         *  External variables used:
         *      bool cmdReceivedFlag        It indicates whether OBC gets a reply
         *      PQ9Frame *rxFrame     Address of the reply
         *
         */
        bool received(DataFrame &newFrame )
        {
            Frame_Type tmpFrame;
            rxFrame.copy(tmpFrame);
            newFrame.copy(rxFrame);
            if(rxMsg.getMessageType() == MsgType::Reply && rxFrame.getSource() == txFrame.getDestination()){
                cmdReceivedFlag = true;
//                Console::log("Reply!");
                return true;
            }else{
                tmpFrame.copy(rxFrame);
                return false;
            }
        };

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
        Message_Type* RequestReply(unsigned char destination, unsigned char dataPayloadSize, unsigned char *dataPayload,
                          unsigned char Service, unsigned char Message_type, unsigned long timeLimitMS)
        {
            // Prepare TX Frame
            txFrame.setDestination(destination);
            txFrame.setSource(Address::OBC);
            txMsg.setMessageType(Message_type);
            txMsg.setService(Service);
            txMsg.setPayloadSize(dataPayloadSize);
            for(int p=0; p < dataPayloadSize; p++){
                txMsg.getDataPayload()[p] = dataPayload[p];
            }
            txFrame.setPayloadSize(txMsg.getSize());
//            Console::log("TX SIZE: %d", txFrame.getPayloadSize());
            // Send the frame
            cmdReceivedFlag = false;
            pq9bus->transmit(txFrame);

            // Wait until timeLimitMS passes or the reply arrives
            unsigned long count = FCLOCK/(1000L/timeLimitMS); // The timer uses FCLOCK
            MAP_Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
            MAP_Timer32_setCount(TIMER32_1_BASE, count);
            MAP_Timer32_startTimer(TIMER32_1_BASE, true);
            while ((MAP_Timer32_getValue(TIMER32_1_BASE) > 0) && (cmdReceivedFlag == false)){
            };

            if(cmdReceivedFlag && rxFrame.getSource() == txFrame.getDestination()){
                rxMsg.setSize(rxFrame.getPayloadSize());
                return &rxMsg;
            }
            return 0;
        };

    private:

};

#endif /* COMMUNICATION_H_ */
