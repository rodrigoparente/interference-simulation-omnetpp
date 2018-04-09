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

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

network wirelessnetwork
{
    @display("bgb=650,500;bgg=100,1,grey95");
    @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

    @figure[rcvdPkText](type=indicatorText; pos=360,20; anchor=w; font=,18; textFormat="packets received by wifi: %g"; initialValue=0);
    @statistic[rcvdPk](source=hostWifiB.udpApp[0].rcvdPk; record=figure(count); targetFigure=rcvdPkText);

//    @figure[rcvdPkText](type=indicatorText; pos=360,40; anchor=w; font=,18; textFormat="packets received by zigbee: %g"; initialValue=0);
//    @statistic[rcvdPk](source=hostZigbeeB.udpApp[0].rcvdPk; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=580,125");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: Ieee80211DimensionalRadioMedium {
            @display("p=580,275");
        }
        hostWifiA: AdhocHost {
            @display("p=61.488003,284.256;i=device/wifilaptop");
        }
        hostWifiB: AdhocHost {
            @display("p=188.496,83.664;i=device/wifilaptop");
        }
        hostWifiC: AdhocHost {

            @display("p=60.480003,133.056;i=device/wifilaptop");
        }
        hostZigbeeA: AdhocHost {
            @display("p=449.56802,287.28;i=device/palm2");
        }
        hostZigbeeB: AdhocHost {
            @display("p=207.64801,284.256;i=device/palm2");
        }
}
