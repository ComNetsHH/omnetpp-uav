/*
 * App_Base.h
 *
 *  Created on: 07-Sep-2018
 *  Based on TCPAppBase.h template
 *      Author: KAUSHIK NAGARAJAN
 */

#ifndef INET_APPLICATIONS_TCPAPP_APP_BASE_H_
#define INET_APPLICATIONS_TCPAPP_APP_BASE_H_


#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

namespace inet {

/**
 * Base class for clients app for TCP-based request-reply protocols or apps.
 * Handles a single session (and TCP connection) at a time.
 *
 * It needs the following NED parameters: localAddress, localPort, connectAddress, connectPort.
 */
class INET_API App_Base : public cSimpleModule, public TcpSocket::ICallback
{
  protected:
    TcpSocket socket;

    // statistics
    int numSessions;
    int numBroken;
    int packetsSent;
    int packetsRcvd;
    int bytesSent;
    int bytesRcvd;
    simtime_t Send_Interval;
    simtime_t Prev_Send_time;
    simtime_t Send_time;
    simtime_t Received_time;
    simtime_t RTT_delay;

    //statistics:
    static simsignal_t connectSignal;
    static simsignal_t rcvdPkSignal;
    static simsignal_t sentPkSignal;
    static simsignal_t RTTdelaySignal;
    static simsignal_t SendIntervalSignal;

  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    /* Utility functions */
    virtual void connect();
    virtual void close();
    virtual void sendPacket(Packet *pkt);

    virtual void handleTimer(cMessage *msg) = 0;

    /* TcpSocket::ICallback callback methods */
    virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;
    virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override { socket->accept(availableInfo->getNewSocketId()); }
    virtual void socketEstablished(TcpSocket *socket) override;
    virtual void socketPeerClosed(TcpSocket *socket) override;
    virtual void socketClosed(TcpSocket *socket) override;
    virtual void socketFailure(TcpSocket *socket, int code) override;
    virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override { }
    virtual void socketDeleted(TcpSocket *socket) override {}
};

} // namespace inet




#endif /* INET_APPLICATIONS_TCPAPP_APP_BASE_H_ */
