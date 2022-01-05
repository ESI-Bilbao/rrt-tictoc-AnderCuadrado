simple txc1PaqetsRedFinal
{
    parameters:
        @display("i=block/departure");
        //@signal[arrival](type="long");
        //@statistic[hopCount](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);
        double probability;
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsDestinoFinal
{
    parameters:
        @display("i=block/arrival");
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsFuenteFinal
{
    parameters:
        @display("i=block/source");
        double meanPacketLength;
    gates:
        output outPort;
}

network tictocFinal
{
    @display("bgb=558,343");
    //@display("bgb=285.6,182.28");
    //@figure[description](type=text; pos=5,20; font=,,bold; text="Random routing example - displaying last hop count");
    //@figure[lasthopcount](type=text; pos=5,35; text="last hopCount: N/A");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 3000bps;
            delay = 5ms;
            per = 0.25;
        }
    submodules:

        source1: txc1PaqetsFuente {
            @display("p=48,139");
            meanPacketLength = 9600/3;
        }
        source2: txc1PaqetsFuente {
            @display("p=54,237");
            meanPacketLength = 9600/3;
        }
        source3: txc1PaqetsFuente {
            @display("p=54,33");
            meanPacketLength = 9600/3;
        }
        node1: txc1PaqetsRed {
            @display("p=141.36,139.5");
            probability = 1/4;
            gates:
                inPort[3];
                outPort[2];
        }
        node2: txc1PaqetsRed {
            @display("p=259.47,237.15");
            probability = 1/2;
            gates:
                inPort[4];
                outPort[3];
        }
        node3: txc1PaqetsRed {
            @display("p=259.47,33.48");
            probability = 1/3;
            gates:
                inPort[5];
                outPort[4];
        }

        node4: txc1PaqetsDestino {
            @display("p=437.09998,33.48");
            gates:
                inPort[1];
                outPort[1];
        }

        node5: txc1PaqetsDestino {
            @display("p=437.09998,237.15");
            gates:
                inPort[2];
                outPort[2];
        }



    connections:
        node1.outPort[0] --> Channel --> node3.inPort[2];	//node1.outPort[0] --> Channel --> node2.inPort[2];
        node1.inPort[0] <-- Channel <-- node3.outPort[2]; 	// node1.inPort[0] <-- Channel <-- node2.outPort[2];

        node1.outPort[1] --> Channel --> node2.inPort[2];	//node1.outPort[1] --> Channel --> node5.inPort[2];
        node1.inPort[1] <-- Channel <-- node2.outPort[2];	//node1.inPort[1] <-- Channel <-- node5.outPort[2];

        node3.outPort[0] --> Channel --> node4.inPort[0];	//node2.outPort[0] --> Channel --> node3.inPort[0];
        node3.inPort[0] <-- Channel <-- node4.outPort[0];	//node2.inPort[0] <-- Channel <-- node3.outPort[0];

        node3.outPort[1] --> Channel --> node5.inPort[0];	//node2.outPort[1] --> Channel --> node4.inPort[0];
        node3.inPort[1] <-- Channel <-- node5.outPort[0];	//node2.inPort[1] <-- Channel <-- node4.outPort[0];

        node2.outPort[0] --> Channel --> node3.inPort[3];	//node5.outPort[0] --> Channel --> node2.inPort[3];
        node2.inPort[0] <-- Channel <-- node3.outPort[3];	//node5.inPort[0] <-- Channel <-- node2.outPort[3];

        node2.outPort[1] --> Channel --> node5.inPort[1];	//node5.outPort[1] --> Channel --> node4.inPort[1];
        node2.inPort[1] <-- Channel <-- node5.outPort[1];   //node5.inPort[1] <-- Channel <-- node4.outPort[1];


        source1.outPort --> node1.inPort[2];
        source2.outPort --> node2.inPort[3];
        source3.outPort --> node3.inPort[4];
}
