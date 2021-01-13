//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
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
        src[3]: source;
        nd[3]: node;
        end[2]: endNode;
        
    connections:
        src[0].out --> nd[1].in++;
        src[1].out --> nd[0].in++;
        src[2].out --> nd[2].in++;

        nd[0].gate++ <--> C <--> nd[1].gate++;
        nd[0].gate++ <--> C <--> nd[2].gate++;
        nd[1].gate++ <--> C <--> nd[2].gate++;
        nd[1].gate++ <--> C <--> end[0].gate++;
        nd[1].gate++ <--> C <--> end[1].gate++;
        nd[2].gate++ <--> C <--> end[1].gate++;

}
