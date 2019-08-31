/*
 * Tcpemitscalar.h
 *
 *  Created on: 24-Oct-2018
 *      Author: KAUSHIK NAGARAJAN
 */

#ifndef INET_TRANSPORTLAYER_TCP_FLAVOURS_TCPEMITSCALAR_H_
#define INET_TRANSPORTLAYER_TCP_FLAVOURS_TCPEMITSCALAR_H_


#include <omnetpp.h>
using namespace omnetpp;

namespace inet {

namespace tcp {


class  Tcpemitscalar: public cSimpleModule
{

private:
    static simsignal_t EmitAckReceivedSignalId;
    static simsignal_t EmitSegDroppedSignalId;
public:
    int segments_rcvd = 0;
    int segments_dropped =0;
    double Tcp_segment_drop_rate =0;
    simtime_t Avg_RTT = 0;
    simtime_t Total_RTT =0;
    Tcpemitscalar();
    virtual ~Tcpemitscalar();
    void EmitAckReceived();
    void Segment_drops();
    void RTT_calc(simtime_t t);
    virtual void initialize()override;
    virtual void finish()override;

// Setting instance initially

    static Tcpemitscalar* GetSetInstance(Tcpemitscalar* val)
    {
        static Tcpemitscalar* instance;
        if (val == 0)
        {
            return instance;
        }
        instance =  val;
        return instance;
    }
};


}
}

#endif /* INET_TRANSPORTLAYER_TCP_FLAVOURS_TCPEMITSCALAR_H_ */
