//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.

network RandomGraph
{
    
    parameters:
        int number_of_nodes @prompt("Enter the number of nodes");// = default(10);
        int linkCapacityNode  @prompt("Enter the link capacity (packets/time-slot)");
        double packetSize  @prompt("Size of each packet (bytes)");
        double degreeOfConnectivity  @prompt("Enter the degree of connectivity among the nodes (between 0 to 1)");
        int TotalnumberOfSlot @prompt("Enter the total time slots");
        int Traffic_Class @prompt("Traffic class: Press 1 for unicast only, 2 for broadcast only and 3 for both unicast and broadcast");
        double Slot_Duration @prompt("Enter the slot duration (in seconds)");
        double lambda @prompt("Enter the poisson arrival rate (lambda)");
        int source_node  @prompt("Enter the source node (for unicast and broadcast)");
        int destination_node @prompt("Enter the destination node (for unicast)");
        double min_key_Gen_rate  @prompt("Enter the minimum key generation rate(lambda) for any edge");
        double max_key_Gen_rate  @prompt("Enter the maximum key generation rate(lambda) for any edge");
      types:
           channel packetPerSec extends ned.DatarateChannel {
	         delay = 20us;// uniform(50,100)* pow(10,-6)*1s;
	         datarate = 0;// (linkCapacityNode*packetLengthNode*8)*(1bps);
	        } 
    submodules:
        
        networkController: networkTopoInfo
         {
        @display("i=block/cogwheel");
        packetLengtNetworkTopo = packetSize;
        linkCapacityNetworkTopo = linkCapacityNode;
        TotalnumberOfSlot = TotalnumberOfSlot;      
        timeSlot = Slot_Duration; 
        min_key_Gen_rate = min_key_Gen_rate;
    	max_key_Gen_rate = max_key_Gen_rate;           
         }
         
        node[number_of_nodes]: Node {
            parameters:
                //  @display("i=misc/node_vs,blue");
                  @display("p=100,100,ring");
                linkCapacity = linkCapacityNode;
                Traffic_Class = Traffic_Class;
                packetSize = packetSize;
                TotalnumberOfSlot = TotalnumberOfSlot;
                n = number_of_nodes;
                address = index;
                Slot_Duration = Slot_Duration;
                lambda = lambda;
                source_node = source_node;
                destination_node = destination_node;
                //               numOfNodes = n;
              
            gates:
                port[number_of_nodes];
        }
//{delay = 0.1ms ; datarate = 1Gbps;}
    connections allowunconnected:
        for i=0..number_of_nodes-2, for j=i+1..number_of_nodes-1 {
            node[i].port[j] <--> packetPerSec {@display("ls=blackn,1.1");}    <--> node[j].port[i] if uniform(0,1)<degreeOfConnectivity;
           //   node[i].port[j] <--> packetPerSec {@display("ls=green,2");}   <--> node[j].port[i];
            
        }
}


