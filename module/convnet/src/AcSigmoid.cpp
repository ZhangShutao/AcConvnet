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

/*! \file AcSigmoid.h
 * \author ZHANG Shu-tao
 * \description Implementation of sigmoid functions
 * \date 2014-6-11
 */

#include <cmath>
#include "AcSigmoid.h"
const double A = 1.71593428;
const double S = 0.66666666;

/*! The sigmoid function
 */
double sigmoid(double x)
{
    return A * tanh(S * x);
}

/*! The derivate sigmoid function
 * \param the output of sigmoid
 * \return the derivate value
 */
double dsigmoid(double o)
{
    return A * S * (1 - o / A) * (1 + o / A);
}
