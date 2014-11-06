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

/*! \file AcSubsamplingPlane.cpp
 * \author ZHANG Shu-tao
 * \description Implementation of subsampling plane
 * \date 2014-6-11
 */
#include "AcSubsamplingPlane.h"

//m_sizeWeight(1, 1)
AcSubsamplingPlane::AcSubsamplingPlane(AcSize sizeFeatureMap, AcSize sizeKernel) : AcPlane(sizeFeatureMap, sizeKernel)
{
    m_sizeWeight = AcSize(1, 1);
}

AcSubsamplingPlane::~AcSubsamplingPlane()
{

}

int AcSubsamplingPlane::ForwardPropagate()
{
    m_FeatureMap.SetValue(m_Bias);
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        AcMatrix ParentFeature = m_pParentConnection[i]->GetParentFeatureMap();
        double beta = m_pParentConnection[i]->GetWeights()(0, 0);
        m_FeatureMap += ParentFeature.DownSample(m_sizeKernel) * beta;
    }
    m_FeatureMap = m_FeatureMap.Calcu(sigmoid);
    return 1;
}

int AcSubsamplingPlane::BackPropagate()
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

AcMatrix AcSubsamplingPlane::PushDelta(AcConnection* pConnection) const
{
    double beta = pConnection->GetWeights()(0, 0);
    AcMatrix mat = m_Delta.UpSample(m_sizeKernel);
    return mat * beta;
}

int AcSubsamplingPlane::UpdateWeights(double eta)
{
    //Update the bias
    double db = 0;
    for (int i = 0; i != m_sizeFeatureMap.height; ++i)
    {
        for (int j = 0; j != m_sizeFeatureMap.width; ++j)
        {
            db += m_Delta(i, j);
        }
    }
    m_Bias += (-eta) * db;

    //Update the weight
    for (size_t k = 0; k != m_pParentConnection.size(); ++k)
    {
        double dw = 0;
        AcMatrix input = m_pParentConnection[k]->GetParentFeatureMap().DownSample(m_sizeKernel);
        for (int i = 0; i != m_sizeFeatureMap.height; ++i)
        {
            for (int j = 0; j != m_sizeFeatureMap.width; ++j)
            {
                dw += input(i, j) * m_Delta(i, j);
            }
        }
        m_pParentConnection[k]->UpdateWeights(AcMatrix(1, 1, (-eta) * dw));
    }
    return 1;
}
