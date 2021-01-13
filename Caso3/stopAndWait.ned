//
// This program is free software: you can redistribute it anode/or modify
// it unodeer the terms of the GNU Lesser General Public License as published by
// the Free Software Founodeation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO auto-generated type
//

import ned.DatarateChannel;

simple source
{
    parameters:
        @display("i=block/source");
        volatile double times @unit(s);
        volatile double lenghts @unit(bit);
    gates:
        output out;
}

simple node
{
    parameters:
        @display("i=block/routing");
        double prob1;
    gates:
        input in[];
        inout gate[];
}
simple endNode
{
    parameters:
        @display("i=block/arrival");
    gates:
        inout gate[];
}

network StopAndWait
{
    @display("bgb=500,200");
    types:
        channel C extends DatarateChannel
        {
            datarate = 1000bps;
            per = 0.01;
            delay = 0.1ms;
        }
    submodules:
        //node PROBS???
        src[3]: source {
            @display("p=50,50,c,10,10,10");
        }
        node[3]: node {
            @display("p=150,50,c,50");
        }
        endNode[2]: endNode {
            @display("p=250,50,c,50");
        }

    connections:
        src[0].out --> node[1].in++;
        src[1].out --> node[0].in++;
        src[2].out --> node[2].in++;

        node[0].gate++ <--> C <--> node[1].gate++;
        node[0].gate++ <--> C <--> node[2].gate++;
        node[1].gate++ <--> C <--> node[2].gate++;
        node[1].gate++ <--> C <--> endNode[0].gate++;
        node[1].gate++ <--> C <--> endNode[1].gate++;
        node[2].gate++ <--> C <--> endNode[1].gate++;

}
