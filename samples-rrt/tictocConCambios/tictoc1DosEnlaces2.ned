simple txc1PaqetsRed2
{
    parameters:
        @display("i=block/departure");
        double probability;
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsDestino2
{
    parameters:
        @display("i=block/arrival");
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsFuente2
{
    parameters:
        @display("i=block/source");
        double meanPacketLength;
    gates:
        output outPort;
}

network tictoc1DosEnlaces2
{
    @display("bgb=285.6,182.28");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 1000bps;
            delay = 100us;
            per = 0.25;
        }
    submodules:

        source1: txc1PaqetsFuente {
            @display("p=29.4,78.96");
            meanPacketLength = 1000/3;
        }
        node2: txc1PaqetsDestino {
            @display("p=256.2,33.6");
            gates:
                inPort[1];
                outPort[1];
        }
        node1: txc1PaqetsRed {
            @display("p=144.15,71.61");
            probability = 1;
            gates:
                inPort[2];
                outPort[1];
        }


    connections:
        node1.outPort[0] --> Channel --> node2.inPort[0];
        node1.inPort[0] <-- Channel <-- node2.outPort[0];

        source1.outPort --> node1.inPort[1];

}
