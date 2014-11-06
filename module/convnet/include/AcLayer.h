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
 * \modify 2014-6-11 10:12, ZHANG Shu-tao, add method Clear
 * \modify 2-14-6-11 10:19, ZHANG Shu-tao, add method NewPlane
 */

#ifndef ACLAYER_H_INCLUDED
#define ACLAYER_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include "AcPlane.h"

/*! The class is the layers in the network. It can be seen as a container of
 * planes with same type.
 */
class AcLayer
{
public:
    //Constructor & Destructor
    //! Constructor
    AcLayer(AcLayer* pPreLayer = NULL);

    //! Destructor
    ~AcLayer();

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
    */
    int Load(std::ifstream& infile, int buildtype);

    //! Store this layer to a configure file
    int Store(std::ofstream& outfile) const;

    //! Clear all planes in this layer
    int Clear();

    /*! Connect a plane in this layer with ones in previous layer.
        Connection process:
        ConnectPlane(i, buildtype)
        begin
            if buildtype == CNN_CREATE
                i->InitWeights();
            end if
            for each connection j in this plane
                p = m_pPreLayer->GetPlane(j->GetParentId());
                i->ConnectParent(p);
            end for
        end
     */
    int ConnectPlane(AcPlane* pPlane, int buildtype);

    //! Get the amount of planes in this layer
    int GetPlaneCount() const;

    //! New a plane with the type of layer
    AcPlane* NewPlane() const;

    //! Get a pointer to some plane with index
    AcPlane* GetPlane(int index) const;

    //! Get a pointer to some plane with id string
    AcPlane* GetPlane(std::string strId) const;

    //! Forward propagate data through this layer
    int ForwardPropagate();

    //! Back propagate error through this layer
    int BackPropagate();

    //! Update the weights in this layer
    int UpdateWeight(double eta);

protected:
    std::string m_strId; //!< the string of id
    std::string m_strType; //!< the type of this layer

    std::vector<AcPlane*> m_pPlane; //!< pointers to planes in this layer
    AcLayer* m_pPreLayer; //!< a pointer to previous layer

    AcSize m_sizeFeatureMap; //!< the size of feature map
    AcSize m_sizeKernel; //!< the size of kernel. Note that kernel's size is nothing to do with weight size.
};


#endif // ACLAYER_H_INCLUDED
