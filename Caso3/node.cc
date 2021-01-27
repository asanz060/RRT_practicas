#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class node : public cSimpleModule{
    private:
        int seqAck;
        int seqNak;
        int enlace;
        double prob;
        cChannel* channelOut[2];
        cQueue* queueOut[2];

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void tratarPaqueteNodo(myPacket *p);
        virtual void enviarPaqueteCola();
};

Define_Module(node);

void node::initialize(){
    seqAck=0;
    seqNak=0;
    prob=(double)par("prob1");
    EV << "\nProb primer enlace: " << prob;

    channelOut[0]=gate("out", 0)->getTransmissionChannel();
    channelOut[1]=gate("out", 1)->getTransmissionChannel();
    //Generar las colaspara cada enlace de salida
    queueOut[0]=new cQueue("queue1");
    queueOut[1]=new cQueue("queue2");
}

void node::handleMessage(cMessage *msg){
    myPacket* p=check_and_cast<myPacket*>(msg);

    EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

    if(strcmp(p->getSenderModule()->getClassName(),"source")==0){//Paquete viene de las fuentes, no tienen error, no hay que enviar ACK/NAK
        EV << "\nIf de paquetes llegados de fuente";

        //stat
        p->setHopCount(p->getHopCount()+1);

        double rnd=((double)rand())/RAND_MAX;
        if(rnd<prob){
            enlace=0;
        }else{
            enlace=1;
        }

        if(queueOut[enlace]->isEmpty()){ //Si la cola esta vacia: insertar y enviar
            queueOut[enlace]->insert(p);
            EV << "\nPaquete guardado en cola " << enlace;

            if(channelOut[enlace]->isBusy()==false){
                enviarPaqueteCola();
            }
        }else{ //Si hay paquetes en la cola: solo insertar
            queueOut[enlace]->insert(p);
            EV << "\nPaquete guardado en cola " << enlace;
        }
    }
    else{//Paquete viene de otro nodo
        EV << "\n If de paquetes llegados de nodo";

        int type=p->getType();
        int gateIndex=p->getArrivalGate()->getIndex();

        switch (type){
                case 0://Mensaje normal
                    EV << "\nPaquete recibido";
                    tratarPaqueteNodo(p);
                    break;
                case 1://ACK
                    EV << "\nACK recibido";
                    delete(p);
                    queueOut[gateIndex]->pop();

                    EV << "\nqueue length: " << queueOut[gateIndex]->getLength();

                    if(!queueOut[enlace]->isEmpty() && !channelOut[enlace]->isBusy()){
                        enviarPaqueteCola();
                    }
                    break;
                case 2://NAK
                    EV << "\nNAK recibido";
                    delete(p);
                    if(channelOut[enlace]->isBusy()==false){
                        enviarPaqueteCola();
                    }
                    break;
            }
    }
}

void node::enviarPaqueteCola(){
    myPacket* p=check_and_cast<myPacket*> (queueOut[enlace]->front());
    send(p->dup(),"out",enlace);

    EV << "\nPaquete enviado por enlace " << enlace;
}

void node::tratarPaqueteNodo(myPacket *p){
    cGate* g=p->getArrivalGate();

    if(p->hasBitError()){
        //SEND NAK
        myPacket* nak=new myPacket("NAK");
        nak->setSeq(seqNak);
        nak->setSource(getIndex());
        nak->setType(2);
        seqNak++;

        send(nak,"out",g->getIndex());
        EV << "\nRespuesta " << nak->getName() << " enviada por enlace " << g->getIndex();

    }else{
        //SEND ACK
        myPacket* ack=new myPacket("ACK");
        ack->setSeq(seqAck);
        ack->setSource(getIndex());
        ack->setType(1);
        seqAck++;

        send(ack,"out",g->getIndex());
        EV << "\nRespuesta " << ack->getName() << " enviada por enlace " << g->getIndex();

        //stat
        p->setHopCount(p->getHopCount()+1);

        double rnd=((double)rand())/RAND_MAX;
        if(rnd<prob){
            enlace=0;
        }else{
            enlace=1;
        }

        if(queueOut[enlace]->isEmpty()){ //Si la cola esta vacia: insertar y enviar
            queueOut[enlace]->insert(p);
            EV << "\nPaquete guardado en cola " << enlace;

            if(channelOut[enlace]->isBusy()==false){
                enviarPaqueteCola();
            }
        }else{ //Si hay paquetes en la cola: solo insertar
            queueOut[enlace]->insert(p);
            EV << "\nPaquete guardado en cola " << enlace;
        }

        EV << "\nPaquete guardado en cola " << enlace;
    }
}


