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

/*! \file AcNormalPlane.h
 * \author ZHANG Shu-tao
 * \description Declaration of normal plane
 * \date 2014-6-11
 */

#ifndef ACNORMALPLANE_H_INCLUDED
#define ACNORMALPLANE_H_INCLUDED

#include "AcPlane.h"

//this plane can be seen as a common neuron in MLP
class AcNormalPlane : public AcPlane
{
public:
    //m_sizeWeight(1, 1)
    AcNormalPlane(AcSize sizeFeatureMap, AcSize sizeKernel);
    virtual ~AcNormalPlane();

    virtual int ForwardPropagate();
    virtual int BackPropagate();
    virtual AcMatrix PushDelta(AcConnection* pConnection) const;
    virtual int UpdateWeights(double eta);
};

#endif // ACNORMALPLANE_H_INCLUDED
