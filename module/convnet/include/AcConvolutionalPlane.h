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
#ifndef ACCONVOLUTIONALPLANE_H_INCLUDED
#define ACCONVOLUTIONALPLANE_H_INCLUDED

/*!\file AcConvolutionalPlane.h
 * \author ZHANG Shu-tao
 * \description declaration of convolutional plane class
 * \date 2014-6-11
 */

#include "AcPlane.h"

class AcConvolutionalPlane : public AcPlane
{
public:
    //Constructor & Destructor

    //! Constructor, m_sizeWeight(sizeKernel);
    AcConvolutionalPlane(AcSize sizeFeatureMap, AcSize sizeKernel);

    //! Destructor
    virtual ~AcConvolutionalPlane();

    virtual int ForwardPropagate();

    virtual int BackPropagate();

    virtual AcMatrix PushDelta(AcConnection* pConnection) const;

    virtual int UpdateWeights(double eta);
};

#endif // ACCONVOLUTIONALPLANE_H_INCLUDED
