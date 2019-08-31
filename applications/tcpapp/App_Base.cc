/*
 * App_Base.cc
 *
 *  Created on: 07-Sep-2018
 *  Based on TCPAppBase.cc template
 *      Author: KAUSHIK NAGARAJAN
 */


#include "inet/applications/tcpapp/App_Base.h"

#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

namespace inet {

simsignal_t App_Base::connectSignal = registerSignal("connect");
simsignal_t App_Base::rcvdPkSignal = registerSignal("rcvdPk");
simsignal_t App_Base::sentPkSignal = registerSignal("sentPk");
simsignal_t App_Base::RTTdelaySignal = registerSignal("RTTdelay");
simsignal_t App_Base::SendIntervalSignal = registerSignal("SendInterval");

void App_Base::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        numSessions = numBroken = packetsSent = packetsRcvd = bytesSent = bytesRcvd = 0;

        WATCH(numSessions);
        WATCH(numBroken);
        WATCH(packetsSent);
        WATCH(packetsRcvd);
        WATCH(bytesSent);
        WATCH(bytesRcvd);
        WATCH(Send_time);
        WATCH(Received_time);
        WATCH(RTT_delay);
        WATCH(Prev_Send_time);
        WATCH(Send_Interval);
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        // parameters
        const char *localAddress = par("localAddress");
        int localPort = par("localPort");
        socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

        socket.setCallback(this);
        socket.setOutputGate(gate("socketOut"));
    }
}

void App_Base::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        handleTimer(msg);
    else
        socket.processMessage(msg);
}

void App_Base::connect()
{
    // we need a new connId if this is not the first connection
    socket.renewSocket();

    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    // connect
    const char *connectAddress = par("connectAddress");
    int connectPort = par("connectPort");

    L3Address destination;
    L3AddressResolver().tryResolve(connectAddress, destination);
    if (destination.isUnspecified()) {
        EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address\n";
    }
    else {
        EV_INFO << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

        socket.connect(destination, connectPort);

        numSessions++;
        emit(connectSignal, 1L);
    }
}

void App_Base::close()
{
    EV_INFO << "issuing CLOSE command\n";
    socket.close();
    emit(connectSignal, -1L);
}

//send packet and emit send interval vectors
void App_Base::sendPacket(Packet *msg)
{
    int numBytes = msg->getByteLength();
    emit(packetSentSignal, msg);
    Send_time = simTime();
    socket.send(msg);
    Send_Interval = Send_time - Prev_Send_time;
    if(packetsSent >1){
    emit(SendIntervalSignal, Send_Interval);
    }
    Prev_Send_time = Send_time;
    packetsSent++;
    bytesSent += numBytes;
}

void App_Base::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, TcpSocket::stateName(socket.getState()));
}

void App_Base::socketEstablished(TcpSocket *)
{
    // *redefine* to perform or schedule first sending
    EV_INFO << "connected\n";
}

void App_Base::socketDataArrived(TcpSocket *, Packet *msg, bool)
{
    // *redefine* to perform or schedule next sending
    // calculate round trip time delay at application layer
    Received_time = simTime();
    RTT_delay = Received_time - Send_time;
    packetsRcvd++;
    emit(RTTdelaySignal,RTT_delay);
    bytesRcvd += msg->getByteLength();
    emit(packetReceivedSignal, msg);
    delete msg;
}

void App_Base::socketPeerClosed(TcpSocket *socket_)
{
    ASSERT(socket_ == &socket);
    // close the connection (if not already closed)
    if (socket.getState() == TcpSocket::PEER_CLOSED) {
        EV_INFO << "remote TCP closed, closing here as well\n";
        close();
    }
}

void App_Base::socketClosed(TcpSocket *)
{
    // *redefine* to start another session etc.
    EV_INFO << "connection closed\n";
}

void App_Base::socketFailure(TcpSocket *, int code)
{
    // subclasses may override this function, and add code try to reconnect after a delay.
    EV_WARN << "connection broken\n";
    numBroken++;
}

void App_Base::finish()
{
    std::string modulePath = getFullPath();

    EV_INFO << modulePath << ": opened " << numSessions << " sessions\n";
    EV_INFO << modulePath << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    EV_INFO << modulePath << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
}

} // namespace inet



