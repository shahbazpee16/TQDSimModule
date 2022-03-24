//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Models a router.
//
simple Routing 
{
   
    parameters:	
        @display("i=block/switch");
        double timeSlot;
        int frameCapacity = 50000;  // default(0); 
        int TotalnumberOfSlot;
        @signal[qlen](type="long");
        @signal[drop](type="long");
        @statistic[qlen](title="queue length";record=timeavg,max,mean;interpolationmode=sample-hold);
        @statistic[drop](title="dropped packet byte length";unit=bytes;record=count,sum;interpolationmode=none);
        
    gates:
        input in[];
        output out[];
        input localIn;
        output localOut;
}


