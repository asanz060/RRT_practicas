#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class source : public cSimpleModule{
    private:
        double lambda=2;
        double meanLength=1000;
        int n=100;
        int seq=0;
        simtime_t startTime=0;
        cMessage *event;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual myPacket* generarPaquete();
};

Define_Module(source);

void source::initialize(){
    event=new cMessage("event");

    double t=exponential(0.5, 0);
    EV << "\nInitial time: " << t;
    scheduleAt(simTime()+t, event);
}

void source::handleMessage(cMessage *msg){
    EV << "\nSending packet from Source " << getIndex() << " At time " << simTime();
    myPacket* paquete=generarPaquete();

    cMessage *prueba=new cMessage("prueba");
    send(paquete,"out");
    double t=exponential(0.5, 0);
    scheduleAt(simTime()+t, event);

    //myPacket* p=check_and_cast<myPacket*>(msg);

}

myPacket* source::generarPaquete(){
    char packetName[20];
    sprintf(packetName,"Source%dPacket%d",getIndex(),seq);

    myPacket* p=new myPacket(packetName);
    p->setSeq(seq);
    p->setSource(getIndex());
    p->setType(0);
    seq++;
    double l=exponential(1000, 0);
    EV << "\nPacket length: " << l << " Seq: " << seq;
    p->setBitLength(l);

    return p;
}



