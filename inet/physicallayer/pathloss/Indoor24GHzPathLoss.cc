#include "Indoor24GHzPathLoss.h"

#include <ext/type_traits.h>
#include <omnetpp/checkandcast.h>
#include <omnetpp/cobjectfactory.h>
#include <omnetpp/regmacros.h>
#include <iostream>

#include "../../common/geometry/common/Coord.h"
#include "../../common/InitStages.h"
#include "../../common/INETMath.h"
#include "../../common/Units.h"
#include "../contract/packetlevel/IArrival.h"
#include "../contract/packetlevel/IPrintableObject.h"
#include "../contract/packetlevel/IPropagation.h"
#include "../contract/packetlevel/IRadio.h"
#include "../contract/packetlevel/IRadioMedium.h"
#include "../contract/packetlevel/IRadioSignal.h"
#include "../contract/packetlevel/ITransmission.h"

namespace inet {

namespace physicallayer {

Define_Module(Indoor24GHzPathLoss);

Indoor24GHzPathLoss::Indoor24GHzPathLoss()
{
    //Do nothing
}

void Indoor24GHzPathLoss::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        //Do nothing
    }
}

std::ostream& Indoor24GHzPathLoss::printToStream(std::ostream& stream, int level) const
{
    stream << "Indoor24GHzPathLoss";
    if (level <= PRINT_LEVEL_TRACE) {
        stream << ", ALO MUNDO! ";
    }
    return stream;
}


double Indoor24GHzPathLoss::computePathLoss(const ITransmission* transmission, const IArrival* arrival) const
{
    auto radioMedium = transmission->getTransmitter()->getMedium();
    auto narrowbandSignalAnalogModel = check_and_cast<const INarrowbandSignal *>(transmission->getAnalogModel());
    mps propagationSpeed = radioMedium->getPropagation()->getPropagationSpeed();
    Hz carrierFrequency = Hz(2400);
    const m waveLength = propagationSpeed / carrierFrequency;

    return computeInterference(transmission->getStartPosition(), arrival->getStartPosition());
}

double Indoor24GHzPathLoss::computeInterference(const Coord& pos_t, const Coord& pos_r) const
{
    const m distance { pos_r.distance(pos_t) };
    m ponto1 = m(0.5);
    m ponto2 = m(8.0);

    if (distance > ponto2)
        return 58.2 + 33 * log10(distance.get() / 8.0);
    else if (distance > ponto1) {
        return 40.2 + 20 * log10(distance.get());
    }
    return 1.0;
}

double Indoor24GHzPathLoss::computePathLoss(mps propagationSpeed, Hz frequency, m distance) const
{
	return NaN;
}

m Indoor24GHzPathLoss::computeRange(mps propagationSpeed, Hz frequency, double loss) const
{
    return m(NaN);
}

} // namespace physicallayer

} // namespace inet
