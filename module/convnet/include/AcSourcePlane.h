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

/*!\file AcSourcePlane.h
 * \author ZHANG Shu-tao
 * \description declaration of source plane class
 * \date 2014-6-11
 */
#ifndef ACSOURCEPLANE_H_INCLUDED
#define ACSOURCEPLANE_H_INCLUDED

#include "AcPlane.h"

class AcSourcePlane : public AcPlane
{
public:
    //Constructor & Destructor

    //! Constructor, m_sizeWeight(0, 0);
    AcSourcePlane(AcSize sizeFeatureMap, AcSize sizeKernel);

    //! Destructor
    virtual ~AcSourcePlane();

    virtual int ForwardPropagate();

    virtual int BackPropagate();

    virtual AcMatrix PushDelta(AcConnection* pConnection) const;

    virtual int UpdateWeights(double eta);
};

#endif // ACSOURCEPLANE_H_INCLUDED
