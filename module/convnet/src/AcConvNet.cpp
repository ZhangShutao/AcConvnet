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

/*! \file AcConvNet.cpp
 * \author ZHANG Shu-tao
 * \description Implementation of class AcResult and AcConvNet. I've written a
    version before, but I think it's necessary to write it again. Holy beast
    bless me.
 * \date 2014-6-10
 */

#include <highgui.h>
#include "AcConvNet.h"
using namespace std;

/******************************************************************************/
//AcResult

//Constructor & Destructor
/*! Constructor
 */
AcResult::AcResult(int n/* = -1*/, double p/* = 0*/)
{
    label = n;
    likelihood = p;
}

AcResult::~AcResult()
{

}

AcResult& AcResult::operator = (const AcResult& b)
{
    label = b.label;
    likelihood = b.likelihood;
    return *this;
}

/******************************************************************************/
//AcConvNet

//Constructor & Destructor
//! Constructor
AcConvNet::AcConvNet()
{
    m_strId = string();
    m_pLayer = vector<AcLayer*>();
    m_dLearningRate = 0;
    m_sizeInput = AcSize();
}

//! Destructor
AcConvNet::~AcConvNet()
{
    for (size_t i = 0; i != m_pLayer.size(); ++i)
    {
        delete m_pLayer[i];
    }
}

//Methods
/*! Load network from a configure file
 * \param the name of configure file
 * \return status
 */
int AcConvNet::Load(string filename)
{
    ifstream infile(filename.c_str());
    string strBuildType;
    infile >> strBuildType;
    if (strBuildType == "create")
    {
        srand(time(0));
    }
    //Load the info of network
    int nLayers = 0;
    infile >> m_strId >> nLayers >> m_sizeInput.height >> m_sizeInput.width >> m_dLearningRate;

    //Load each layer in the network
    AcLayer* pPreLayer = NULL;
    int result = 0;
    for (int i = 0; i != nLayers; ++i)
    {
        AcLayer* pLayer = new AcLayer(pPreLayer);
        pPreLayer = pLayer;
        if (!(result = pLayer->Load(infile, strBuildType == "create" ? CNN_CREATE : CNN_LOAD)))
        {
            break;
        }
        m_pLayer.push_back(pLayer);
    }

    //error occurred
    if (!result)
    {
        Clear();
    }
    infile.close();
    return result;
}

/*! Store network to a configure file
 * \param the name of configure file
 * \return status
 */
int AcConvNet::Store(string filename) const
{
    ofstream outfile(filename.c_str());
    outfile << setprecision(12) << setiosflags(ios::scientific);
    outfile << "load" << endl << endl;
    outfile << m_strId <<" "<< m_pLayer.size() <<" "<< m_sizeInput.height <<" "<< m_sizeInput.width <<" "<< m_dLearningRate << endl << endl;

    int result = 0;
    for (size_t i = 0; i != m_pLayer.size(); ++i)
    {
        if (!(result = m_pLayer[i]->Store(outfile)))
        {
            break;
        }
    }
    outfile.close();
    return result;
}

//! Clear all layers in this network
int AcConvNet::Clear()
{
    for (size_t i = 0; i != m_pLayer.size(); ++i)
    {
        delete m_pLayer[i];
    }
    return 1;
}

/*! Classify an image
    Classify process:
    Classify(img)
    begin
        input = convert the image into a matrix
        sourceplane = m_pLayer[0]->GetPlane(0);
        sourceplane->SetFeatureMap(input)
        for each layer l in this network
            l->ForwardPropagate()
        end for

        res(-1, 0)
        for each plane p in the output layer
            if p->featuremap(0, 0) > res.likelihood
                update res
            end if
        end for
        return res
    end
 * \param a pointer to the image
 * \return classify result
 */
AcResult AcConvNet::Classify(IplImage* img)
{
    AcMatrix input(img->height, img->width);
    input.ConvImage(img);

    AcPlane* sourceplane = m_pLayer[0]->GetPlane(0);
    if (sourceplane == NULL)
    {
        return AcResult();
    }
    sourceplane->SetFeatureMap(input);
    ForwardPropagate();

    AcLayer* outlayer = m_pLayer[m_pLayer.size() - 1];
    int nPlane = outlayer->GetPlaneCount();
    AcResult res;
    for (int i = 0; i != nPlane; ++i)
    {
        AcPlane* pPlane = outlayer->GetPlane(i);
        double tmp = pPlane->GetFeatureMap()(0, 0);
        //cout<<tmp<<" ";
        if (tmp > res.likelihood)
        {
            res = AcResult(i, tmp);
        }
    }
    //cout<<endl;
    return res;
}

/*! Train the network with a pair of image and label
    Train process
    Train(image, label)
    begin
        Classify(image);
        BackPropagate(label)
        UpdateWeights()
    end
 * \param a pointer to the sample image
 * \param the label of sample
 * \return status
 */
int AcConvNet::Train(IplImage* img, int label)
{
    Classify(img);
    BackPropagate(label);
    UpdateWeight();
    return 1;
}

/*! Forward propagate data through the network
 * \return status
 */
int AcConvNet::ForwardPropagate()
{
    int result = 0;
    for (size_t i = 1; i != m_pLayer.size(); ++i)
    {
        if (!(result = m_pLayer[i]->ForwardPropagate()))
        {
            break;
        }
    }
    return result;
}

/*! Back propagate error through the network
 * \param the label of sample
 * \return status
 */
int AcConvNet::BackPropagate(int label)
{
    int result = 0;
    //set the delta of output layer
    AcLayer* pLayer = m_pLayer[m_pLayer.size() - 1];
    for (int i = 0; i != pLayer->GetPlaneCount(); ++i)
    {
        AcPlane* pPlane = pLayer->GetPlane(i);
        double out = pPlane->GetFeatureMap()(0, 0);
        double target = (label == i ? 1 : -1);
        double delta = dsigmoid(out) * (out - target);
        pPlane->SetDelta(AcMatrix(1, 1, delta));
    }
    //back propagate
    for (size_t i = m_pLayer.size() - 1; i != 0; --i)
    {
        if (!(result = m_pLayer[i - 1]->BackPropagate()))
        {
            break;
        }

    }
    return result;
}

/*! Update the weights of every layer
 * \return status
 */
int AcConvNet::UpdateWeight()
{
    int result = 0;
    for (size_t i = 0; i != m_pLayer.size(); ++i)
    {
        if (!(result = m_pLayer[i]->UpdateWeight(m_dLearningRate)))
        {
            break;
        }
    }
    return result;
}
