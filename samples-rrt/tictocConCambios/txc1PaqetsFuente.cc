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
class txc1PaqetsFuente : public cSimpleModule
{
  private:
            double lambda = 2;
            double meanPacketLength;
            int samples = 100;
            int sequenceNumber = 0;
            long numSent;
            long numReceived;

  protected:
            virtual void initialize() override;
            virtual std::vector<double> getDepartures(double lambda, int samples);
            virtual std::vector<double> getLengths(double mu, int samples);
            virtual CustomPacket* getPacket();
            virtual void handleMessage(cMessage *msg) override;
            virtual void refreshDisplay() const override;
};

// The module class needs to be registered with OMNeT++
Define_Module(txc1PaqetsFuente);

void txc1PaqetsFuente::initialize()
{
    // Initialize variables
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);
    meanPacketLength = (double) par("meanPacketLength");
       // Get departure times, generate packets and schedule them
       std::vector<double> departures = getDepartures(lambda, samples);
       std::vector<double> lengths = getLengths(meanPacketLength, samples);
       for(int i = 0; i < departures.size(); i++) {
           // Scheduled packets will arrived to the same module at
           // departures[i]
           CustomPacket *pkt = getPacket();
           pkt -> setBitLength(lengths[i]);
           scheduleAt(departures[i], pkt);
       }
}


std::vector<double> txc1PaqetsFuente::getDepartures(double lambda, int samples) {
    // Generates an exponential distribution with SAMPLES length
    // and using LAMDBA.
    std::uniform_real_distribution<double> randomReal(0.0, 1.0);
    std::default_random_engine generator(time(NULL));
    std::vector<double> departures(samples);
    for(int i = 0; i < departures.size(); i++) {
        double randomNumber = randomReal(generator);
        double number = (-1/lambda) * log(randomNumber);
        if (i != 0)
            departures[i] = departures[i - 1] + number;
        else
            departures[i] = number;
    }
    return departures;
}

std::vector<double> txc1PaqetsFuente::getLengths(double meanPacketLength, int samples) {
    std::uniform_real_distribution<double> randomReal(0.0, 1.0);
    std::default_random_engine generator(time(NULL));
    std::vector<double> lengths(samples);
    for(int i = 0; i < lengths.size(); i++) {
        double randomNumber = randomReal(generator);
        double number = (-meanPacketLength) * log(randomNumber);
        lengths[i] = number;
    }
    return lengths;
}

CustomPacket* txc1PaqetsFuente::getPacket() {
    std::string packetName = "packet::" + std::to_string(getId()) + "::" + std::to_string(sequenceNumber);
    char *charPacketName = new char[packetName.length() + 1];
    strcpy(charPacketName, packetName.c_str());
    CustomPacket *pkt = new CustomPacket(charPacketName);
    pkt -> setDesdeDest(true);
    pkt -> setKind(1);
    pkt -> setNumSeq(sequenceNumber);
    pkt -> setOrigen(getId());
    sequenceNumber++;
    return pkt;
}

void txc1PaqetsFuente::handleMessage(cMessage *msg) {
    // Send scheduled packet
    CustomPacket *pkt = check_and_cast<CustomPacket *> (msg);
    send(pkt, "outPort");
    numSent++;
}

void txc1PaqetsFuente::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

