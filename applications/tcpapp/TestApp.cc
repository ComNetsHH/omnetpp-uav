/*
 * TestApp.cc
 *
 *  Created on: 11-Sep-2018
 *   Based on TcpGenericServerApp.cc template
 *      Author: KAUSHIK NAGARAJAN
 */


#include "inet/applications/tcpapp/TestApp.h"

#include "inet/applications/common/SocketTag_m.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpCommand_m.h"

namespace inet {

Define_Module(TestApp);

void TestApp::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        delay = par("replyDelay");
        maxMsgDelay = 0;

        //statistics
        msgsRcvd = msgsSent = bytesRcvd = bytesSent = 0;

        WATCH(msgsRcvd);
        WATCH(msgsSent);
        WATCH(bytesRcvd);
        WATCH(bytesSent);
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        const char *localAddress = par("localAddress");
        int localPort = par("localPort");
        socket.setOutputGate(gate("socketOut"));
        socket.bind(localAddress[0] ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);
        socket.listen();

        bool isOperational;
        NodeStatus *nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;
        if (!isOperational)
            throw cRuntimeError("This module doesn't support starting in node DOWN state");
    }
}

void TestApp::sendOrSchedule(cMessage *msg, simtime_t delay)
{
    if (delay == 0)
        sendBack(msg);
    else
        scheduleAt(simTime() + delay, msg);
}

void TestApp::sendBack(cMessage *msg)
{
    Packet *packet = dynamic_cast<Packet *>(msg);

    if (packet) {
        msgsSent++;
        bytesSent += packet->getByteLength();
        emit(packetSentSignal, packet);

        EV_INFO << "sending \"" << packet->getName() << "\" to TCP, " << packet->getByteLength() << " bytes\n";
    }
    else {
        EV_INFO << "sending \"" << msg->getName() << "\" to TCP\n";
    }

    auto& tags = getTags(msg);
    tags.addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::tcp);
    send(msg, "socketOut");
}

/*
 * Send a response of 66 Bytes for very packet received.
 */


void TestApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
          sendBack(msg);
      }
      else if (msg->getKind() == TCP_I_PEER_CLOSED) {
          // we'll close too, but only after there's surely no message
          // pending to be sent back in this connection
          int connId = check_and_cast<Indication *>(msg)->getTag<SocketInd>()->getSocketId();
          delete msg;
          auto request = new Request("close", TCP_C_CLOSE);
          request->addTagIfAbsent<SocketReq>()->setSocketId(connId);
          sendOrSchedule(request, delay + maxMsgDelay);
      }
      else if (msg->getKind() == TCP_I_DATA || msg->getKind() == TCP_I_URGENT_DATA) {
          Packet *packet = check_and_cast<Packet *>(msg);
          int connId = packet->getTag<SocketInd>()->getSocketId();
          ChunkQueue &queue = socketQueue[connId];
          bool doClose = false;
          B size_pkt = packet->getTotalLength();
         Chunk::enableImplicitChunkSerialization = true;
          if(size_pkt == B(1448)){
              auto chunk = packet->peekDataAt(B(0), packet->getTotalLength());
              emit(packetReceivedSignal, packet);
              Packet *outPacket = new Packet(msg->getName());
              outPacket->addTagIfAbsent<SocketReq>()->setSocketId(connId);
              outPacket->setKind(TCP_C_SEND);
              const auto& payload = makeShared<GenericAppMsg>();
              payload->setChunkLength(B(66));
              payload->setExpectedReplyLength(B(0));
              payload->setReplyDelay(0);
              outPacket->insertAtBack(payload);
              sendOrSchedule(outPacket, delay);
          }

          else{
          auto chunk = packet->peekDataAt(B(0), packet->getTotalLength());
         emit(packetReceivedSignal, packet);
             msgsRcvd++;
                  Packet *outPacket = new Packet(msg->getName());
                  outPacket->addTagIfAbsent<SocketReq>()->setSocketId(connId);
                  outPacket->setKind(TCP_C_SEND);
                  const auto& payload = makeShared<GenericAppMsg>();
                  payload->setChunkLength(B(66));
                  payload->setExpectedReplyLength(B(0));
                  payload->setReplyDelay(0);
                  outPacket->insertAtBack(payload);
                  sendOrSchedule(outPacket, delay);
          delete msg;

          if (doClose) {
              auto request = new Request("close", TCP_C_CLOSE);
              TcpCommand *cmd = new TcpCommand();
              request->addTagIfAbsent<SocketReq>()->setSocketId(connId);
              request->setControlInfo(cmd);
              sendOrSchedule(request, delay + maxMsgDelay);
          }
      }
      }
      else if (msg->getKind() == TCP_I_AVAILABLE)
          socket.processMessage(msg);
      else {
          // some indication -- ignore
          EV_WARN << "drop msg: " << msg->getName() << ", kind:" << msg->getKind() << "(" << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind()) << ")\n";
          delete msg;
      }
}

void TestApp::refreshDisplay() const
{
    char buf[64];
    sprintf(buf, "rcvd: %ld pks %ld bytes\nsent: %ld pks %ld bytes", msgsRcvd, bytesRcvd, msgsSent, bytesSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void TestApp::finish()
{
    EV_INFO << getFullPath() << ": sent " << bytesSent << " bytes in " << msgsSent << " packets\n";
    EV_INFO << getFullPath() << ": received " << bytesRcvd << " bytes in " << msgsRcvd << " packets\n";
}

} // namespace inet




