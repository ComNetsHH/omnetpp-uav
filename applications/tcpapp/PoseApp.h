/*
 * PoseApp.h
 *
 *  Created on: 07-Sep-2018
 *  Based on TCPBasicClientApp.h
 *      Author: KAUSHIK NAGARAJAN
 */

#ifndef INET_APPLICATIONS_TCPAPP_POSEAPP_H_
#define INET_APPLICATIONS_TCPAPP_POSEAPP_H_

#include "inet/common/INETDefs.h"

#include "inet/applications/tcpapp/App_Base.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/lifecycle/ILifecycle.h"

namespace inet {

/**
 * An example request-reply based client application.
 */
class INET_API PoseApp : public App_Base, public ILifecycle
{
  protected:
    cMessage *timeoutMsg = nullptr;
    NodeStatus *nodeStatus = nullptr;
    bool earlySend = false;    // if true, don't wait with sendRequest() until established()
    int numRequestsToSend = 0;    // requests to send in this session
    simtime_t startTime;
    simtime_t stopTime;
    simtime_t e;
    //needed statistics recording
    static simsignal_t PosePayloadSignal;
    static simsignal_t PoseSISignal;

    virtual void sendRequest();
    virtual void rescheduleOrDeleteTimer(simtime_t d, short int msgKind);

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleTimer(cMessage *msg) override;
    virtual void socketEstablished(TcpSocket *socket) override;
    virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;
    virtual void socketClosed(TcpSocket *socket) override;
    virtual void socketFailure(TcpSocket *socket, int code) override;
    virtual bool isNodeUp();
    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override;

  public:
    PoseApp() {}
    virtual ~PoseApp();
};

} // namespace inet




#endif /* INET_APPLICATIONS_TCPAPP_POSEAPP_H_ */
