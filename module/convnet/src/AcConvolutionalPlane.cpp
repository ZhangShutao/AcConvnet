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

/*!\file AcConvolutionalPlane.cpp
 * \author ZHANG Shu-tao
 * \description Implementation of convolutional plane class
 * \date 2014-6-11
 */

#include "AcConvolutionalPlane.h"
using namespace std;

AcConvolutionalPlane::AcConvolutionalPlane(AcSize sizeFeatureMap, AcSize sizeKernel) : AcPlane(sizeFeatureMap, sizeKernel)
{
    m_sizeWeight = sizeKernel;
}

AcConvolutionalPlane::~AcConvolutionalPlane()
{

}

int AcConvolutionalPlane::ForwardPropagate()
{
    m_FeatureMap.SetValue(m_Bias);
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        AcMatrix ParentFeature = m_pParentConnection[i]->GetParentFeatureMap();
        AcMatrix Weight = m_pParentConnection[i]->GetWeights();
        m_FeatureMap += ParentFeature.ConvValid(Weight);
    }
    m_FeatureMap = m_FeatureMap.Calcu(sigmoid);
    return 1;
}

int AcConvolutionalPlane::BackPropagate()
{
    m_Delta.SetValue(0);
    for (size_t i = 0; i != m_pChildConnection.size(); ++i)
    {
        AcMatrix ChildDelta = m_pChildConnection[i]->GetChildPlane()->PushDelta(m_pChildConnection[i]);
        m_Delta += ChildDelta;
    }
    m_Delta *= m_FeatureMap.Calcu(dsigmoid);
    /*
    for (int i = 0; i != m_sizeFeatureMap.height; ++i)
    {
        for (int j = 0; j != m_sizeFeatureMap.width; ++j)
        {
            cout<<m_Delta(i, j)<<" ";
        }
        cout<<endl;
    }*/
    return 1;
}

AcMatrix AcConvolutionalPlane::PushDelta(AcConnection* pConnection) const
{
    AcMatrix Kernel = pConnection->GetWeights();
    /*
    for (int i = 0; i != m_sizeWeight.height; ++i)
    {
        for (int j = 0; j != m_sizeWeight.width; ++j)
        {
            cout<<Kernel(i, j)<<" ";
        }
        cout<<endl;
    }*/
    return m_Delta.ConvFull(Kernel.Rot180());
}

//rot180(conv2(xi −1 , rot180(δ j ), valid )).
int AcConvolutionalPlane::UpdateWeights(double eta)
{
    //Update the bias;
    double db = 0;
    for (int i = 0; i != m_sizeFeatureMap.height; ++i)
    {
        for (int j = 0; j != m_sizeFeatureMap.width; ++j)
        {
            db += m_Delta(i, j);
        }
    }
    m_Bias += (-eta) * db;

    //Update weights in each connection
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        AcMatrix input = m_pParentConnection[i]->GetParentFeatureMap();
        AcMatrix dw = (input.ConvValid(m_Delta.Rot180())).Rot180();
        m_pParentConnection[i]->UpdateWeights(dw * (-eta));
    }
    return 1;
}
