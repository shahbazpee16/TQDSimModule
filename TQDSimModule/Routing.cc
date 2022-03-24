//
//
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include "Routing.h"
#include "networkTopoInfo.h"


Routing::Routing()
{
    targetModuleNetworkManager=nullptr;
    timeSlotMsg = nullptr;

}
Routing::~Routing()
{
    cancelAndDelete(timeSlotMsg);
}

Define_Module(Routing);

void Routing::initialize()
{
    targetModuleNetworkManager = getModuleByPath("RandomGraph.networkController");
    myAddress = getParentModule()->par("address");
    timeSlot =  par("timeSlot");
    TotaltimeSlot = par("TotalnumberOfSlot");
    timeSlotCounter = 0;

    timeSlotMsg = new cMessage("nextTimeSlotMsg");
    scheduleAt(timeSlot, timeSlotMsg);
}

void Routing::handleMessage(cMessage *msg)
{
    networkTopoInfo *targetTopoInfo = check_and_cast<networkTopoInfo*>(targetModuleNetworkManager);

    if(msg->isSelfMessage())
    {
        dynamicRoutingTableUniCast = targetTopoInfo->rTableCalForUniCast_2();
        if (timeSlotCounter<TotaltimeSlot)
        {
            scheduleAt(simTime() + timeSlot, timeSlotMsg);
            timeSlotCounter++;
        }
    }

    if (strcmp(msg->getName(),"BroadCast Packet")==0)
    {
        Packet *pk = check_and_cast<Packet *>(msg);
        if (pk->getSrcAddr() == myAddress)
        {
            targetTopoInfo->updateVQforBroadcastTrafficArrivalAtSource();
            handleBroadCastTraffic(pk);
        }
        else
            handleBroadCastTraffic(pk);
    }

    if (strcmp(msg->getName(),"Unicast Packet")==0)
    {
        Packet *pk = check_and_cast<Packet *>(msg);
        int destAddr = pk->getDestAddr();

        if (destAddr == myAddress)
        {
            EV << "Delivery of packet " << pk->getPacketName() << " to node "<<myAddress<<endl;
            send(pk, "localOut");
        }

        if (destAddr != myAddress)
        {

            if (pk->getSrcAddr() == myAddress)
            {
                EV<<"Received unencrypted unicast packet in the routing module generated in time slot: "<<pk->getTimeSlotCounter()<<" from app of node "<<myAddress<<endl;
                pk->setRouteArraySize(pk->getArraySizeAndIndex());
                pk->setRoute((pk->getArraySizeAndIndex()-1), pk->getSrcAddr());
                pk->setArraySizeAndIndex(pk->getArraySizeAndIndex()+1);
                targetTopoInfo->updateVQonSrcArrvUnicastPacket(pk->getSrcAddr(), pk->getDestAddr(), dynamicRoutingTableUniCast, pk);
                pk->setHopCount(pk->getRouteArraySize()-1);
                EV << "The selected route between "<<pk->getSrcAddr()<<" to "<<pk->getDestAddr()<<" is: ";

                for(int i = 0; i < pk->getRouteArraySize(); i++)
                {
                    EV<<pk->getRoute(i)<<" ";
                }
                EV<<endl;

                pk->setArraySizeAndIndex(1);
                int outGateIndex = pk->getRoute((pk->getArraySizeAndIndex()));
                pk->setArraySizeAndIndex(pk->getArraySizeAndIndex()+1);
                EV<<"Unencrypted unicast packet is forwarded to Physical Queue X available in the module: "<<getParentModule()->getSubmodule("queue", outGateIndex)->getFullPath()<<endl;
                send(pk, "out", outGateIndex);
            }
            else
            {
                EV<<"Received encryted packet to Node "<<myAddress<<" from Node "<<pk->getRoute((pk->getArraySizeAndIndex()-1))<<endl;
                int outGateIndex = pk->getRoute((pk->getArraySizeAndIndex()));
                pk->setArraySizeAndIndex(pk->getArraySizeAndIndex()+1);
                send(pk, "out", outGateIndex);
            }
        }
    }

}


void Routing :: handleBroadCastTraffic(Packet *pk)
{
    networkTopoInfo *targetTopoInfo = check_and_cast<networkTopoInfo*>(targetModuleNetworkManager);
    cTopology::Node *thisNode = targetTopoInfo->returnTopologyObject(pk->getTimeSlotCounter())->getNodeFor(getParentModule());

    int numberOfOutLink = thisNode->getNumOutLinks();
    int numberOfInLink  = thisNode->getNumInLinks();

    if (strcmp(pk->getArrivalGate()->getName(), "localIn")==0)
    {
        for (int j=0; j<numberOfOutLink; j++)
        {
            cTopology::LinkOut *linkout = thisNode->getLinkOut(j);
            if (linkout->isEnabled())
            {
                cGate *gate = linkout->getLocalGate();
                EV<<" The incoming unencrypted broadcast packet from app of node "<<myAddress<<" is sent to physical queue X named as "<< linkout->getLocalNode()->getModule()->getSubmodule("queue", linkout->getRemoteNode()->getModule()->getIndex())->getFullPath()<<endl;// linkout->getRemoteNode()->getModule()->getFullName()<<endl;
                send(pk->dup(), "out", gate->getIndex());
            }
        }

        delete pk;
    }

    else if (strcmp(pk->getArrivalGate()->getName(),"in")==0)
    {
        int tempInLinkGateIndex = pk->getArrivalGate()->getIndex();
        for (int j=0; j<numberOfOutLink; j++)
        {
            cTopology::LinkOut *linkout = thisNode->getLinkOut(j);
            if (linkout->isEnabled())
            {
                cGate *gate = linkout->getLocalGate();
                if (gate->getIndex()!= tempInLinkGateIndex)
                {
                    EV<<" The copy of incoming packet is send to "<<linkout->getRemoteNode()->getModule()->getFullName()<<endl;
                    send(pk->dup(), "out", gate->getIndex());
                }
            }
        }
        EV << "Original packet is forward to app of node "<<myAddress<<endl;
        send(pk, "localOut");
    }
    else
        delete pk;
}

