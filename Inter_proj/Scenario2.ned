//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//




import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.wireless.AccessPoint;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.physicallayer.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.common.misc.ThruputMeteringChannel;
import inet.transportlayer.tcp.flavours.Tcpemitscalar;


network Inter_proj_3
{
    @display("bgb=531,306");
    int numHosts;
    types:
        channel ethernetline extends ThruputMeteringChannel
        {
            datarate = 100Mbps;
            thruputDisplayFormat = "B";
        }
    submodules:
        Tcpemit: Tcpemitscalar;
        accessPoint1: AccessPoint {
            @display("p=28,44");
        }
        accessPoint2: AccessPoint {
            @display("p=14003,252");
        }
        Turtle_Bot_51[numHosts]: WirelessHost {
            @display("p=28,100");
        }
        Turtle_Bot_52[numHosts]: WirelessHost {
            @display("p=14075,271");
        }
        etherSwitch: EtherSwitch {
            @display("p=237,77");
        }
        Cloud: StandardHost {
            @display("p=150,0;i=device/server_l");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=86,48");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                assignDisjunctSubnetAddresses = false;
                @display("p=100,100;is=s");
        }
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            parameters:
                @display("p=99,249;is=s");
        }
    connections:

        accessPoint2.ethg++ <--> ethernetline <--> etherSwitch.ethg++;
        accessPoint1.ethg++ <--> ethernetline <--> etherSwitch.ethg++;
        Cloud.ethg++ <--> ethernetline <--> etherSwitch.ethg++;
}
