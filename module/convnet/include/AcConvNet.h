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

/*! \file AcConvNet.h
 * \author ZHANG Shu-tao
 * \description declaration of class AcResult and AcConvNet
 * \date 2014-6-10
 * \modify 2014-6-11-10:07, ZHANG Shu-tao, add method Clear
 */

#ifndef ACCONVNET_H_INCLUDED
#define ACCONVNET_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include <cv.h>
#include "AcLayer.h"

/******************************************************************************/
//AcResult
class AcResult
{
public:
    AcResult(int n = -1, double p = -2);
    ~AcResult();

    AcResult& operator = (const AcResult& b);
    int label;
    double likelihood;
};

/******************************************************************************/
//AcConvNet
class AcConvNet
{
public:
    //Constructor & Destructor
    //! Constructor
    AcConvNet();

    //! Destructor
    ~AcConvNet();

    //Methods
    //! Load network from a configure file
    int Load(std::string filename);

    //! Store network to a configure file
    int Store(std::string filename) const;

    //! Clear all layers in this network
    int Clear();

    //! Classify an image
    AcResult Classify(IplImage* img);

    //! Train the network with a pair of image and label
    int Train(IplImage* img, int label);
protected:
    //! Forward propagate data through the network
    int ForwardPropagate();

    //! Back propagate error through the network
    int BackPropagate(int label);

    //! Update the weights of every layer
    int UpdateWeight();
protected:
    std::string m_strId; //!< the id of this network

    std::vector<AcLayer*> m_pLayer; //!< pointers to layers
    double m_dLearningRate; //! learning rate of weight updating

    AcSize m_sizeInput; //!< the size of input
};

#endif // ACCONVNET_H_INCLUDED
