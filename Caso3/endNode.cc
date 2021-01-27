#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class endNode : public cSimpleModule{
    private:
        int seqAck;
        int seqNak;
        int rcvPack;

        //stat
        cLongHistogram hopCountStats;
        cOutVector hopCountVector;

    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
       virtual void finish() override;
};

Define_Module(endNode);

void endNode::initialize(){
    seqAck=0;
    seqNak=0;
    rcvPack=0;
    WATCH(rcvPack);
    WATCH(seqAck);
    WATCH(seqNak);

    //Stats
    hopCountStats.setName("HopCountStats");
    hopCountStats.setRangeAutoUpper(0, 10, 1.5);
    hopCountVector.setName("HopCount");
}

void endNode::handleMessage(cMessage *msg){
    //delete msg;
   myPacket* p=check_and_cast<myPacket*>(msg);

   EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

   int type=p->getType();
   if(type==0){
       int gateIndex=p->getArrivalGate()->getIndex();

       if(p->hasBitError()){
           //SEND NAK
           myPacket* nak=new myPacket("NAK");
           nak->setSeq(seqNak);
           nak->setSource(getIndex());
           nak->setType(2);
           seqNak++;

           send(nak,"out",gateIndex);
           EV << "\nRespuesta " << nak->getName() << " enviada por enlace " << gateIndex;

       }else{
           //SEND ACK
           myPacket* ack=new myPacket("ACK");
           ack->setSeq(seqAck);
           ack->setSource(getIndex());
           ack->setType(1);
           seqAck++;

           send(ack,"out",gateIndex);
           EV << "\nRespuesta " << ack->getName() << " enviada por enlace " << gateIndex;
           EV << "\nPaquete llegado a destino sin errores";
           rcvPack++;

           //stat
           p->setHopCount(p->getHopCount()+1);
           hopCountVector.record(p->getHopCount());
           hopCountStats.collect(p->getHopCount());
       }
   }
}

void endNode::finish(){
    hopCountStats.recordAs("hop count");
}
