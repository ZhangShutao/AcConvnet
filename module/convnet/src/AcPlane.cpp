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

/*!\file AcPlane.h
 * \author ZHANG Shu-tao
 * \description Implementation of plane class
 * \date 2014-6-10
 */

#include "AcPlane.h"
using namespace std;

 /******************************************************************************/
//AcConnection

//Constructor & Destructor
//! Constructor
AcConnection::AcConnection(AcPlane* pChild, AcSize sizeWeight)
{
    m_Weights = AcMatrix(sizeWeight.height, sizeWeight.width);

    m_pParent = NULL;
    m_pChild = pChild;
    m_strParent = string();
    m_strChild = string();
}

//! Destructor
AcConnection::~AcConnection()
{

}

//! Load the connection from a configure file. if buildtype == CNN_LOAD, read the weights
int AcConnection::Load(std::ifstream& infile, int buildtype)
{
    infile >> m_strParent;
    if (buildtype == CNN_LOAD)
    {
        for (int i = 0; i != m_Weights.GetHeight(); ++i)
        {
            for (int j = 0; j != m_Weights.GetWidth(); ++j)
            {
                infile >> m_Weights(i, j);
            }
        }
    }
    return 1;
}

//! Store the connection into a configure file
int AcConnection::Store(std::ofstream& outfile)
{
    outfile << m_strParent << endl;
    for (int i = 0; i != m_Weights.GetHeight(); ++i)
    {
        for (int j = 0; j != m_Weights.GetWidth(); ++j)
        {
            outfile << m_Weights(i, j) << " ";
        }
        outfile << endl;
    }
    return 1;
}

/*! Initialize the weights of this connection
 * \param the bound of rand value, depends on the fan-in of this plane
 * \return status
 */
int AcConnection::InitWeights(double bound)
{
    m_Weights.SetRand(-bound, bound);
    return 1;
}

//! Update the weights in this connection by dw
int AcConnection::UpdateWeights(const AcMatrix& dw)
{
    //cout<<"connection updating"<<" "<<dw(0, 0)<<endl;
    m_Weights += dw;
    return 1;
}

//! Get the weight matrix of this connection
AcMatrix AcConnection::GetWeights() const
{
    return m_Weights;
}

//! Get a pointer to the child plane
AcPlane* AcConnection::GetChildPlane() const
{
    return m_pChild;
}

//! connect the connection with parent plane
int AcConnection::ConnectParent(AcPlane* pParent)
{
    if (pParent->GetId() != m_strParent)
    {
        return 0;
    }
    m_pParent = pParent;
    return pParent->ConnectChild(this);
}

//! Get the id of parent plane
string AcConnection::GetParentId() const
{
    return m_strParent;
}

//! Get the feature map of parent plane
AcMatrix AcConnection::GetParentFeatureMap()
{
    return m_pParent->GetFeatureMap();
}

/******************************************************************************/
//generic class for planes

//Constructor & Destructor
//! Constructor
AcPlane::AcPlane(AcSize sizeFeatureMap, AcSize sizeKernel)
{
    m_strId = string();

    m_sizeFeatureMap = sizeFeatureMap;
    m_sizeKernel = sizeKernel;

    //weights and connections
    m_Bias = 0;
    m_pParentConnection = vector<AcConnection*> ();
    m_pChildConnection = vector<AcConnection*> ();

    m_FeatureMap = AcMatrix(sizeFeatureMap.height, sizeFeatureMap.width);
    m_Delta = AcMatrix(sizeFeatureMap.height, sizeFeatureMap.width);
}

//!Destructor
AcPlane::~AcPlane()
{
    Clear();
}

/*! Load process
    Load(infile, buildtype)
    begin
        m_strId = read id
        nConnect = read connection amount
        if buildtype == CNN_LOAD
            read bias
        for each connection
            AcConnection* p = new AcConnection(this, m_sizeWeight);
            p->Load(infile, buildtype);
            m_pParentConnection.push_back(p)
        end for
    end
 * \param the input file stream
 * \param the type of build method
 * \return status
 */
int AcPlane::Load(ifstream& infile, int buildtype)
{
    int nConnection = 0;
    infile >> m_strId >> nConnection;

    //load weights and connections
    if (buildtype == CNN_LOAD && nConnection != 0)
    {
        infile >> m_Bias;
    }
    //load all connection
    int result = 1; // a plane may have no connection, such as a source plane
    for (int i = 0; i != nConnection; ++i)
    {
        AcConnection *pConnection = new AcConnection(this, m_sizeWeight);
        if (!(result = pConnection->Load(infile, buildtype)))
        {
            break;
        }
        m_pParentConnection.push_back(pConnection);
    }
    return result;
}

//! Store the plane from a configure file
int AcPlane::Store(ofstream& outfile) const
{
    outfile << m_strId <<" "<< m_pParentConnection.size()<<endl;
    if (m_pParentConnection.size() != 0)
    {
        outfile << m_Bias << endl;
    }

    int result = 0;
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        if (!(result = m_pParentConnection[i]->Store(outfile)))
        {
            break;
        }
    }
    return result;
}

//! Clear all connections of this plane
int AcPlane::Clear()
{
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        delete m_pParentConnection[i];
    }
    return 1;
}

//! Initialize the weights in this plane
int AcPlane::InitWeights()
{
    if (m_pParentConnection.size() == 0)
    {
        return 1;
    }
    int fan = 1 + m_pParentConnection.size() * m_sizeWeight.height * m_sizeWeight.width;
    double bound = 2.4 / fan;

    //initialize bias
    m_Bias = bound * 2 * rand() / RAND_MAX - bound;

    //initialize connection
    int result = 0;
    for (size_t i = 0; i != m_pParentConnection.size(); ++i)
    {
        if (!(result = m_pParentConnection[i]->InitWeights(bound)))
        {
            break;
        }
    }
    return 1;
}

//! Get the id of this plane
string AcPlane::GetId() const
{
    return m_strId;
}

//! Get the amount of connections to the lower layer
int AcPlane::GetConnectionCount() const
{
    return m_pParentConnection.size();
}

//! Get a pointer to a connection to a lower plane
AcConnection* AcPlane::GetConnection(int index)
{
    return m_pParentConnection[index];
}

//! Connect to a child connection
int AcPlane::ConnectChild(AcConnection* pChildConnection)
{
    m_pChildConnection.push_back(pChildConnection);
    return 1;
}

//! Set the feature map of this plane
int AcPlane::SetFeatureMap(const AcMatrix& FeatureMap)
{
    assert(m_sizeFeatureMap == FeatureMap.GetSize());
    m_FeatureMap = FeatureMap;
    return 1;
}

//! Get the feature map of this plane
AcMatrix AcPlane::GetFeatureMap() const
{
    return m_FeatureMap;
}

//! Set the delta matrix of this plane
int AcPlane::SetDelta(const AcMatrix& Delta)
{
    assert(m_sizeFeatureMap == Delta.GetSize());
    m_Delta = Delta;
    return 1;
}

//! Get the delta matrix of this plane
AcMatrix AcPlane::GetDelta() const
{
    return m_Delta;
}
