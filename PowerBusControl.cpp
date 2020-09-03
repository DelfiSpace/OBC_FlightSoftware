/*
 * LineControl.cpp
 *
 *  Created on: July 11th, 2020
 *      Author: Johan Monster
 *
 *  Generalizes control over EPS power lines.
 */


bool PowerBusControl(bool Line1, bool Line2, bool Line3, bool Line4) {

    // Define relevant hex values corresponding to EPS command over bus.
    bool fault = true;
    char execute = 0x01;
    char request = 0x01;
    char V1 = 0x01;
    char V2 = 0x02;
    char V3 = 0x03;
    char V4 = 0x04;
    char stateon = 0x01;
    char stateoff = 0x00;

    // char to store received command
    unsigned char* Reply;
    unsigned char ReplySize;

    // Define structure of first payload.
    unsigned char payload1[4];
    payload1[0] = stateon;
    payload1[1] = request;
    payload1[2] = V1;
    payload1[3] = execute;

    // Define other payloads.
    // The default state is [1, 0, 0, 0]
    unsigned char* payload2 = payload1;
    payload2[0] = stateoff;
    payload2[2] = V2;

    unsigned char* payload3 = payload2;
    payload3[2] = V3;

    unsigned char* payload4 = payload2;
    payload4[2] = V4;

    if (Line1 == 0) {payload1[0] = stateoff;}
    if (Line2 == 1) {payload2[0] = stateon;}
    if (Line3 == 1) {payload3[0] = stateon;}
    if (Line4 == 1) {payload4[0] = stateon;}

    //Send to EPS
//    char Success1 = RequestReply(EPS, 4, payload1, &ReplySize, &Reply, 500);
//    char Success2 = RequestReply(EPS, 4, payload2, &ReplySize, &Reply, 500);
//    char Success3 = RequestReply(EPS, 4, payload3, &ReplySize, &Reply, 500);
//    char Success4 = RequestReply(EPS, 4, payload4, &ReplySize, &Reply, 500);

//    if (Success1 ==2 && Success2 ==2 && Success3 ==2 && Success4 ==2) {
//        fault = false;
//    }
    return fault;
}
