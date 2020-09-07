/*
 * CommandHandler.h
 *
 *  Created on: 15 Jan 2020
 *      Author: CasperBroekhuizen
 */

#ifndef INTERNALCOMMANDHANDLER_H_
#define INTERNALCOMMANDHANDLER_H_

#include "Service.h"
#include "Task.h"
#include "Service.h"
#include "Console.h"

#define ERROR_UNKNOWN_SERVICE       1


template <class Frame_Type, class Message_Type>
class InternalCommandHandler
{
 protected:
     Service** services;
     int servicesCount;
     Frame_Type rxBuffer, txBuffer;
     Message_Type rxMsg, txMsg;
     void (*onValidCmd)( void );

 public:
     InternalCommandHandler(Service **servArray, int count) :
          services(servArray), servicesCount(count)
      {
          onValidCmd = 0;
          rxMsg.setPointer(rxBuffer.getPayload());
          txMsg.setPointer(txBuffer.getPayload());
      };

     void init(Service **servArray, int count)
     {
         services = servArray;
         servicesCount = count;
         onValidCmd = 0;
         rxMsg.setPointer(rxBuffer.getPayload());
         txMsg.setPointer(txBuffer.getPayload());
     };

     void received( DataFrame &newFrame )
     {
         newFrame.copy(rxBuffer);
     };

     void onValidCommand(void (*function)( void ))
     {
         onValidCmd = function;
     };

     Frame_Type* getTxBuffer(){
         return &txBuffer;
     }

     Message_Type* getTxMsg(){
          return &txMsg;
      }

     void run()
      {
          //Prepare Frame to be send back:
          txBuffer.setDestination(rxBuffer.getSource());
          txBuffer.setSource(rxBuffer.getDestination());
          rxMsg.setSize(rxBuffer.getPayloadSize());

//          Console::log("msgType: %d, size: %d", rxMsg.getMessageType(), rxBuffer.getPayloadSize());

          if (rxBuffer.getPayloadSize() > 1 && rxMsg.getMessageType() == SERVICE_RESPONSE_REQUEST)
          {
              bool found = false;

              for (int i = 0; i < servicesCount; i++)
              {
                  if (services[i]->process(rxMsg, txMsg)) // Does any of the Services Handle this command?
                  {
                      // stop the loop if a service is found
                      txBuffer.setPayloadSize(txMsg.getSize());
                      txBuffer.PrepareTransmit();
                      services[i]->postFunc();
                      found = true;
                      break;
                  }
              }

              if (!found)
              {
                  Console::log("Unknown Service (%d)", (int) rxBuffer.getPayload()[0]);

                  txBuffer.setPayloadSize(2);
                  txBuffer.getPayload()[0] = 0;
                  txBuffer.getPayload()[1] = 0;
                  txBuffer.PrepareTransmit();
                  return;
              }
              else
              {
                  if (onValidCmd)
                  {
                      onValidCmd();
                  }
                  return;
              }
          }
          else
          {
              // invalid payload size
              // what should we do here?
              Console::log("Invalid Command, size must be > 1");
              txMsg.setService(0);
              txMsg.setMessageType(SERVICE_RESPONSE_REPLY);
              txMsg.setPayloadSize(1);
              txMsg.getDataPayload()[0] = ERROR_UNKNOWN_SERVICE;
              txBuffer.PrepareTransmit();
              return;
          }
      };
};

#endif /* COMMANDHANDLER_H_ */
