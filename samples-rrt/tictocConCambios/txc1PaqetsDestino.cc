//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <random>
using namespace omnetpp;
#include "paqueteDef_m.h"


/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class txc1PaqetsDestino : public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    //simsignal_t arrivalSignal;
    cLongHistogram hopCountStats;
    cOutVector hopCountVector;

    cHistogram e2eHistogram;
    cOutVector e2Delay;


protected:
        virtual void handleMessage(cMessage *msg) override;
        virtual void initialize() override;
        virtual void refreshDisplay() const override;
        virtual void finish() override;
};

// The module class needs to be registered with OMNeT++
Define_Module(txc1PaqetsDestino);

void txc1PaqetsDestino::initialize()
{
        // Initialize variables
            numSent = 0;
            numReceived = 0;
            WATCH(numSent);
            WATCH(numReceived);
            //arrivalSignal = registerSignal("arrival");
            e2Delay.setName("e2Delay");
            e2eHistogram.setName("e2eHistogram");


}



void txc1PaqetsDestino::handleMessage(cMessage *msg)
{
    CustomPacket *pkt = (CustomPacket*) msg;
    cGate *arrivalGate = pkt -> getArrivalGate();
    int arrivalGateIndex = arrivalGate -> getIndex();
    EV << "Packet arrived from gate " + std::to_string(arrivalGateIndex) + "\n";
    numReceived++;

    //int hopcount=pkt ->getHopcount();
    //EV <<"Packet" << pkt->getId() << "arrived after" <<hopcount <<"hops.\n";
    if (pkt -> getKind() == 1) { // 1: Packet
        if (pkt -> hasBitError()) {
            EV << "Packet arrived with error, send NAK\n";
            CustomPacket *nak = new CustomPacket("NAK");
            nak -> setKind(3);
            send(nak, "outPort", arrivalGateIndex);
            numSent++;
        }
        else {
            int hopcount=pkt->getHopcount();
            hopCountStats.collect(hopcount);
            hopCountVector.record(hopcount);
            double delay = (simTime() - pkt -> getCreationTime()).dbl();
            e2Delay.record(delay);
            e2eHistogram.collect(delay);
            EV << "Packet arrived without error, send ACK\n";
            CustomPacket *ack = new CustomPacket("ACK");
            ack -> setKind(2);
            send(ack, "outPort", arrivalGateIndex);
            numSent++;
            EV << "Packet it's okay!";
        }
    }
}

void txc1PaqetsDestino::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}
void txc1PaqetsDestino::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;

    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);

    hopCountStats.recordAs("hop count");
    e2eHistogram.recordAs("end to end Delay");
}
