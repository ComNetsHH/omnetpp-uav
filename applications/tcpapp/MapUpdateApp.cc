/*
 * MapUpdateApp.cc
 *
 *  Created on: 08-Sep-2018
 *  Based on TCPBasicClientApp.cc template
 *      Author: KAUSHIK NAGARAJAN
 */


#include "inet/applications/tcpapp/MapUpdateApp.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"

namespace inet {

#define MSGKIND_CONNECT    0
#define MSGKIND_SEND       1

Define_Module(MapUpdateApp);

simsignal_t MapUpdateApp::NumberPixelsSignal = registerSignal("NumberOfPixels");
simsignal_t MapUpdateApp::MUPayloadSignal = registerSignal("MUPayload");
simsignal_t MapUpdateApp::MUSISignal = registerSignal("MUSI");

MapUpdateApp::~MapUpdateApp()
{
    cancelAndDelete(timeoutMsg);
}

void MapUpdateApp::initialize(int stage)
{
    App_Base::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        numRequestsToSend = 0;
        earlySend = false;    // TBD make it parameter
        WATCH(numRequestsToSend);
        WATCH(earlySend);

        startTime = par("startTime");
        stopTime = par("stopTime");
        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            throw cRuntimeError("Invalid startTime/stopTime parameters");
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        timeoutMsg = new cMessage("timer");
        nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));

        if (isNodeUp()) {
            timeoutMsg->setKind(MSGKIND_CONNECT);
            scheduleAt(startTime, timeoutMsg);
        }
    }
}

bool MapUpdateApp::isNodeUp()
{
    return !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
}

bool MapUpdateApp::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();
    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if (static_cast<NodeStartOperation::Stage>(stage) == NodeStartOperation::STAGE_APPLICATION_LAYER) {
            simtime_t now = simTime();
            simtime_t start = std::max(startTime, now);
            if (timeoutMsg && ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))) {
                timeoutMsg->setKind(MSGKIND_CONNECT);
                scheduleAt(start, timeoutMsg);
            }
        }
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) {
        if (static_cast<NodeShutdownOperation::Stage>(stage) == NodeShutdownOperation::STAGE_APPLICATION_LAYER) {
            cancelEvent(timeoutMsg);
            if (socket.getState() == TcpSocket::CONNECTED || socket.getState() == TcpSocket::CONNECTING || socket.getState() == TcpSocket::PEER_CLOSED)
                close();
            // TODO: wait until socket is closed
        }
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation)) {
        if (static_cast<NodeCrashOperation::Stage>(stage) == NodeCrashOperation::STAGE_CRASH)
            cancelEvent(timeoutMsg);
    }
    else
        throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName());
    return true;
}

/*
 * Send Map update messages from the start of simulation
 * Number of pixels are chosen based on which payload is chosen
 * Set timer for next message generation
 */
void MapUpdateApp::sendRequest()
{
    int payload_1;
    int Num_Objects = intuniform(0,1000);
    long Num_pixels =0;

    if(Num_Objects >=0 && Num_Objects <=20)
    {
        payload_1 = 319+ intuniform(0,2);
        Num_pixels =1;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=21 && Num_Objects <=88)
    {
        payload_1 = 432 + intuniform(0,4);
        Num_pixels =2;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=89 && Num_Objects <=188)
    {
        payload_1 = 545+ intuniform(0,6);
        Num_pixels =3;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=189 && Num_Objects <=313)
    {
        payload_1 = 658 + intuniform(0,8);
        Num_pixels =4;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=314 && Num_Objects <=435)
    {
        payload_1 = 771 + intuniform(0,10);
        Num_pixels =5;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=436 && Num_Objects <=557)
    {
        payload_1 = 884 + intuniform(0,12);
        Num_pixels =6;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=558 && Num_Objects <=667)
    {
        payload_1 = 997 + intuniform(0,14);
        Num_pixels =7;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=668 && Num_Objects <=752)
    {
        payload_1 = 1110 + intuniform(0,16);
        Num_pixels =8;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=753 && Num_Objects <=822)
    {
        payload_1 = 1223 + intuniform(0,18);
        Num_pixels =9;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=823 && Num_Objects <=863)
    {
        payload_1 = 1336 + intuniform(0,20);
        Num_pixels =10;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=864 && Num_Objects <=913)
    {
        payload_1 = 1449 + intuniform(0,22);
        Num_pixels =11;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=914 && Num_Objects <=935)
    {
        payload_1 = 1562 + intuniform(0,24);
        Num_pixels =12;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=936 && Num_Objects <=955)
    {
        payload_1 = 1675 + intuniform(0,26);
        Num_pixels =13;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=956 && Num_Objects <=969)
    {
        payload_1 = 1788 + intuniform(0,28);
        Num_pixels =14;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=970 && Num_Objects <=977)
    {
        payload_1 = 1901 + intuniform(0,30);
        Num_pixels =15;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=978 && Num_Objects <=984)
    {
        payload_1 = 2014 + intuniform(0,32);
        Num_pixels =16;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=985 && Num_Objects <=991)
    {
        payload_1 = 2127 + intuniform(0,34);
        Num_pixels =17;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=992 && Num_Objects <=993)
    {
        payload_1 = 2240 + intuniform(0,36);
        Num_pixels =18;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=994 && Num_Objects <=995)
    {
        payload_1 = 2353 + intuniform(0,38);
        Num_pixels =19;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects == 996)
    {
        payload_1 = 2466 + intuniform(0,40);
        Num_pixels =20;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects >=997 && Num_Objects <=998)
    {
        payload_1 = 2579 + intuniform(0,42);
        Num_pixels =21;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else if (Num_Objects == 999)
    {
        payload_1 = 2692 + intuniform(0,44);
        Num_pixels =22;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }
    else
    {
        payload_1 = 2805 + intuniform(0,46);
        Num_pixels =23;
        emit(NumberPixelsSignal, Num_pixels);
        emit(MUPayloadSignal, payload_1);
    }

    long requestLength = payload_1;
    long replyLength = par("replyLength");
    if (requestLength < 1)
        requestLength = 1;
    if (replyLength < 1)
        replyLength = 1;

    const auto& payload = makeShared<GenericAppMsg>();
    Packet *packet = new Packet("data");
    payload->setChunkLength(B(requestLength));
    payload->setExpectedReplyLength(B(replyLength));
    payload->setServerClose(false);
    packet->insertAtBack(payload);

    EV_INFO << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
            << "remaining " << numRequestsToSend - 1 << " request\n";

    sendPacket(packet);

    int random_pick = intuniform(0,98);
    if(random_pick >= 0 && random_pick < 82)
    {
        simtime_t d_1 = (simtime_t)normal(1.0453,0.0098);
        simtime_t d = simTime() + d_1;
        rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        emit(MUSISignal,d_1);
    }
    else if(random_pick >= 82 && random_pick < 94)
    {
        simtime_t d_1 = (simtime_t)normal(1.1251,0.0103);
        simtime_t d = simTime() + d_1;
        rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        emit(MUSISignal,d_1);
    }
    else
    {
        simtime_t d_1 = (simtime_t)normal(1.2066,0.0073);
    simtime_t d = simTime() + d_1;
    rescheduleOrDeleteTimer(d, MSGKIND_SEND);
    emit(MUSISignal,d_1);
    }
}

void MapUpdateApp::handleTimer(cMessage *msg)
{
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            connect();    // active OPEN

            // significance of earlySend: if true, data will be sent already
            // in the ACK of SYN, otherwise only in a separate packet (but still
            // immediately)
            if (earlySend)
                sendRequest();
            break;

        case MSGKIND_SEND:
            sendRequest();
            numRequestsToSend--;
            // no scheduleAt(): next request will be sent when reply to this one
            // arrives (see socketDataArrived())
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void MapUpdateApp::socketEstablished(TcpSocket *socket)
{
    App_Base::socketEstablished(socket);

    // determine number of requests in this session
    numRequestsToSend = par("numRequestsPerSession");
    if (numRequestsToSend < 1)
        numRequestsToSend = 1;

    // perform first request if not already done (next one will be sent when reply arrives)
    if (!earlySend)
        sendRequest();

    numRequestsToSend--;
}

void MapUpdateApp::rescheduleOrDeleteTimer(simtime_t d, short int msgKind)
{
    cancelEvent(timeoutMsg);

    if (stopTime < SIMTIME_ZERO || d < stopTime) {
        timeoutMsg->setKind(msgKind);
        scheduleAt(d, timeoutMsg);
    }
    else {
        delete timeoutMsg;
        timeoutMsg = nullptr;
    }
}

void MapUpdateApp::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
    App_Base::socketDataArrived(socket, msg, urgent);

    if (numRequestsToSend > 0) {
        EV_INFO << "reply arrived\n";

        if (timeoutMsg) {
            simtime_t d = simTime() + par("thinkTime");
            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        }
    }
    else if (socket->getState() != TcpSocket::LOCALLY_CLOSED) {
        EV_INFO << "reply to last request arrived, closing session\n";
        close();
    }
}

void MapUpdateApp::socketClosed(TcpSocket *socket)
{
    App_Base::socketClosed(socket);

    // start another session after a delay
    if (timeoutMsg) {
        simtime_t d = simTime() + par("idleInterval");
        rescheduleOrDeleteTimer(d, MSGKIND_CONNECT);
    }
}

void MapUpdateApp::socketFailure(TcpSocket *socket, int code)
{
    App_Base::socketFailure(socket, code);

    // reconnect after a delay
    if (timeoutMsg) {
        simtime_t d = simTime() + par("reconnectInterval");
        rescheduleOrDeleteTimer(d, MSGKIND_CONNECT);
    }
}

} // namespace inet



