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
 * \description declaration of plane class
 * \date 2014-6-10
 * \modify 2014-6-11 10:33, ZHANG Shu-tao, add method Clear in AcPlane
 */
#ifndef ACPLANE_H_INCLUDED
#define ACPLANE_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include "AcMatrix.h"
#include "AcSigmoid.h"

#define CNN_CREATE  0
#define CNN_LOAD    1

class AcConnection;
class AcPlane;

/******************************************************************************/
//AcConnection
class AcConnection
{
public:
    //Constructor & Destructor
    //! Constructor
    AcConnection(AcPlane* pChild, AcSize sizeWeight);

    //! Destructor
    ~AcConnection();

    //! Load the connection from a configure file. if buildtype == CNN_LOAD, read the weights
    int Load(std::ifstream& infile, int buildtype);

    //! Store the connection into a configure file
    int Store(std::ofstream& outfile);

    //! Initialize the weights of this connection
    int InitWeights(double bound);

    //! Update the weights in this connection by dw
    int UpdateWeights(const AcMatrix& dw);

    //! Get the weight matrix of this connection
    AcMatrix GetWeights() const;

    //! Get the child plane
    AcPlane* GetChildPlane() const;

    //! connect the connection with parent plane
    int ConnectParent(AcPlane* pParent);

    //! Get the id of parent plane
    std::string GetParentId() const;

    //! Get the feature map of parent plane
    AcMatrix GetParentFeatureMap();

protected:
    AcMatrix m_Weights; //!< the weight matrix

    AcPlane* m_pParent; //!< a pointer to the parent plane
    AcPlane* m_pChild; //!< a pointer to the child plane
    std::string m_strParent; //! the id of parent plane
    std::string m_strChild; //!< the id of child plane
};

/******************************************************************************/
//generic class for planes
class AcPlane
{
public:
    //Constructor & Destructor
    //! Constructor
    AcPlane(AcSize sizeFeatureMap, AcSize sizeKernel);

    //!Destructor
    virtual ~AcPlane();

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
    */
    int Load(std::ifstream& infile, int buildtype);

    //! Store the plane from a configure file
    int Store(std::ofstream& outfile) const;

    //! Clear all connections in this plane
    int Clear();

    //! Initialize the weights in this plane
    int InitWeights();

    //! Get the id of this plane
    std::string GetId() const;

    //! Get the amount of connections to the lower layer
    int GetConnectionCount() const;

    //! Get a pointer to a connection to a lower plane
    AcConnection* GetConnection(int index);

    //! Connect to a child connection
    int ConnectChild(AcConnection* pChildConnection);

    //! Set the feature map of this plane
    int SetFeatureMap(const AcMatrix& FeatureMap);

    //! Get the feature map of this plane
    AcMatrix GetFeatureMap() const;

    //! Set the delta matrix of this plane
    int SetDelta(const AcMatrix& Delta);

    //! Get the delta matrix of this plane
    AcMatrix GetDelta() const;

    //! Forward propagate data through this plane
    virtual int ForwardPropagate() = 0;

    //! Back propagate error through this plane
    virtual int BackPropagate() = 0;

    //! Push the delta to lower plane
    virtual AcMatrix PushDelta(AcConnection* pConnection) const = 0;

    //! Update the weights of this plane
    virtual int UpdateWeights(double eta) = 0;

protected:
    std::string m_strId; //!< the id of this plane

    AcSize m_sizeFeatureMap; //!< the size of feature map
    AcSize m_sizeKernel; //!< the size of kernel
    AcSize m_sizeWeight; //!< the size of weight matrix in connections

    //weights and connections
    double m_Bias; //!< bias
    std::vector<AcConnection*> m_pParentConnection;
    std::vector<AcConnection*> m_pChildConnection;

    AcMatrix m_FeatureMap;
    AcMatrix m_Delta;
};

#endif // ACPLANE_H_INCLUDED
