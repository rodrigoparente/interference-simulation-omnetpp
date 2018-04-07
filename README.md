# Interference Simulation Using OMNeT++

Simulating an WHN (*Wirelesss Home Network*) that contains devices of the following technologies: 802.11, 802.15.4 and 802.15.1.

# TO-DO

We are using Ieee80211DimensionalRadiuMedium, which uses the *DimensionalAnalogModel* that inherits its characteristics from *DimensionalAnalogModelBase*.

/inet/src/physicallayer/base/packetlevel/DimensionalAnalogModelBase.cc
/inet/src/physicallayer/base/packetlevel/DimensionalAnalogModelBase.h

```C++
const ConstMapping *DimensionalAnalogModelBase::computeReceptionPower(...)
{
	[...]
	double pathLoss = radioMedium->getPathLoss()->computePathLoss(propagationSpeed, carrierFrequency, distance);
	W elementReceptionPower = elementTransmissionPower * std::min(1.0, transmitterAntennaGain * receiverAntennaGain * pathLoss * obstacleLoss);
	[...]
}
```

Obs.: The pathloss model that we implemented has two *computePathLoss()*. The one that is used, we didn't implemented.

We need to find were the interference is calculated. Maybe is in the same *DimensionalModelAnalogModelBase* file. Probably in the following function:

```C++
const INoise *DimensionalAnalogModelBase::computeNoise(...)
{
	[...]
	double pathLoss = radioMedium->getPathLoss()->computePathLoss(propagationSpeed, carrierFrequency, distance);
	W elementReceptionPower = elementTransmissionPower * std::min(1.0, transmitterAntennaGain * receiverAntennaGain * pathLoss * obstacleLoss);
	[...]
}
```