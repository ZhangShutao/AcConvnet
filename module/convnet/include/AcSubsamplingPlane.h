/****************************************************************************|
|           ┏┓   ┏┓                                                      |
|          ┏┛┻━━━┛┻┓ Code is far away from bug with holy beast protecting.   |
|          ┃   ━   ┃ 神兽护佑，代码无bug                                       |
|          ┃ ┳┛ ┗┳ ┃                                                     |
|          ┃       ┃                                                     |
|          ┃   ┻   ┃                                                     |
|          ┗━┓   ┏━┛                                                     |
|            ┃   ┃                                                       |
|            ┃   ┃                                                       |
|            ┃   ┗━━━━━━┓                                                |
|            ┃          ┣┓                                               |
|            ┃          ┏┛                                               |
|            ┗┓┓┏━━━━┳┓┏┛                                                |
|             ┃┫┫    ┃┫┫                                                 |
|             ┗┻┛    ┗┻┛                                                 |
|****************************************************************************/

/*! \file AcSubsamplingPlane.h
 * \author ZHANG Shu-tao
 * \description Declaration of subsampling plane
 * \date 2014-6-11
 */

#ifndef ACSUBSAMPLINGPLANE_H_INCLUDED
#define ACSUBSAMPLINGPLANE_H_INCLUDED

#include "AcPlane.h"

class AcSubsamplingPlane : public AcPlane
{
public:
    //m_sizeWeight(1, 1)
    AcSubsamplingPlane(AcSize sizeFeatureMap, AcSize sizeKernel);
    virtual ~AcSubsamplingPlane();

    virtual int ForwardPropagate();
    virtual int BackPropagate();
    virtual AcMatrix PushDelta(AcConnection* pConnection) const;
    virtual int UpdateWeights(double eta);
};

#endif // ACSUBSAMPLINGPLANE_H_INCLUDED
