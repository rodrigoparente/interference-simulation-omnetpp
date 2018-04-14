//
// Custom implementation of DimensionalAnalogModelBase to
// take into account the interference between different technologies
// that uses the 2.4GHz frequency to communicate
//
// 14/04/2018
//

#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/physicallayer/common/packetlevel/BandListening.h"
#include "inet/physicallayer/base/packetlevel/CustomDimensionalAnalogModelBase.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalReception.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalNoise.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalSNIR.h"

namespace inet {

namespace physicallayer {

void CustomDimensionalAnalogModelBase::initialize(int stage)
{
    AnalogModelBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        attenuateWithCarrierFrequency = par("attenuateWithCarrierFrequency");
        const char *interpolationModeString = par("interpolationMode");
        if (!strcmp("linear", interpolationModeString))
            interpolationMode = Mapping::LINEAR;
        else if (!strcmp("sample-hold", interpolationModeString))
            interpolationMode = Mapping::STEPS;
        else
            throw cRuntimeError("Unknown interpolation mode: '%s'", interpolationModeString);
    }
}

std::ostream& CustomDimensionalAnalogModelBase::printToStream(std::ostream& stream, int level) const
{
    if (level <= PRINT_LEVEL_TRACE)
        stream << ", attenuateWithCarrierFrequency = " << attenuateWithCarrierFrequency
               << ", interpolationMode = " << interpolationMode;
    return stream;
}

const ConstMapping *CustomDimensionalAnalogModelBase::computeReceptionPower(const IRadio *receiverRadio, const ITransmission *transmission, const IArrival *arrival) const
{
    const IRadioMedium *radioMedium = receiverRadio->getMedium();
    const IRadio *transmitterRadio = transmission->getTransmitter();
    const IAntenna *receiverAntenna = receiverRadio->getAntenna();
    const IAntenna *transmitterAntenna = transmitterRadio->getAntenna();
    const INarrowbandSignal *narrowbandSignalAnalogModel = check_and_cast<const INarrowbandSignal *>(transmission->getAnalogModel());
    const IDimensionalSignal *dimensionalSignalAnalogModel = check_and_cast<const IDimensionalSignal *>(transmission->getAnalogModel());
    const simtime_t transmissionStartTime = transmission->getStartTime();
    const simtime_t transmissionEndTime = transmission->getEndTime();
    const simtime_t receptionStartTime = arrival->getStartTime();
    const simtime_t receptionEndTime = arrival->getEndTime();
    const Coord receptionStartPosition = arrival->getStartPosition();
    const Coord receptionEndPosition = arrival->getEndPosition();
    const EulerAngles transmissionDirection = computeTransmissionDirection(transmission, arrival);
    const EulerAngles transmissionAntennaDirection = transmission->getStartOrientation() - transmissionDirection;
    const EulerAngles receptionAntennaDirection = transmissionDirection - arrival->getStartOrientation();
    double transmitterAntennaGain = transmitterAntenna->computeGain(transmissionAntennaDirection);
    double receiverAntennaGain = receiverAntenna->computeGain(receptionAntennaDirection);
    m distance = m(receptionStartPosition.distance(transmission->getStartPosition()));
    mps propagationSpeed = radioMedium->getPropagation()->getPropagationSpeed();
    const ConstMapping *transmissionPower = dimensionalSignalAnalogModel->getPower();
    EV_DEBUG << "Transmission power begin " << endl;
    transmissionPower->print(EVSTREAM);
    EV_DEBUG << "Transmission power end" << endl;
    const DimensionSet& dimensions = transmissionPower->getDimensionSet();
    bool hasTimeDimension = dimensions.hasDimension(Dimension::time);
    bool hasFrequencyDimension = dimensions.hasDimension(Dimension::frequency);
    ConstMappingIterator *it = transmissionPower->createConstIterator();
    Mapping *receptionPower = MappingUtils::createMapping(Argument::MappedZero, dimensions, interpolationMode);
    while (true) {
        const Argument& elementTransmissionPosition = it->getPosition();
        Hz carrierFrequency = attenuateWithCarrierFrequency || !hasFrequencyDimension ? narrowbandSignalAnalogModel->getCarrierFrequency() : Hz(elementTransmissionPosition.getArgValue(Dimension::frequency));
        double pathLoss = radioMedium->getPathLoss()->computePathLoss(propagationSpeed, carrierFrequency, distance);
        double obstacleLoss = radioMedium->getObstacleLoss() ? radioMedium->getObstacleLoss()->computeObstacleLoss(carrierFrequency, transmission->getStartPosition(), receptionStartPosition) : 1;
        W elementTransmissionPower = W(it->getValue());
        W elementReceptionPower = elementTransmissionPower * std::min(1.0, transmitterAntennaGain * receiverAntennaGain * pathLoss * obstacleLoss);
        Argument elementReceptionPosition = elementTransmissionPosition;
        if (hasTimeDimension) {
            if (elementTransmissionPosition.getTime() == transmissionStartTime)
                elementReceptionPosition.setTime(receptionStartTime);
            else if (elementTransmissionPosition.getTime() == transmissionEndTime)
                elementReceptionPosition.setTime(receptionEndTime);
            else {
                double alpha = (elementTransmissionPosition.getTime() - transmissionStartTime).dbl() / (transmissionEndTime - transmissionStartTime).dbl();
                simtime_t elementReceptionTime = (1 - alpha) * receptionStartTime.dbl() + alpha * receptionEndTime.dbl();
                elementReceptionPosition.setTime(elementReceptionTime);
            }
        }
        receptionPower->setValue(elementReceptionPosition, elementReceptionPower.get());
        if (it->hasNext())
            it->next();
        else
            break;
    }
    delete it;
    EV_DEBUG << "Reception power begin " << endl;
    receptionPower->print(EVSTREAM);
    EV_DEBUG << "Reception power end" << endl;
    return receptionPower;
}

const INoise *CustomDimensionalAnalogModelBase::computeNoise(const IListening *listening, const IInterference *interference) const
{
    const BandListening *bandListening = check_and_cast<const BandListening *>(listening);
    Hz carrierFrequency = bandListening->getCarrierFrequency();
    Hz bandwidth = bandListening->getBandwidth();
    std::vector<ConstMapping *> receptionPowers;
    const DimensionalNoise *dimensionalBackgroundNoise = dynamic_cast<const DimensionalNoise *>(interference->getBackgroundNoise());
    if (dimensionalBackgroundNoise) {
        const ConstMapping *backgroundNoisePower = dimensionalBackgroundNoise->getPower();
        if (backgroundNoisePower->getDimensionSet().hasDimension(Dimension::frequency) || (carrierFrequency == dimensionalBackgroundNoise->getCarrierFrequency() && bandwidth == dimensionalBackgroundNoise->getBandwidth()))
            receptionPowers.push_back(const_cast<ConstMapping *>(backgroundNoisePower));
    }
    const std::vector<const IReception *> *interferingReceptions = interference->getInterferingReceptions();
    for (const auto & interferingReception : *interferingReceptions) {
        const DimensionalReception *dimensionalReception = check_and_cast<const DimensionalReception *>(interferingReception);
        const ConstMapping *receptionPower = dimensionalReception->getPower();
        if (receptionPower->getDimensionSet().hasDimension(Dimension::frequency) || (carrierFrequency == dimensionalReception->getCarrierFrequency() && bandwidth == dimensionalReception->getBandwidth())) {
            receptionPowers.push_back(const_cast<ConstMapping *>(receptionPower));
            EV_DEBUG << "Interference power begin " << endl;
            dimensionalReception->getPower()->print(EVSTREAM);
            EV_DEBUG << "Interference power end" << endl;
        }
    }
    DimensionSet dimensions = receptionPowers[0]->getDimensionSet();
    if (!dimensions.hasDimension(Dimension::time))
        dimensions.addDimension(Dimension::time);
    ConstMapping *listeningMapping = MappingUtils::createMapping(Argument::MappedZero, dimensions, Mapping::STEPS);
    ConcatConstMapping<std::plus<double> > *noisePower = new ConcatConstMapping<std::plus<double> >(listeningMapping, receptionPowers.begin(), receptionPowers.end(), false, Argument::MappedZero);
    EV_TRACE << "Noise power begin " << endl;
    noisePower->print(EVSTREAM);
    EV_TRACE << "Noise power end" << endl;
    return new DimensionalNoise(listening->getStartTime(), listening->getEndTime(), carrierFrequency, bandwidth, noisePower);
}

const ISNIR *CustomDimensionalAnalogModelBase::computeSNIR(const IReception *reception, const INoise *noise) const
{
    const DimensionalReception *dimensionalReception = check_and_cast<const DimensionalReception *>(reception);
    const DimensionalNoise *dimensionalNoise = check_and_cast<const DimensionalNoise *>(noise);
    return new DimensionalSNIR(dimensionalReception, dimensionalNoise);
}

} // namespace physicallayer

} // namespace inet

