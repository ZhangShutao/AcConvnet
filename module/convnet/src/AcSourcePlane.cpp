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

/*!\file AcSourcePlane.cpp
 * \author ZHANG Shu-tao
 * \description Implementation of source plane class
 * \date 2014-6-11
 */

#include "AcSourcePlane.h"
using namespace std;

AcSourcePlane::AcSourcePlane(AcSize sizeFeatureMap, AcSize sizeKernel) : AcPlane(sizeFeatureMap, sizeKernel)
{
    m_sizeWeight = AcSize(0, 0);
}

AcSourcePlane::~AcSourcePlane()
{

}

int AcSourcePlane::ForwardPropagate()
{
    return 1;
}

int AcSourcePlane::BackPropagate()
{
    return 1;
}

AcMatrix AcSourcePlane::PushDelta(AcConnection* pConnection) const
{
    return AcMatrix();
}

int AcSourcePlane::UpdateWeights(double eta)
{
    return 1;
}
