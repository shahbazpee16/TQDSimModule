/*
 * networkTopoInfo.h
 *
 *  Created on: 17-Sep-2021
 *      Author: Md Shahbaz Akhhtar
 */

#ifndef NODE_NETWORKTOPOINFO_H_
#define NODE_NETWORKTOPOINFO_H_

#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include "Routing.h"
#include <algorithm>
#include <string.h>

using namespace std;
using namespace omnetpp;

class networkTopoInfo : public cSimpleModule
{
private:
    int num_of_nodes;
    cModule *targetModule;
 //   long** graphNetwork_2;
    long** graphNetwork_Broadcast_Traffic; //// Actual graph of the network showing the edge weight based on virtual queue length
    long** graphMstperSlot_Broadcast; /////////// Graph after finding the MST from the actual graph graphNetwork_MST
    long** graphForKeyBankStoragePerEdge;
    double** graphForlambdaPerEdge;
    int linkCapacity;
    int minLinkWeight;
    double packetLength;
    double timeSlot;
    double minKeyGenerationRate;
    double maxKeyGenerationRate;
    int timeSlotCounter;
    int TotaltimeSlot;
  cTopology *getTopoInfo;

  vector<cTopology*> topoMstVector = {nullptr};

  std::map<int, map<int, int> > rTableUniCast;
  std::map<int, map<int, int> > virtualQueuePerEdge;
  std::map<int, map<int, int> > Y_virtualQueue;
  std::map<int, map<int, int> > X_virtualQueue;
  std::map<int, map<int, int> > keyStorageBank;
  std::map<int, map<int, int> > tempKeyStorageBank;


  cMessage *virtualQueueUpdation;

  public:
    networkTopoInfo();
    virtual ~networkTopoInfo();
   // long** read_adjency_graph_2();
    void updateGraphByVirtualQueueLength();
    void updateGraphByVQs();
    void keyGenerationAndStoragePerSlot();
    virtual std::map<int, map<int, int> > rTableCalForUniCast(cModule *node, int nodeAddress, bool initializeCall);
    void  updateVQonSrcArrvUnicastPacket(int srcAddr, int dstAddr, std::map<int, map<int, int> > tempRTable, Packet *pkt);
    void  updateVQforBroadcastTrafficArrivalAtSource();
    virtual std::map<int, map<int, int> > rTableCalForUniCast_2();
    std::map<int, map<int, int> > returnKeyStorageBank();
    virtual void findMstPerSlotFromAdjencyMatrix(); ////////// To find graphMstperSlot graph from graphNetwork_MST per slot
    virtual void topologyModificationPerSlotForBroadCast(int slotNumber);
   int returnSlotCounterValue();
  cTopology* returnTopologyObject(int temptimeSlotCounter);
    virtual void put_graph_weight_on_link();

    double getPacketSize();
    int getNumberOfNodes();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;



};



#endif /* NODE_NETWORKTOPOINFO_H_ */
