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
protected:
        virtual void handleMessage(cMessage *msg) override;
        virtual void initialize() override;
        virtual void refreshDisplay() const override;
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

}



void txc1PaqetsDestino::handleMessage(cMessage *msg)
{
    CustomPacket *pkt = (CustomPacket*) msg;
    cGate *arrivalGate = pkt -> getArrivalGate();
    int arrivalGateIndex = arrivalGate -> getIndex();
    EV << "Packet arrived from gate " + std::to_string(arrivalGateIndex) + "\n";
    numReceived++;
    if (pkt -> getKind() == 1) { // 1: Packet
        if (pkt -> hasBitError()) {
            EV << "Packet arrived with error, send NAK\n";
            CustomPacket *nak = new CustomPacket("NAK");
            nak -> setKind(3);
            send(nak, "outPort", arrivalGateIndex);
            numSent++;
        }
        else {
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

