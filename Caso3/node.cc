#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class node : public cSimpleModule{
    private:
        int seq=0;
        simtime_t startTime=0;
        cMessage *event;
        double prob;
        cChannel* channelOut[2];
        cQueue* queueOut[2];
        int enlace;
        int gates[2];
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void tratarPaqueteNodo(myPacket *p);
        virtual void enviarPaqueteCola();
};

Define_Module(node);

void node::initialize(){
    //node n=getSystemModule();
    prob=(double)par("prob1");
    EV << "\nProb primer enlace: " << prob;

    //Guardar los canales de los enlaces de salida
    /*if(getIndex()==0){
        channelOut[0]=gate("gate$o", 2)->getTransmissionChannel();
        channelOut[1]=gate("gate$o", 3)->getTransmissionChannel();
        //channelOut[0]=gate()
        gates[0]=2;
        gates[1]=3;
    }else if(getIndex()==1){
        channelOut[0]=gate("gate$o", 0)->getTransmissionChannel();
        channelOut[1]=gate("gate$o", 1)->getTransmissionChannel();
        gates[0]=0;
        gates[1]=1;
    }else if(getIndex()==2){
        channelOut[0]=gate("gate$o", 1)->getTransmissionChannel();
        channelOut[1]=gate("gate$o", 2)->getTransmissionChannel();
        gates[0]=1;
        gates[1]=2;
    }*/

    channelOut[0]=gate("out", 0)->getTransmissionChannel();
    channelOut[1]=gate("out", 1)->getTransmissionChannel();
    //Generar las colaspara cada enlace de salida
    queueOut[0]=new cQueue("queue1");
    queueOut[1]=new cQueue("queue2");
}

void node::handleMessage(cMessage *msg){
    //delete msg;
    myPacket* p=check_and_cast<myPacket*>(msg);

    EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

    if(strcmp(p->getSenderModule()->getClassName(),"source")==0){//Paquete viene de las fuentes, no tienen error, no hay que enviar ACK/NAK
        EV << "\nIf de paquetes llegados de fuente";

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

    if(strcmp(p->getSenderModule()->getClassName(),"node")==0){//Paquete viene de otro nodo
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
        nak->setSeq(seq);
        nak->setSource(getIndex());
        nak->setType(2);
        seq++;

        send(nak,"out",g->getIndex());
        EV << "\nRespuesta " << nak->getName() << " enviada por enlace " << g->getIndex();

    }else{
        //SEND ACK
        myPacket* ack=new myPacket("ACK");
        ack->setSeq(seq);
        ack->setSource(getIndex());
        ack->setType(1);
        seq++;

        send(ack,"out",g->getIndex());
        EV << "\nRespuesta " << ack->getName() << " enviada por enlace " << g->getIndex();

        double rnd=((double)rand())/RAND_MAX;
        if(rnd<prob){
            enlace=0;
        }else{
            enlace=1;
        }
        queueOut[enlace]->insert(p);

        EV << "\nPaquete guardado en cola " << enlace;
    }
}


