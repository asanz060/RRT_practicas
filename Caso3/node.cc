#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "myPacket_m.h"

using namespace omnetpp;

class node : public cSimpleModule{
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

};

Define_Module(node);

void node::initialize(){

}

void node::handleMessage(cMessage *msg){
    delete msg;
}
