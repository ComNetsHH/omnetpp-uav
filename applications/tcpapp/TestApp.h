/*
 * TestApp.h
 *
 *  Created on: 11-Sep-2018
 *  Based on TcpGenericServerApp.h template
 *      Author: KAUSHIK NAGARAJAN
 */

#ifndef INET_APPLICATIONS_TCPAPP_TESTAPP_H_
#define INET_APPLICATIONS_TCPAPP_TESTAPP_H_



#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/packet/ChunkQueue.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

namespace inet {

/**
 * Generic server application. It serves requests coming in GenericAppMsg
 * request messages. Clients are usually subclassed from TcpAppBase.
 *
 * @see GenericAppMsg, TcpAppBase
 */
class INET_API TestApp : public cSimpleModule, public ILifecycle
{
  protected:
    TcpSocket socket;
    simtime_t delay;
    simtime_t maxMsgDelay;

    long msgsRcvd;
    long msgsSent;
    long bytesRcvd;
    long bytesSent;

    std::map<int, ChunkQueue> socketQueue;

  protected:
    virtual void sendBack(cMessage *msg);
    virtual void sendOrSchedule(cMessage *msg, simtime_t delay);

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override
    { Enter_Method_Silent(); throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName()); return true; }
};

} // namespace inet



#endif /* INET_APPLICATIONS_TCPAPP_TESTAPP_H_ */
