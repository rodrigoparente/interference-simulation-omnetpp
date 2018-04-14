//
// Custom implementation of DimensionalAnalogModel to
// take into account the interference between different technologies
// that uses the 2.4GHz frequency to communicate
//
// 14/04/2018
//
//

#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/physicallayer/common/packetlevel/BandListening.h"
#include "inet/physicallayer/analogmodel/packetlevel/CustomDimensionalAnalogModel.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalTransmission.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalReception.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalNoise.h"
#include "inet/physicallayer/analogmodel/packetlevel/DimensionalSNIR.h"

namespace inet {

namespace physicallayer {

Define_Module(CustomDimensionalAnalogModel);

std::ostream& CustomDimensionalAnalogModel::printToStream(std::ostream& stream, int level) const
{
    stream << "CustomDimensionalAnalogModel";
    return CustomDimensionalAnalogModelBase::printToStream(stream, level);
}

const IReception *CustomDimensionalAnalogModel::computeReception(const IRadio *receiverRadio, const ITransmission *transmission, const IArrival *arrival) const
{
    const DimensionalTransmission *dimensionalTransmission = check_and_cast<const DimensionalTransmission *>(transmission);
    const simtime_t receptionStartTime = arrival->getStartTime();
    const simtime_t receptionEndTime = arrival->getEndTime();
    const Coord receptionStartPosition = arrival->getStartPosition();
    const Coord receptionEndPosition = arrival->getEndPosition();
    const EulerAngles receptionStartOrientation = arrival->getStartOrientation();
    const EulerAngles receptionEndOrientation = arrival->getEndOrientation();
    const ConstMapping *receptionPower = computeReceptionPower(receiverRadio, transmission, arrival);
    return new DimensionalReception(receiverRadio, transmission, receptionStartTime, receptionEndTime, receptionStartPosition, receptionEndPosition, receptionStartOrientation, receptionEndOrientation, dimensionalTransmission->getCarrierFrequency(), dimensionalTransmission->getBandwidth(), receptionPower);
}

} // namespace physicallayer

} // namespace inet

