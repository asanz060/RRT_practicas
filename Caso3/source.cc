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
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual double getPacketLenght(double lambda);
        virtual double getTime(double meanLength);
        virtual myPacket* generarPaquete();
};

Define_Module(source);

void source::initialize(){
    for(int i=0;i<n;i++){
        double l=getPacketLenght(meanLength);
        double t=getTime(lambda);
        myPacket* paquete=generarPaquete();
        paquete->setBitLength(l);
        scheduleAt(simTime()+t, paquete);
    }
}

void source::handleMessage(cMessage *msg){
    myPacket* p=check_and_cast<myPacket*>(msg);
    send(p,"out");
}

double source::getPacketLenght(double meanLength){
    std::exponential_distribution<double> randomExp(1/meanLength);
    std::default_random_engine generator;

    double l=randomExp(generator);
    //printf("%f ",l);
    return l;
}
double source::getTime(double lambda){
    std::exponential_distribution<double> randomExp(1/lambda);
    std::default_random_engine generator;

    double t=randomExp(generator);
    //printf("%f ",t);
    return t;
}

myPacket* source::generarPaquete(){
    //std::string name=getName()+"Packet"+std::to_string(seq);
    char packetName[20];
    strcpy(packetName,getName());
    strcpy(packetName, "Packet");
    std::string id=std::to_string(seq);
    strcpy(packetName, id.c_str());
    myPacket* p=new myPacket(packetName);
    p->setSeq(seq);
    p->setSource(getId());
    p->setType(1);
    seq++;

    return p;
}



