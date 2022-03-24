
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <map>
#include <omnetpp.h>
#include "Packet_m.h"
#include <string.h>
using namespace std;
using namespace omnetpp;


class App : public cSimpleModule
{
  private:

    int myAddress;  //Address of the parent node i.e., index of the node
    int numOfNodes; // Number of nodes in the network (user defined value)
    double packeSizeBytes; // Size of each packet
    int sourceNode;
    int destinationNode;

    double timeSlotDuration; // duration of each time slot in seconds (user defined value)
    double lambda; // lamda for poisson distribution (user defined value)
    int PacketArrivalPerSlot; // number of packets generated per time slot depending on the value of lambda
    int TotaltimeSlot; // Total simulation time in terms of time slot (user defined value)
    int timeSlotCounter; // Counter to count the number of slots completed in order to reach the total time slot.
    int traffic_class_index; // used to select the traffic class (user defined value)

    typedef std::map<int, std::string> trafficType;  //mapping: key is traffic_class_index and value is the name of traffic class (String)
    trafficType setTrafficType;

    cMessage *generatePacket; // message object to generate the packets
    cTopology *getTopoInfo;

    // signals to record the statistic
    simsignal_t endToEndDelaySignal; // used to record the end to end delay of packet received at the destination node
    simsignal_t hopCountSignal; // used to record the hop count of the packet received at the destination node


  public:
    App(); // Class Constructor
    virtual ~App(); // Class Destructor

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void generateUnicastPackt(string trafficClass, int totalPacketsArrivedInCurrentSlot);
    void generateBroadcastPacket(string trafficClass, int totalPacketsArrivedInCurrentSlot);
};

Define_Module(App);

App::App()
{
    generatePacket = nullptr;
}

App::~App()
{
    cancelAndDelete(generatePacket);
    delete getTopoInfo;
}

void App::initialize()
{
    numOfNodes = par("n");
    timeSlotDuration = par("timeSlot");
    TotaltimeSlot = par("TotalnumberOfSlot");
    traffic_class_index = par("Traffic_Class");
    myAddress = par("address");
    lambda = par("lambda");
    sourceNode = par("source_node");
    destinationNode = par("destination_node");
    timeSlotCounter=0;
    packeSizeBytes = par("packetSize");

    setTrafficType.insert(std::pair<int, string>(1, "Unicast"));
    setTrafficType.insert(std::pair<int, std::string>(2, "Broadcast"));
    setTrafficType.insert(std::pair<int, std::string>(3, "Mixed"));

    getTopoInfo = new cTopology("Topology finding at APP module");
    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getNedTypeName());
    getTopoInfo->extractByNedTypeName(nedTypes);
    cTopology::Node *node = getTopoInfo->getNodeFor(getParentModule());

    if(node->getNumOutLinks()>=1)
    {
//        if (myAddress == sourceNode && sourceNode!=destinationNode)
//        {
                if (myAddress == 1 || myAddress == 5)
                {
            generatePacket = new cMessage("nextPacket");
            scheduleAt(timeSlotDuration, generatePacket);
                }
//       }
    }

    endToEndDelaySignal = registerSignal("endToEndDelay");
    hopCountSignal = registerSignal("hopCount");

}

void App::handleMessage(cMessage *msg)
{
    if (msg == generatePacket)
    {
        if (timeSlotCounter<TotaltimeSlot)
           {
            trafficType::iterator it = setTrafficType.find(traffic_class_index);
            string traffic_Class = (*it).second;
            EV<< "Selected Traffic class is: "<<traffic_Class<<endl;
            PacketArrivalPerSlot = poisson(lambda);//poisson(lambda);2;

            if(strcmp(traffic_Class.c_str(), "Unicast")==0)
            {
             EV<<"Number of unicast packets generated in the time slot: "<<timeSlotCounter<< " are "<<PacketArrivalPerSlot<<endl;
             generateUnicastPackt(traffic_Class,PacketArrivalPerSlot);
            }

            if(strcmp(traffic_Class.c_str(), "Broadcast")==0)
            {
             EV<<"Number of broadcast packets generated in the time slot: "<<timeSlotCounter<< " are "<<PacketArrivalPerSlot<<endl;
             generateBroadcastPacket(traffic_Class,PacketArrivalPerSlot);
            }

            if(strcmp(traffic_Class.c_str(), "Mixed")==0) // broadCast traffic generation
            {
             EV<<"Number of mixed (unicast plus broadcast) packets generated in the time slot: "<<timeSlotCounter<< " are "<<PacketArrivalPerSlot<<endl;
             int num_of_unicast_pckt = intuniform(0,PacketArrivalPerSlot);
             EV<<"Among "<<PacketArrivalPerSlot<<" packets, number of unicast packets are: "<<num_of_unicast_pckt<<endl;
             int num_of_broadcast_pckt = PacketArrivalPerSlot - num_of_unicast_pckt;
             EV<<"and number of broadcast packets are: "<<num_of_broadcast_pckt<<endl;
             generateUnicastPackt(traffic_Class,num_of_unicast_pckt);
             generateBroadcastPacket(traffic_Class,num_of_broadcast_pckt);
            }

        timeSlotCounter++;
        scheduleAt(simTime() + timeSlotDuration, generatePacket);
           }
   }
    // Handle incoming packet
    else
    {
        Packet *pk = check_and_cast<Packet *>(msg);
        EV << "received packet " << pk->getName() << " after " << pk->getHopCount() << " hops" << " from "<<pk->getSrcAddr()<< " to "<< myAddress<< " with end to end delay  "<< pk->getQueueDelayPerHop()<<endl;
        emit(endToEndDelaySignal, pk->getQueueDelayPerHop());
        emit(hopCountSignal, pk->getHopCount());
        EV<<"The received packet was generated in the time slot "<<pk->getTimeSlotCounter()<<endl;
        delete pk;
        if (hasGUI())
        getParentModule()->bubble("Arrived!");
    }
}

void App :: generateBroadcastPacket(string traffic_Class, int Num_of_packet)
{
    if(Num_of_packet>=1)
    {
        if (hasGUI())
            getParentModule()->bubble("Generating Broadcast packet...");
    }

    for(int i=0; i<Num_of_packet;i++)
    {
        char pkname[40];
        sprintf(pkname, "broadCast-pk-%d-to-all", myAddress);
        EV << "Generated packet name is " << pkname << endl;
        Packet *pk = new Packet("BroadCast Packet");
        pk->setByteLength(packeSizeBytes);
        pk->setKind(intuniform(0, 7));
        pk->setSrcAddr(myAddress);
        pk->setQueueDelayPerHop(0);
        pk->setPacketName(pkname);
        pk->setTimeSlotCounter(timeSlotCounter);
        send(pk, "out");
    }

}

void App :: generateUnicastPackt(string traffic_Class, int Num_of_packet)
{
    if(Num_of_packet>=1)
    {
        if (hasGUI())
            getParentModule()->bubble("Generating Unicast Packet...");
    }

    for(int i=0; i<Num_of_packet;i++)
    {
        int destAddress;
        do
        {
            destAddress = intuniform(0, numOfNodes-1);//destinationNode;//intuniform(0, numOfNodes-1);
        }
        while (destAddress==myAddress);

        char pkname[40];
        sprintf(pkname, "uniCast-pk-%d-to-%d", myAddress, destAddress);
        EV << "Generated packet name is: " << pkname << endl;
        Packet *pk = new Packet("Unicast Packet");
        pk->setByteLength(packeSizeBytes);
        //   pk->setByteLength(packeLengthBytes->intValue());
        pk->setKind(intuniform(0, 7));
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);
        pk->setQueueDelayPerHop(0);
        pk->setPacketName(pkname);
        pk->setTimeSlotCounter(timeSlotCounter);
        send(pk, "out");
    }

}

