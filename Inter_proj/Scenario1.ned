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
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.common.misc.ThruputMeteringChannel;
import inet.transportlayer.tcp.flavours.Tcpemitscalar;

network Inter_proj_2
{
    @display("bgi=background/map_5cm_HHM41_20170720");
    int numHosts;
    types:
        channel ethernetline extends ThruputMeteringChannel
        {
            datarate = 100Mbps;
            thruputDisplayFormat = "B";
        }
    submodules:
        Tcpemit: Tcpemitscalar;
        accessPoint: AccessPoint {
            @display("p=160,121");
        }
        Turtle_Bot_51[numHosts]: WirelessHost {
            @display("p=83,169");
        }
        etherSwitch: EtherSwitch {
            @display("p=296,149");
        }
        Cloud: StandardHost {
            @display("p=347,49;i=device/server_l");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=105,15");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                assignDisjunctSubnetAddresses = false;
                @display("p=391,252;is=s");
        }
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            parameters:
                @display("p=26,252;is=s");
        }
    connections:
        accessPoint.ethg++ <--> ethernetline <--> etherSwitch.ethg++;
        Cloud.ethg++ <--> ethernetline <--> etherSwitch.ethg++;
}
