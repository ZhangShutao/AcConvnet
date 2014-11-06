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
 * \description Implement of normal plane
 * \date 2014-6-11
 */

 #include "AcNormalPlane.h"

//m_sizeWeight(1, 1)
AcNormalPlane::AcNormalPlane(AcSize sizeFeatureMap, AcSize sizeKernel) : AcPlane(sizeFeatureMap, sizeKernel)
{
    m_sizeWeight = AcSize(1, 1);
}

AcNormalPlane::~AcNormalPlane()
{

}

int AcNormalPlane::ForwardPropagate()
{
    m_FeatureMap.SetValue(m_Bias);
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        double x = m_pParentConnection[i]->GetParentFeatureMap()(0, 0);
        double weight = m_pParentConnection[i]->GetWeights()(0, 0);
        m_FeatureMap += AcMatrix(1, 1, x * weight);
    }
    m_FeatureMap = m_FeatureMap.Calcu(sigmoid);
    return 1;
}

int AcNormalPlane::BackPropagate()
{
    m_Delta.SetValue(0);
    for (size_t i = 0; i != m_pChildConnection.size(); ++i)
    {
        AcMatrix ChildDelta = m_pChildConnection[i]->GetChildPlane()->PushDelta(m_pChildConnection[i]);
        m_Delta += ChildDelta;
    }
    m_Delta *= m_FeatureMap.Calcu(dsigmoid);
    return 1;
}

AcMatrix AcNormalPlane::PushDelta(AcConnection* pConnection) const
{
    double weight = pConnection->GetWeights()(0, 0);
    double delta = m_Delta(0, 0);
    return AcMatrix(1, 1, weight * delta);
}

int AcNormalPlane::UpdateWeights(double eta)
{
    double delta = m_Delta(0, 0);
    m_Bias += (-eta) * delta;

    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        double input = m_pParentConnection[i]->GetParentFeatureMap()(0, 0);
        double dw = input * delta;
        m_pParentConnection[i]->UpdateWeights(AcMatrix(1, 1, (-eta) * dw));
    }
    return 1;
}
