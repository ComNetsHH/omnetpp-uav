/*
 * Tcpemitscalar.cc
 *
 *  Created on: 24-Oct-2018
 *      Author: KAUSHIK NAGARAJAN
 *      This module gets Round trip time values from TcpBaseAlg module and
 *      calculates average round trip time and segment latency rates
 */





#include "inet/transportlayer/tcp/flavours/Tcpemitscalar.h"

namespace inet {

namespace tcp {


Define_Module(Tcpemitscalar);

simsignal_t Tcpemitscalar::EmitAckReceivedSignalId = registerSignal("AckReceivedSignal");
simsignal_t Tcpemitscalar::EmitSegDroppedSignalId = registerSignal("SegDroppedSignal");

Tcpemitscalar::Tcpemitscalar()
{

}

void Tcpemitscalar::initialize()
{
    GetSetInstance(this);
}

void Tcpemitscalar::finish()
{
    Tcp_segment_drop_rate = ((double)segments_dropped/(double)segments_rcvd)*100;
    recordScalar("Segment Drop Rate",Tcp_segment_drop_rate);
    Avg_RTT = Total_RTT /(double)segments_rcvd;
    recordScalar("Average TCP RTT",Avg_RTT);

}

Tcpemitscalar::~Tcpemitscalar()
{

}

void Tcpemitscalar::EmitAckReceived()
{
    emit(EmitAckReceivedSignalId,1L);
    segments_rcvd = segments_rcvd +1;
}

void Tcpemitscalar::RTT_calc(simtime_t t)
{
simtime_t current_RTT = t;
Total_RTT = Total_RTT + current_RTT;

}


void Tcpemitscalar::Segment_drops()
{
    emit(EmitSegDroppedSignalId,1L);
    segments_dropped = segments_dropped +1;
}

}
}
