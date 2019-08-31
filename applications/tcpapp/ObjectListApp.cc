/*
 * ObjectListApp.cc
 *
 *  Created on: 07-Sep-2018
 *  Based on TCPBasicClientApp.cc template
 *      Author: KAUSHIK NAGARAJAN
 */




#include "inet/applications/tcpapp/ObjectListApp.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"

namespace inet {

#define MSGKIND_CONNECT    0
#define MSGKIND_SEND       1

Define_Module(ObjectListApp);

simsignal_t ObjectListApp::NumberOfTrackedObjectsSignal = registerSignal("TrackedObjects");
simsignal_t ObjectListApp::ObjectListPayloadSignal = registerSignal("ObjectListPayload");
simsignal_t ObjectListApp::ObjectListSISignal = registerSignal("ObjectListSI");

ObjectListApp::~ObjectListApp()
{
    cancelAndDelete(timeoutMsg);
}

void ObjectListApp::initialize(int stage)
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

bool ObjectListApp::isNodeUp()
{
    return !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
}

bool ObjectListApp::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
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
 * Send Object list messages from the start of simulation
 * Number of objects are chosen based on which payload is chosen
 * Set timer for next message generation
 */

void ObjectListApp::sendRequest()
{
    int payload_1;
        int Num_Objects = intuniform(0,100);
        long tracked_obj = 0;
        if(Num_Objects >=0 && Num_Objects <9)
        {
            payload_1 = 147;
            tracked_obj = 0;
            emit(NumberOfTrackedObjectsSignal, tracked_obj);
            emit(ObjectListPayloadSignal, payload_1);
        }
        else if (Num_Objects >=9 && Num_Objects <61)
        {
            payload_1 = 2092+ intuniform(0,27);
            tracked_obj = 1;
            emit(NumberOfTrackedObjectsSignal, tracked_obj);
            emit(ObjectListPayloadSignal, payload_1);
        }
        else if (Num_Objects >=61 && Num_Objects <89)
        {
            payload_1 = 4037+ intuniform(0,27);
            tracked_obj = 2;
            emit(NumberOfTrackedObjectsSignal, tracked_obj);
            emit(ObjectListPayloadSignal, payload_1);
        }
        else if (Num_Objects >=89 && Num_Objects <96)
        {
            payload_1 = 5982+ intuniform(0,27);
            tracked_obj = 3;
            emit(NumberOfTrackedObjectsSignal, tracked_obj);
            emit(ObjectListPayloadSignal, payload_1);
        }
        else
        {
            payload_1 = 7927 + intuniform(0,27);
            tracked_obj = 4;
            emit(NumberOfTrackedObjectsSignal, tracked_obj);
            emit(ObjectListPayloadSignal, payload_1);
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
    int random_pick = intuniform(0,100);
    if(random_pick > 92)
    {
    simtime_t d_1 = (simtime_t)normal(1.1263,0.0017);
    simtime_t d = simTime() + d_1;
    emit(ObjectListSISignal,d_1);
    rescheduleOrDeleteTimer(d, MSGKIND_SEND);

    }
    else
    {
        simtime_t d_1 = (simtime_t)par("thinkTime");
        simtime_t d = simTime() + d_1;
        emit(ObjectListSISignal,d_1);
        rescheduleOrDeleteTimer(d, MSGKIND_SEND);
    }
}

void ObjectListApp::handleTimer(cMessage *msg)
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

void ObjectListApp::socketEstablished(TcpSocket *socket)
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

void ObjectListApp::rescheduleOrDeleteTimer(simtime_t d, short int msgKind)
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

void ObjectListApp::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
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

void ObjectListApp::socketClosed(TcpSocket *socket)
{
    App_Base::socketClosed(socket);

    // start another session after a delay
    if (timeoutMsg) {
        simtime_t d = simTime() + par("idleInterval");
        rescheduleOrDeleteTimer(d, MSGKIND_CONNECT);
    }
}

void ObjectListApp::socketFailure(TcpSocket *socket, int code)
{
    App_Base::socketFailure(socket, code);

    // reconnect after a delay
    if (timeoutMsg) {
        simtime_t d = simTime() + par("reconnectInterval");
        rescheduleOrDeleteTimer(d, MSGKIND_CONNECT);
    }
}

} // namespace inet

