//
// Copyright (C) 2016 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see http://www.gnu.org/licenses/.
//
//

package inet.linklayer.ieee80211.mac.originator;

simple NonQosRecoveryProcedure
{
    parameters:
        string cwCalculatorModule;
        string rtsPolicyModule;
        
        int shortRetryLimit = default(7);
        int longRetryLimit = default(4);
        @display("i=block/cogwheel");
        @signal[LinklayerRetransmission](type=long);
        @signal[LinklayercontententionTime](type = simtime_t);
        @signal[LinklayercontentionFailedTime](type = simtime_t);  
        @statistic[LinklayercontententionTime](title="Contention Time"; source= LinklayercontententionTime; unit=s; record=vector; interpolationmode=none); 
        @statistic[LinklayercontententionTime_mean](title="Contention Time mean"; source= LinklayercontententionTime; unit=s; record=mean; interpolationmode=none); 
        @statistic[LinklayercontentionFailedTime](title="Contention failed"; source= LinklayercontentionFailedTime; unit=s; record=vector; interpolationmode=none);  
        @statistic[LinklayercontentionFailedTime_mean](title="Contention failed mean"; source= LinklayercontentionFailedTime; unit=s; record=mean; interpolationmode=none);  
        @statistic[Linklayer_Retransmission](title="Linklayer Retransmissions"; source= LinklayerRetransmission; record=vector,histogram; interpolationmode=none);
        @statistic[Linklayer_Retransmission_mean](title="Linklayer Retransmissions mean"; source= LinklayerRetransmission; record=mean; interpolationmode=none);
        
}
