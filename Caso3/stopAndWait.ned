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
        output out[];
}

simple endNode
{
    parameters:
        @display("i=block/arrival");
    gates:
        input in[];
        output out[];
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
        source1: source {
            @display("p=55,90");
        }
        source2: source {
            @display("p=55,30");
        }
        source5: source {
            @display("p=55,150");
        }
        node1: node {
            @display("p=150,90");
            prob1 = 1/4;
            gates:
                in[3];
                out[2];
        }
        node2: node {
            @display("p=250,30");
            prob1 = 1/3;
            gates:
                in[5];
                out[4];
        }
        node5: node {
            @display("p=250,150");
            prob1 = 1/2;
            gates:
                in[4];
                out[3];
        }
        node3: endNode {
            @display("p=400,30");
            gates:
                in[1];
                out[1];
        }
        node4: endNode {
            @display("p=400,150");
            gates:
                in[2];
                out[2];
        }
    connections:
        source1.out --> node1.in[0];
        source2.out --> node2.in[0];
        source5.out --> node5.in[0];

        node1.out[0] --> C --> node2.in[1];
        node1.out[1] --> C --> node5.in[1];

        node2.out[0] --> C --> node1.in[1];
        node2.out[1] --> C --> node5.in[2];
        node2.out[2] --> C --> node4.in[0];
        node2.out[3] --> C --> node3.in[0];

        node3.out[0] --> C --> node2.in[4];

        node4.out[0] --> C --> node2.in[3];
        node4.out[1] --> C --> node5.in[3];

        node5.out[0] --> C --> node1.in[2];
        node5.out[1] --> C --> node2.in[2];
        node5.out[2] --> C --> node4.in[1];
}
