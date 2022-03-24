#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "networkTopoInfo.h"

Define_Module(networkTopoInfo);

networkTopoInfo::networkTopoInfo()
{
    virtualQueueUpdation = nullptr;
}

networkTopoInfo::~networkTopoInfo()
{
    delete getTopoInfo;
    for (int i = 0; i < topoMstVector.size(); i++)
    {
        delete topoMstVector.at(i);
    }

}

void networkTopoInfo::initialize()
{
    packetLength = par("packetLengtNetworkTopo");
    linkCapacity = par("linkCapacityNetworkTopo");
    TotaltimeSlot = par("TotalnumberOfSlot");
    timeSlot = par("timeSlot");
    minKeyGenerationRate = par("min_key_Gen_rate");
    maxKeyGenerationRate = par("max_key_Gen_rate");
    timeSlotCounter = 0;
    minLinkWeight = 0;
    virtualQueueUpdation = new cMessage("virtualQueueUpdatingGraph");

    getTopoInfo = new cTopology("getTopoInfo");
    std::vector<std::string> nedTypesforTopoInfo;
    targetModule = getModuleByPath("RandomGraph.node[0]");
    nedTypesforTopoInfo.push_back(targetModule->getNedTypeName());
    getTopoInfo->extractByNedTypeName(nedTypesforTopoInfo);
    num_of_nodes = getTopoInfo->getNumNodes();

    topoMstVector.resize(TotaltimeSlot);

  //  Graph weight allocation for unicast and broadcast traffic
    graphNetwork_Broadcast_Traffic = new long*[num_of_nodes];
    graphMstperSlot_Broadcast = new long*[num_of_nodes];
    graphForKeyBankStoragePerEdge = new long*[num_of_nodes];
    graphForlambdaPerEdge = new double*[num_of_nodes];
    for(int i=0;i<num_of_nodes;i++)
    {
        graphForKeyBankStoragePerEdge[i]= new long[num_of_nodes];
        graphForlambdaPerEdge[i]= new double[num_of_nodes];
        graphNetwork_Broadcast_Traffic[i] = new long[num_of_nodes];      //// Memory allocation for graph for broadcast traffic
        graphMstperSlot_Broadcast[i]= new long[num_of_nodes];
    }

    //Initialization of all the graphs defined

    for(int i=0; i<num_of_nodes;i++)
    {
        for (int j=0; j<num_of_nodes;j++)
        {
            graphForKeyBankStoragePerEdge[i][j] = 0;
            graphForlambdaPerEdge[i][j] = 0;
            graphNetwork_Broadcast_Traffic[i][j] = 0;
            graphMstperSlot_Broadcast[i][j] = 0;
        }

        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int k = 0; k < node->getNumOutLinks(); k++)
        {
            cTopology::Node *neighbour = node->getLinkOut(k)->getRemoteNode();
            cTopology::LinkOut *linkout = node->getLinkOut(k);
            cGate *gate = linkout->getLocalGate();
            int temp = 1;
            double lamdaPerEdge = uniform(minKeyGenerationRate,maxKeyGenerationRate);
            graphNetwork_Broadcast_Traffic[i][gate->getIndex()]=temp;
            graphNetwork_Broadcast_Traffic[gate->getIndex()][i]=temp;
            graphForlambdaPerEdge[i][gate->getIndex()]=lamdaPerEdge;
            graphForlambdaPerEdge[gate->getIndex()][i]=lamdaPerEdge;
            virtualQueuePerEdge[i][gate->getIndex()] = 0;
            Y_virtualQueue[i][gate->getIndex()] = 0;
            X_virtualQueue[i][gate->getIndex()] = 0;
            keyStorageBank[i][gate->getIndex()] = 0;
        }
    }
//    findMstPerSlotFromAdjencyMatrix();
//    topologyModificationPerSlotForBroadCast(timeSlotCounter);
    scheduleAt(timeSlot, virtualQueueUpdation);
}

void networkTopoInfo::handleMessage(cMessage *msg)
{
    if(timeSlotCounter<TotaltimeSlot)
    {
        EV<<"Slot Number: "<<timeSlotCounter<<endl;
        findMstPerSlotFromAdjencyMatrix();
        topologyModificationPerSlotForBroadCast(timeSlotCounter);
        keyGenerationAndStoragePerSlot();
        updateGraphByVQs();
        updateGraphByVirtualQueueLength();
        timeSlotCounter++;
        scheduleAt(simTime()+timeSlot, virtualQueueUpdation);
    }

    else

        delete virtualQueueUpdation;
}

std::map<int, map<int, int> > networkTopoInfo::rTableCalForUniCast(cModule *node, int nodeAddress, bool initializeCall)
{
    Enter_Method_Silent("Get Routing Table");
    put_graph_weight_on_link();

    cTopology::Node *thisNode = getTopoInfo->getNodeFor(node);
    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        if (getTopoInfo->getNode(i) == thisNode)
            continue;  // skip ourselves
        getTopoInfo->calculateWeightedSingleShortestPathsTo(getTopoInfo->getNode(i));

        if (thisNode->getNumPaths() == 0)
            continue;  // not connected

        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
        int gateIndex = parentModuleGate->getIndex();
        int address = getTopoInfo->getNode(i)->getModule()->par("address");
        rTableUniCast[nodeAddress][address] = gateIndex;
    }

    return rTableUniCast;
}

std::map<int, map<int, int> > networkTopoInfo::rTableCalForUniCast_2()
{
    Enter_Method_Silent("Get Routing Table");
    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        cTopology::Node *thisNode = getTopoInfo->getNode(i);
        int nodeAddress = thisNode->getModule()->par("address");
        rTableCalForUniCast(thisNode->getModule(), nodeAddress, true);
    }

    return rTableUniCast;
}

void networkTopoInfo :: put_graph_weight_on_link()
{
    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int j = 0; j < node->getNumOutLinks(); j++)
        {
            cTopology::Node *neighbour = node->getLinkOut(j)->getRemoteNode();
            cTopology::LinkOut *linkout = node->getLinkOut(j);
            cGate *gate = linkout->getLocalGate();
            linkout->setWeight(virtualQueuePerEdge[i][gate->getIndex()]);
        }
    }
}

void  networkTopoInfo:: updateVQonSrcArrvUnicastPacket(int srcAddr, int dstAddr, std::map<int, map<int, int> > tempRTable, Packet *pkt)
{
    cTopology::Node *node = getTopoInfo->getNode(srcAddr);

    if (tempRTable[srcAddr][dstAddr]==dstAddr)
    {
        X_virtualQueue[srcAddr][dstAddr] = X_virtualQueue[srcAddr][dstAddr] + 1;
        X_virtualQueue[dstAddr][srcAddr] = X_virtualQueue[dstAddr][srcAddr] + 1;
        Y_virtualQueue[srcAddr][dstAddr] = Y_virtualQueue[srcAddr][dstAddr] + 1;
        Y_virtualQueue[dstAddr][srcAddr] = Y_virtualQueue[dstAddr][srcAddr] + 1;
        pkt->setRouteArraySize(pkt->getArraySizeAndIndex());
        pkt->setRoute((pkt->getArraySizeAndIndex()-1), dstAddr);
    }

    else
    {
        int tempAdrss = tempRTable[srcAddr][dstAddr];
        pkt->setRouteArraySize(pkt->getArraySizeAndIndex());
        pkt->setRoute((pkt->getArraySizeAndIndex()-1), tempAdrss);
        pkt->setArraySizeAndIndex(pkt->getArraySizeAndIndex()+1);
        X_virtualQueue[srcAddr][tempAdrss] = X_virtualQueue[srcAddr][tempAdrss]+1;
        X_virtualQueue[tempAdrss][srcAddr] = X_virtualQueue[tempAdrss][srcAddr]+1;
        Y_virtualQueue[srcAddr][tempAdrss] = Y_virtualQueue[srcAddr][tempAdrss]+1;
        Y_virtualQueue[tempAdrss][srcAddr] = Y_virtualQueue[tempAdrss][srcAddr]+1;

        updateVQonSrcArrvUnicastPacket(tempAdrss, dstAddr, tempRTable, pkt);
    }
}

void networkTopoInfo :: updateGraphByVirtualQueueLength()
{
    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int j = 0; j < node->getNumOutLinks(); j++)
        {
            cTopology::LinkOut *linkout = node->getLinkOut(j);
            cGate *gate = linkout->getLocalGate();
            virtualQueuePerEdge[i][gate->getIndex()] =  Y_virtualQueue[i][gate->getIndex()] +  X_virtualQueue[i][gate->getIndex()];
            graphNetwork_Broadcast_Traffic[i][gate->getIndex()] =  max(1,virtualQueuePerEdge[i][gate->getIndex()]);
        }
    }

    map<int, map<int, int> >::iterator itr;
    map<int, int>::iterator ptr;
    EV << "\n";
    for (itr = virtualQueuePerEdge.begin(); itr != virtualQueuePerEdge.end(); itr++)
    {
        for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
            EV << "Total link weight W (after adding X and Y) between node " <<itr->first<< " and "<<ptr->first<<" is "<< ptr->second<<endl;
        }
    }
}

void networkTopoInfo :: updateGraphByVQs()
{
        map<int, map<int, int> >::iterator itr2;
        map<int, int>::iterator ptr2;
        EV << "\nVirtual queue Y after packet arrival to source is"<<endl;
        for (itr2 = Y_virtualQueue.begin(); itr2 != Y_virtualQueue.end(); itr2++)
        {
            for (ptr2 = itr2->second.begin(); ptr2 != itr2->second.end(); ptr2++)
            {
                EV << "Before: Virtual queue length Y between node " <<itr2->first<< " and "<<ptr2->first<<" is "<< ptr2->second<<endl;
            }
        }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        map<int, map<int, int> >::iterator itr1;
        map<int, int>::iterator ptr1;
        EV << "\nVirtual queue X after packet arrival to source is"<<endl;
        for (itr1 = X_virtualQueue.begin(); itr1 != X_virtualQueue.end(); itr1++)
        {
            for (ptr1 = itr1->second.begin(); ptr1 != itr1->second.end(); ptr1++)
            {
                EV << "Before: Virtual queue length X between node " <<itr1->first<< " and "<<ptr1->first<<" is "<< ptr1->second<<endl;
            }
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        map<int, map<int, int> >::iterator itr3;

        // For accessing inner map
        map<int, int>::iterator ptr3;
        EV << "\nKey storage Bank detail before updation is"<<endl;
        for (itr3 = keyStorageBank.begin(); itr3 != keyStorageBank.end(); itr3++) {

            for (ptr3 = itr3->second.begin(); ptr3 != itr3->second.end(); ptr3++) {

                EV << "Before: Available key between node " <<itr3->first<< " and "<<ptr3->first<<" is "<< ptr3->second<<endl;
            }
        }


        for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
        {
            cTopology::Node *node = getTopoInfo->getNode(i);
            for (int j = 0; j < node->getNumOutLinks(); j++)
            {
                cTopology::LinkOut *linkout = node->getLinkOut(j);
                cGate *gate = linkout->getLocalGate();
                Y_virtualQueue[i][gate->getIndex()]=  max(minLinkWeight,(Y_virtualQueue[i][gate->getIndex()] - linkCapacity));
                if (X_virtualQueue[i][gate->getIndex()] >= keyStorageBank[i][gate->getIndex()])
                {
                    X_virtualQueue[i][gate->getIndex()] = X_virtualQueue[i][gate->getIndex()] - keyStorageBank[i][gate->getIndex()];
                    keyStorageBank[i][gate->getIndex()] = 0;
                }
                else
                {
                    keyStorageBank[i][gate->getIndex()] = keyStorageBank[i][gate->getIndex()] - X_virtualQueue[i][gate->getIndex()];
                    X_virtualQueue[i][gate->getIndex()] = 0;
                }

            }

        }

        map<int, map<int, int> >::iterator itr4;
        map<int, int>::iterator ptr4;
        EV << "\nVirtual queue X after updation"<<endl;
        for (itr4 = X_virtualQueue.begin(); itr4 != X_virtualQueue.end(); itr4++) {

            for (ptr4 = itr4->second.begin(); ptr4 != itr4->second.end(); ptr4++) {

                EV << "After: Virtual queue length X between node " <<itr4->first<< " and "<<ptr4->first<<" is "<< ptr4->second<<endl;
            }
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        map<int, map<int, int> >::iterator itr6;
        map<int, int>::iterator ptr6;
        EV << "\nUnsed Key in the current slot is"<<endl;
        for (itr6 = keyStorageBank.begin(); itr6 != keyStorageBank.end(); itr6++) {

            for (ptr6 = itr6->second.begin(); ptr6 != itr6->second.end(); ptr6++) {

                EV << "After: Available key between the node " <<itr6->first<< " and "<<ptr6->first<<" is "<< ptr6->second<<endl;
            }
        }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////
        map<int, map<int, int> >::iterator itr5;
        map<int, int>::iterator ptr5;
        EV << "\nVirtual queue Y after updation is"<<endl;
        for (itr5 = Y_virtualQueue.begin(); itr5 != Y_virtualQueue.end(); itr5++) {

            for (ptr5 = itr5->second.begin(); ptr5 != itr5->second.end(); ptr5++) {

                EV << "After: Virtual queue length Y between node " <<itr5->first<< " and "<<ptr5->first<<" is "<< ptr5->second<<endl;
            }
        }

}

void networkTopoInfo :: keyGenerationAndStoragePerSlot()
{
    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int j = 0; j < node->getNumOutLinks(); j++)
        {
            cTopology::LinkOut *linkout = node->getLinkOut(j);
            cGate *gate = linkout->getLocalGate();
            int tempKeyGenerated =  poisson(graphForlambdaPerEdge[i][gate->getIndex()]);
            graphForKeyBankStoragePerEdge[i][gate->getIndex()]=tempKeyGenerated;
            graphForKeyBankStoragePerEdge[gate->getIndex()][i]=tempKeyGenerated;

        }
    }

    for (int i = 0; i < getTopoInfo->getNumNodes(); i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int j = 0; j < node->getNumOutLinks(); j++)
        {
            cTopology::LinkOut *linkout = node->getLinkOut(j);
            cGate *gate = linkout->getLocalGate();
            keyStorageBank[i][gate->getIndex()] = keyStorageBank[i][gate->getIndex()] + graphForKeyBankStoragePerEdge[i][gate->getIndex()];
        }

    }

    tempKeyStorageBank = keyStorageBank;
}
//////////////////////////////////////////////////////////////
//Operation for braodcast traffic starts from here

void networkTopoInfo :: findMstPerSlotFromAdjencyMatrix() ////////// To find graphMstperSlot_Broadcast graph from graphNetwork_Broadcast_Traffic per slot
{
    for(int i=0; i<num_of_nodes;i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int k = 0; k < node->getNumOutLinks(); k++)
        {
            cTopology::Node *neighbour = node->getLinkOut(k)->getRemoteNode();
            cTopology::LinkOut *linkout = node->getLinkOut(k);
            cGate *gate = linkout->getLocalGate();
            graphMstperSlot_Broadcast[i][gate->getIndex()]=0;
            graphMstperSlot_Broadcast[gate->getIndex()][i]=0;

        }

    }

    int edge;            // number of edge

    // create an array to check visited vertex
    int visit[num_of_nodes];

    //initialise the visit array to false
    for(int i=0;i<num_of_nodes;i++)
    {
        visit[i]=false;
    }

    // set number of edge to 0
    edge = 0;

    // the number of edges in minimum spanning tree will be
    // always less than (V -1), where V is the number of vertices in
    //graph

    // choose 0th vertex and make it true
    visit[0] = true;

    int x;            //  row number
    int y;            //  col number

    // print for edge and weight
    EV << "Edge" << " : " << "Weight";
    EV << endl;
    //    int graph[V][V] ={0};

    while (edge < num_of_nodes - 1) {//in spanning tree consist the V-1 number of edges

        int min = INT_MAX;
        x = 0;
        y = 0;

        for (int i = 0; i < num_of_nodes; i++) {
            if (visit[i]) {
                for (int j = 0; j < num_of_nodes; j++) {
                    if (!visit[j] && graphNetwork_Broadcast_Traffic[i][j]) { // not in selected and there is an edge
                        if (min > graphNetwork_Broadcast_Traffic[i][j])
                        {
                            min = graphNetwork_Broadcast_Traffic[i][j];
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }
        graphMstperSlot_Broadcast[x][y] = 1; //graphNetwork_Broadcast_Traffic[x][y];
        graphMstperSlot_Broadcast[y][x] = 1; //graphNetwork_Broadcast_Traffic[x][y];

        EV << x <<  " ---> " << y ;//<< " :  " << graphNetwork_Broadcast_Traffic[x][y];
        EV << endl;
        visit[y] = true;
        edge++;
    }

}

void networkTopoInfo :: topologyModificationPerSlotForBroadCast(int slotNumber)

{
    topoMstVector.at(slotNumber)= new cTopology();
    std::vector<std::string> nedTypesforTopoInfo;
    targetModule = getModuleByPath("RandomGraph.node[0]");
    nedTypesforTopoInfo.push_back(targetModule->getNedTypeName());
    topoMstVector.at(slotNumber)->extractByNedTypeName(nedTypesforTopoInfo);

    for (int i = 0; i < topoMstVector.at(slotNumber)->getNumNodes(); i++)
    {
        cTopology::Node *node = topoMstVector.at(slotNumber)->getNode(i);
        for (int j = 0; j < node->getNumOutLinks(); j++)
        {
            cTopology::Node *neighbour = node->getLinkOut(j)->getRemoteNode();
            cTopology::LinkOut *linkout = node->getLinkOut(j);
            cGate *gate = linkout->getLocalGate();
            if (graphMstperSlot_Broadcast[i][gate->getIndex()]==0)
            {
                linkout->setWeight(graphNetwork_Broadcast_Traffic[i][gate->getIndex()]);
                linkout-> disable();
             }
            else
            {
                linkout->setWeight(graphMstperSlot_Broadcast[i][gate->getIndex()]);
            }

        }

    }
}

void networkTopoInfo :: updateVQforBroadcastTrafficArrivalAtSource()
{
    for(int i=0; i<num_of_nodes;i++)
    {
        cTopology::Node *node = getTopoInfo->getNode(i);
        for (int k = 0; k < node->getNumOutLinks(); k++)
        {
            cTopology::Node *neighbour = node->getLinkOut(k)->getRemoteNode();
            cTopology::LinkOut *linkout = node->getLinkOut(k);
            cGate *gate = linkout->getLocalGate();
            Y_virtualQueue[i][gate->getIndex()] = Y_virtualQueue[i][gate->getIndex()] +  graphMstperSlot_Broadcast[i][gate->getIndex()];
            X_virtualQueue[i][gate->getIndex()] = X_virtualQueue[i][gate->getIndex()] +  graphMstperSlot_Broadcast[i][gate->getIndex()];
        }

    }

}

double networkTopoInfo :: getPacketSize()
{
    return packetLength;
}
int networkTopoInfo :: getNumberOfNodes()
{
    return num_of_nodes;
}

int networkTopoInfo::returnSlotCounterValue() // to return the present time slot counter value
{
    return timeSlotCounter;

}

cTopology* networkTopoInfo :: returnTopologyObject(int temptimeSlotCounter)
{

   return topoMstVector.at(temptimeSlotCounter);//getTopoInfo ;// topoMstVector[temptimeSlotCounter]; // topoMstVector[0];
}

std::map<int, map<int, int> > networkTopoInfo::returnKeyStorageBank()
{
    return tempKeyStorageBank;

}
