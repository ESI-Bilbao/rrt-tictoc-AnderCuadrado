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
using namespace omnetpp;

/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class Txc1 : public cSimpleModule
{
  private:
      simtime_t timeout;  // timeout
      cMessage *timeoutEvent;  // holds pointer to the timeout self-message
      int seq;  // message sequence number
      cMessage *message;  // message that has to be re-sent on timeout

  public:
    Txc1();
    virtual ~Txc1();
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(Txc1);
Txc1::Txc1()
{
    timeoutEvent = message = nullptr;
}

Txc1::~Txc1()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}
void Txc1::initialize()
{
    // Initialize variables.
       seq = 0;
       timeout = 1.0;
       timeoutEvent = new cMessage("timeoutEvent");

       // Generate and send initial message.
       EV << "Sending initial message\n";
       message = generateNewMessage();
       sendCopyOf(message);
       scheduleAt(simTime()+timeout, timeoutEvent);
}

void Txc1::handleMessage(cMessage *msg)
{
    if (msg == timeoutEvent) {
           // If we receive the timeout event, that means the packet hasn't
           // arrived in time and we have to re-send it.
           EV << "Timeout expired, resending message and restarting timer\n";
           sendCopyOf(message);
           scheduleAt(simTime()+timeout, timeoutEvent);
       }
       else {  // message arrived
               // Acknowledgement received!
           EV << "Received: " << msg->getName() << "\n";
           delete msg;

           // Also delete the stored message and cancel the timeout event.
           EV << "Timer cancelled.\n";
           cancelEvent(timeoutEvent);
           delete message;

           // Ready to send another one.
           message = generateNewMessage();
           sendCopyOf(message);
           scheduleAt(simTime()+timeout, timeoutEvent);
       }
}
cMessage *Txc1::generateNewMessage()
{
    // Generate a message with a different name every time.
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Txc1::sendCopyOf(cMessage *msg)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, "out");
}

/**
 * Sends back an acknowledgement -- or not.
 */
class Txoc1 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txoc1);

void Txoc1::handleMessage(cMessage *msg)
{
    if (uniform(0, 1) < 0.1) {
        EV << "\"Losing\" message " << msg << endl;
        bubble("message lost");
        delete msg;
    }
    else {
        EV << msg << " received, sending back an acknowledgement.\n";
        delete msg;
        send(new cMessage("ack"), "out");
    }
}

