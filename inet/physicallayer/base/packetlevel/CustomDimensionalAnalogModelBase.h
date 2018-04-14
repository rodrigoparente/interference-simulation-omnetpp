//
// Custom implementation of DimensionalAnalogModelBase to
// take into account the interference between different technologies
// that uses the 2.4GHz frequency to communicate
//
// 14/04/2018
//

#ifndef __INET_CUSTOMDIMENSIONALANALOGMODELBASE_H
#define __INET_CUSTOMDIMENSIONALANALOGMODELBASE_H

#include "inet/common/mapping/MappingBase.h"
#include "inet/physicallayer/base/packetlevel/AnalogModelBase.h"

namespace inet {

namespace physicallayer {

class INET_API CustomDimensionalAnalogModelBase : public AnalogModelBase
{
  protected:
    bool attenuateWithCarrierFrequency;
    Mapping::InterpolationMethod interpolationMode;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual std::ostream& printToStream(std::ostream& stream, int level) const override;

    virtual const ConstMapping *computeReceptionPower(const IRadio *radio, const ITransmission *transmission, const IArrival *arrival) const;
    virtual const INoise *computeNoise(const IListening *listening, const IInterference *interference) const override;
    virtual const ISNIR *computeSNIR(const IReception *reception, const INoise *noise) const override;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_CUSTOMDIMENSIONALANALOGMODELBASE_H

