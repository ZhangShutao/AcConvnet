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

/*!\file AcLayer.h
 * \author ZHANG Shu-tao
 * \description declaration of layer class
 * \date 2014-6-10
 */

#include "AcSourcePlane.h"
#include "AcConvolutionalPlane.h"
#include "AcSubsamplingPlane.h"
#include "AcNormalPlane.h"
#include "AcLayer.h"
using namespace std;

//Constructor & Destructor
/*! Constructor
 \param a pointer to previous layer, NULL for source layer
 */
AcLayer::AcLayer(AcLayer* pPreLayer/* = NULL*/)
{
    m_strId = string();

    m_pPlane = vector<AcPlane*>();
    m_pPreLayer = pPreLayer;

    m_sizeFeatureMap = AcSize();
    m_sizeKernel = AcSize();
}

//! Destructor
AcLayer::~AcLayer()
{
    Clear();
}

/*! Load each plane in this layer and connect to previous layer
    Load process:
    Load(infile, buildtype)
    begin
        input layer type, layer name, size of feature map, size of kernel, amount of plane
        for each plane
            p = new a plane with different layer type
            p->Load(infile, buildtype)
            ConnectPlane(p, buildtype)
        end for
    end
 * \param reference of input file
 * \param the type of build, create or load
 * \return status
 */
int AcLayer::Load(std::ifstream& infile, int buildtype)
{
    int nPlanes = 0;
    infile >> m_strType >> m_strId >> m_sizeFeatureMap.height >> m_sizeFeatureMap.width;
    infile >> m_sizeKernel.height >> m_sizeKernel.width >> nPlanes;


    //Load each plane in this layer
    int result = 0;
    for (int i = 0; i != nPlanes; ++i)
    {
        AcPlane* pPlane = NewPlane();
        if (!(result = (pPlane->Load(infile, buildtype) && ConnectPlane(pPlane, buildtype))))
        {
            break;
        }
        m_pPlane.push_back(pPlane);
    }
    //error occurred
    if (!result)
    {
        Clear();
    }
    return result;
}

/*! Store this layer to a configure file
 * \param the reference of output file
 * \return status
 */
int AcLayer::Store(std::ofstream& outfile) const
{
    outfile << m_strType <<" "<< m_strId <<" "<< m_sizeFeatureMap.height <<" "<< m_sizeFeatureMap.width <<" ";
    outfile << m_sizeKernel.height <<" "<< m_sizeKernel.width <<" "<< m_pPlane.size() << endl;

    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        m_pPlane[i]->Store(outfile);
    }
    outfile << endl;
    return 1;
}

//! Clear all planes in this layer
int AcLayer::Clear()
{
    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        delete m_pPlane[i];
    }
    return 1;
}

/*! Connect a plane in this layer with ones in previous layer.
        Connection process:
        ConnectPlane(i, buildtype)
        begin
            if buildtype == CNN_CREATE
                i->InitWeights();
            end if
            for each connection j in this plane
                p = m_pPreLayer->GetPlane(j->GetParentId());
                j->ConnectParent(p);
            end for
        end
 * \param pointer of some plane
 * \return status
 */
int AcLayer::ConnectPlane(AcPlane* pPlane, int buildtype)
{
    if (buildtype == CNN_CREATE)
    {
        pPlane->InitWeights();
    }
    int result = 1; // a plane may have no connection
    for (int i = 0; i != pPlane->GetConnectionCount(); ++i)
    {
        AcConnection* pConnection = pPlane->GetConnection(i);
        AcPlane* pParentPlane = m_pPreLayer->GetPlane(pConnection->GetParentId());
        if (!(result = pConnection->ConnectParent(pParentPlane)))
        {
            break;
        }
    }
    return result;
}

/*! Get the amount of planes in this layer
 * \return status
 */
int AcLayer::GetPlaneCount() const
{
    return m_pPlane.size();
}

/*! New a plane with the type of layer
 * \return pointer to newed plane
 */
AcPlane* AcLayer::NewPlane() const
{
    if (m_strType == "src")
    {
        return new AcSourcePlane(m_sizeFeatureMap, m_sizeKernel);
    }
    else if (m_strType == "conv")
    {
        return new AcConvolutionalPlane(m_sizeFeatureMap, m_sizeKernel);
    }
    else if (m_strType == "sub")
    {
        return new AcSubsamplingPlane(m_sizeFeatureMap, m_sizeKernel);
    }
    else if (m_strType == "normal")
    {
        return new AcNormalPlane(m_sizeFeatureMap, m_sizeKernel);
    }
    return NULL;
}

/*! Get a pointer to some plane with index
 * \param the index
 * \return pointer
 */
AcPlane* AcLayer::GetPlane(int index) const
{
    return m_pPlane[index];
}

/*! Get a pointer to some plane with id string
 * \param the id of plane
 * \return pointer
 */
AcPlane* AcLayer::GetPlane(string strId) const
{
    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        if (m_pPlane[i]->GetId() == strId)
        {
            return m_pPlane[i];
        }
    }
    return NULL;
}

/*! Forward propagate data through this layer
 * \return status
 */
int AcLayer::ForwardPropagate()
{
    int result = 0;
    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        if (!(result = m_pPlane[i]->ForwardPropagate()))
        {
            break;
        }
    }
    return result;
}

/*! Back propagate error through this layer
 * \return status
 */
int AcLayer::BackPropagate()
{
    int result = 0;
    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        if (!(result = m_pPlane[i]->BackPropagate()))
        {
            break;
        }
    }
    return result;
}

/*! Update the weights in this layer
 * \param the learning rate
 * \return status
 */
int AcLayer::UpdateWeight(double eta)
{
    int result = 0;
    for (size_t i = 0; i != m_pPlane.size(); ++i)
    {
        if (!(result = m_pPlane[i]->UpdateWeights(eta)))
        {
            break;
        }
    }
    return result;
}
