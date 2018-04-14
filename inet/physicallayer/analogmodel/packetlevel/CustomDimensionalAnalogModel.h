//
// Custom implementation of DimensionalAnalogModel to
// take into account the interference between different technologies
// that uses the 2.4GHz frequency to communicate
//
// 14/04/2018
//

#ifndef __INET_CUSTOMDIMENSIONALANALOGMODEL_H
#define __INET_CUSTOMDIMENSIONALANALOGMODEL_H

#include "inet/physicallayer/base/packetlevel/CustomDimensionalAnalogModelBase.h"
#include "inet/common/mapping/MappingBase.h"

namespace inet {

namespace physicallayer {

class INET_API CustomDimensionalAnalogModel : public CustomDimensionalAnalogModelBase
{
  public:
    virtual std::ostream& printToStream(std::ostream& stream, int level) const override;

    virtual const IReception *computeReception(const IRadio *radio, const ITransmission *transmission, const IArrival *arrival) const override;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_CUSTOMDIMENSIONALANALOGMODEL_H

