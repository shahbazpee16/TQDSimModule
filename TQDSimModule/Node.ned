//
// 
//This code is made by Md Shahbaz Akhtar, Date: 10-08-2021
// 
//
// 
//



module Node
{
    parameters:
        int address;
        int n;
        int linkCapacity;
        string appType;
        int TotalnumberOfSlot;
        int Traffic_Class;
        double Slot_Duration;
        double lambda;
        int source_node;
        int destination_node;
        double packetSize;
        @display("i=old/x_blank,green,10");
    gates:
        inout port[];
    submodules:
        //      app: <appType> like IApp {
        app: App {
            parameters:
                address = address;
                n = n;
                packetSize = packetSize;
                TotalnumberOfSlot = TotalnumberOfSlot;
                Traffic_Class = Traffic_Class;
                timeSlot = Slot_Duration;
                lambda = lambda;
                source_node = source_node;
                destination_node = destination_node;
                //  packetLength = packetLength;
                //           numOfNodes = numOfNodes;
                @display("p=140,60");
        }

        routing: Routing {
            parameters:
                @display("p=140,130");
                TotalnumberOfSlot = TotalnumberOfSlot;
                timeSlot = Slot_Duration;
            gates:
                in[sizeof(port)];
                out[sizeof(port)];
        }
        queue[sizeof(port)]: L2Queue {
            parameters:
                TotalnumberOfSlot = TotalnumberOfSlot;
                linkCapacity = linkCapacity;
                timeSlot = Slot_Duration;
                @display("p=80,200,row");
        }
    connections:
        routing.localOut --> app.in;
        routing.localIn <-- app.out;
        for i=0..sizeof(port)-1 {
            routing.out[i] --> queue[i].in;
            routing.in[i] <-- queue[i].out;
            queue[i].line <--> port[i];
        }
}


