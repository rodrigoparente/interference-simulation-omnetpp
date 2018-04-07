#ifndef __INET_BREAKPOINTPATHLOSS_H
#define __INET_BREAKPOINTPATHLOSS_H

#include "inet/physicallayer/base/packetlevel/PathLossBase.h"

namespace inet {

namespace physicallayer {

/**
 * Implementation of a breakpoint path loss model.
 */
class INET_API Indoor24GHzPathLoss : public PathLossBase
{
  protected:
    virtual void initialize(int stage) override;

  public:
	Indoor24GHzPathLoss();
	virtual std::ostream& printToStream(std::ostream& stream, int level) const override;
	virtual double computePathLoss(const ITransmission* transmission, const IArrival* arrival) const override;
	virtual double computeInterference(const Coord& pos_t, const Coord& pos_r) const;
    virtual double computePathLoss(mps propagationSpeed, Hz frequency, m distance) const override;
    virtual m computeRange(mps propagationSpeed, Hz frequency, double loss) const override;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_BREAKPOINTPATHLOSS_H
