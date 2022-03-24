//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Represents a packet in the network.
//
packet Packet
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;
    double size @packetData;
    int route[];
    int arraySizeAndIndex = 1;
    simtime_t queueDelayPerHop;
    string packetName;
    int timeSlotCounter;
}

