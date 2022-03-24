/*
 * Routing.h
 *
 *  Created on: 16-Sep-2021
 *      Author: MDSHA
 */

#ifndef NODE_ROUTING_H_
#define NODE_ROUTING_H_

#include <map>
#include <omnetpp.h>
#include "Packet_m.h"
#include <iostream>
#include <fstream>
using namespace omnetpp;
using namespace std;

class Routing : public cSimpleModule
{
private:
    int myAddress;
    int number_of_nodes;
    cModule *targetModuleNetworkManager;
    double timeSlot;
    cMessage *timeSlotMsg;
    int timeSlotCounter;
    int TotaltimeSlot;
    std::map<int, map<int, int> > dynamicRoutingTableUniCast;

public:
    Routing();
    virtual ~Routing();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleBroadCastTraffic(Packet *pk);
};


#endif /* NODE_ROUTING_H_ */
