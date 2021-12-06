simple txc1PaqetsRed
{
    parameters:
        @display("i=block/departure");
        double probability;
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsDestino
{
    parameters:
        @display("i=block/arrival");
    gates:
        input inPort[];
        output outPort[];
}

simple txc1PaqetsFuente
{
    parameters:
        @display("i=block/source");
        double meanPacketLength;
    gates:
        output outPort;
}

network tictoc1DosEnlaces
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
                inPort[2];//Indica el numero de puertos de entrada
                outPort[2];//Indica el numero de puertos de salida
        }
        node1: txc1PaqetsRed {
            @display("p=144.15,71.61");
            gates:
                inPort[2];
                outPort[2];
        }


    connections:
        node1.outPort[1] --> Channel --> node2.inPort[1];
        node1.inPort[1] <-- Channel <-- node2.outPort[1];

        source1.outPort --> node1.inPort[0];

}
