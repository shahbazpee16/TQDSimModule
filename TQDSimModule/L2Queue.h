/*
 * L2Queue.h
 *
 *  Created on: 16-Sep-2021
 *      Author: MDSHA
 */

#ifndef NODE_L2QUEUE_H_
#define NODE_L2QUEUE_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <algorithm>
#include "Packet_m.h"
using namespace omnetpp;
using namespace std;
class L2Queue : public cSimpleModule
{
  private:
    long frameCapacity;
    int linkCapacity;
    int myAddress;
    cModule *targetModuleNetworkManager;
    std::map<int, map<int, int> > keyStorageBank;
  //  long virtualQueueLength;
    simtime_t timeToUpdateVirtualQueue;
    int minLinkWeight = 1;
  //  long linkCapacity = 5; // packets per second per link
 //   cMessage *eventForVirtualQueueUpdate;
    cQueue queue;
    cMessage *endTransmissionEvent;
    bool isBusy;
    double timeSlot;
    int TotaltimeSlot;
    int timeSlotCounter;
    cMessage *timeSlotMsg;
    cQueue queueForUncryptedPackets;
    cQueue queueForEncryptedPackets;

    //cArray uncryptedPacketArray;

  //  simsignal_t qlenSignal;
//    simsignal_t busySignal;
//    simsignal_t queueingTimeSignal;
 //   simsignal_t dropSignal;
//    simsignal_t txBytesSignal;
//    simsignal_t rxBytesSignal;

  public:
    L2Queue();
    virtual ~L2Queue();
 //   virtual long getVirtualQueueCounter();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    virtual void startTransmitting(cMessage *msg);
};

#endif /* NODE_L2QUEUE_H_ */
