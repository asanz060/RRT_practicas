#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class source : public cSimpleModule{
    private:
        double meanLength=1000;
        double meanTime=0.5;
        int seq=0;
        cMessage *event;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual myPacket* generarPaquete();
};

Define_Module(source);

void source::initialize(){
    event=new cMessage("event");

    double t=exponential(meanTime, 0);
    EV << "\nInitial time: " << t;
    scheduleAt(simTime()+t, event);
}

void source::handleMessage(cMessage *msg){
    EV << "\nSending packet from Source " << getIndex() << " At time " << simTime();
    myPacket* paquete=generarPaquete();

    send(paquete,"out");
    double t=exponential(0.5, 0);
    scheduleAt(simTime()+t, event);

}

myPacket* source::generarPaquete(){
    char packetName[20];
    sprintf(packetName,"Source%dPacket%d",getIndex(),seq);

    myPacket* p=new myPacket(packetName);
    p->setSeq(seq);
    p->setSource(getIndex());
    p->setType(0);
    //stat
    p->setHopCount(0);
    seq++;
    double l=exponential(meanLength, 0);
    EV << "\nPacket length: " << l << " Seq: " << seq;
    p->setBitLength(l);

    return p;
}



