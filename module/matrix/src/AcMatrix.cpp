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

/**
 * Create a double matrix without STL.
 */

/*!\file AcMatrix.h
 * \author ZHANG Shu-tao
 * \description an implementation of double-type matrix class
 * \date 2014-6-3
 * \modify 2014-6-6 Added class AcSize, ZHANG Shu-tao
 * \modify 2014-6-6 modified AcMatrix, added some methods, ZHANG Shu-tao
 * \modify 2014-6-10 modifying up-sample and down-sample.
 */

#include <cstdlib>
#include <cassert>
#include <ctime>
#include "AcMatrix.h"

/******************************************************************************/
//! AcSize
//Constructor & Destructor
/*! Constructor
 * \param the height
 * \param the width
 */
AcSize::AcSize(int h/* = 0*/, int w/*= 0*/)
{
    height = h;
    width = w;
}

/*! Copy Constructor
 * \param another AcSize object
 */
AcSize::AcSize(const AcSize& b)
{
    height = b.height;
    width = b.width;
}

//! Destructor
AcSize::~AcSize()
{

}

bool AcSize::operator==(const AcSize& b) const
{
    return (height == b.height) && (width == b.width);
}

bool AcSize::operator!=(const AcSize& b) const
{
    return !(*this == b);
}

AcSize& AcSize::operator=(const AcSize& b)
{
    height = b.height;
    width = b.width;
    return *this;
}

/******************************************************************************/
//! AcMatrix
//Constructor & Destructor

/*! Constructor
 * \param the height of this matrix
 * \param the width of this matrix
 * \param the default value of each element in this matrix
 */
AcMatrix::AcMatrix(int height/* = 0*/, int width/* = 0*/, double val/* = 0*/)
{
    m_data = NULL;
    if (!Create(height, width, val))
    {
        exit(1);
    }
}

/*! Copy Constructor
 * \param the source matrix
 */
AcMatrix::AcMatrix(const AcMatrix& mat)
{
    m_height = mat.m_height;
    m_width = mat.m_width;
    m_data = new double[m_height * m_width];
    for (int i = 0; i != m_height * m_width; ++i)
    {
        m_data[i] = mat.m_data[i];
    }
}

/*! Destructor
 */
AcMatrix::~AcMatrix()
{
    if (!Release())
    {
        exit(1);
    }
}

//Methods

/*! The method creates the matrix with these parameters.
 * \param the height of this matrix
 * \param the width of this matrix
 * \param the value for every element in this matrix
 * \return status
 */
int AcMatrix::Create(int height/* = 0*/, int width/* = 0*/, double val/* = 0*/)
{
    assert(height >= 0 && width >= 0);
    //We need to ensure the matrix is new or released
    assert(m_data == NULL);

    if (height == 0 || width == 0)
    {
        m_data = NULL;
    }
    else
    {
        m_height = height;
        m_width = width;
        m_data = new double[height * width];
        for (int i = 0; i != height * width; ++i)
        {
            m_data[i] = val;
        }
    }
    return 1;
}

/*! The method release the data in this matrix and reset the member data in this
 * matrix. The pointer will print to NULL, and the height and width reset as 0.
 * \return status
 */
int AcMatrix::Release()
{
    if (m_data != NULL)
    {
        delete [] m_data;
        m_data = NULL;

        m_height = 0;
        m_width = 0;
    }
    return 1;
}

/*! The method return the height of this matrix
 * \return the height of matrix
 */
int AcMatrix::GetHeight() const
{
    return m_height;
}

/*! The method return the width of this matrix
 * \return the width of this matrix
 */
int AcMatrix::GetWidth() const
{
    return m_width;
}

/*! The method return the size of this matrix
 * \return the size of this matrix
 */
AcSize AcMatrix::GetSize() const
{
    return AcSize(m_height, m_width);
}

/*! The method randomly set the value of each element with a value between left
 * and right value.
 * \param min-value of random
 * \param max-value of random
 * \return status
 */
int AcMatrix::SetRand(double left, double right)
{
    for (int i = 0; i != m_height * m_width; ++i)
    {
        m_data[i] = rand() * (right - left) / RAND_MAX + left;
    }
    return 1;
}

/*! The method set all element in this matrix as val
 * \param the value to be set
 * \return status
 */
int AcMatrix::SetValue(double val)
{
    for (int i = 0; i != m_height * m_width; ++i)
    {
        m_data[i] = val;
    }
    return 1;
}

/*! The method calculate every element in the matrix with pfunc
 * \param a pointer to the call back function that calculates every element.
 * \return a matrix containing the result.
 */
AcMatrix AcMatrix::Calcu(MATRIX_FUNC pfunc)
{
    AcMatrix ret(m_height, m_width);
    for (int i = 0; i != m_height * m_width; ++i)
    {
        ret.m_data[i] = pfunc(m_data[i]);
    }
    return ret;
}

/*! The method calculate the convolution of this matrix with a kernel. Note
 * that it works like conv2d(a, kernel, 'valid') in MatLab. That means it will
 * not flip the kernel.
 * \param the kernel of convolution
 * \return the convolution result
 */
AcMatrix AcMatrix::ConvValid(const AcMatrix& mat) const
{
    assert(m_height >= mat.m_height && m_width >= mat.m_width);
    AcMatrix res(m_height - mat.m_height + 1, m_width - mat.m_width + 1, 0);
    for (int x = 0; x != res.m_height; ++x)
    {
        for (int y = 0; y != res.m_width; ++y)
        {
            for (int i = 0; i != mat.m_height; ++i)
            {
                for (int j = 0; j != mat.m_width; ++j)
                {
                    res(x, y) += (*this)(x + i, y + j) * mat(mat.m_height - i - 1, mat.m_width - j - 1);
                }
            }
        }
    }
    return res;
}

/*! The method calculate the convolution of this matrix with a kernel. Note
 * that it works like conv2d(a, kernel, 'full') in MatLab. That means it will
 * not flip the kernel.
 * \param the kernel of convolution
 * \return the convolution result
 */
AcMatrix AcMatrix::ConvFull(const AcMatrix& mat) const
{
    AcMatrix res(m_height + mat.m_height - 1, m_width + mat.m_width - 1, 0);
    for (int x = 0; x != res.m_height; ++x)
    {
        for (int y = 0; y != res.m_width; ++y)
        {
            for (int i = 0; i != mat.m_height; ++i)
            {
                for (int j = 0; j != mat.m_width; ++j)
                {
                    int tx = x + i - mat.m_height + 1, ty = y + j - mat.m_width + 1;
                    if (tx < 0 || ty < 0 || tx >= m_height || ty >= m_width)
                    {
                        continue;
                    }
                    res(x, y) += (*this)(tx, ty) * mat(mat.m_height - i - 1, mat.m_width - j - 1);
                }
            }
        }
    }
    return res;
}

/*! The method do sub-sampling operation on this matrix with a kernel by
 * weight summing elements in a patch of matrix with elements in kernel
 * \param kernel matrix
 * \return sub-sampled matrix
 * \date 2014-6-6
 */
AcMatrix AcMatrix::DownSample(const AcSize& scale) const
{
    assert(m_height % scale.height == 0 && m_width % scale.width == 0);

    AcMatrix res(m_height / scale.height, m_width / scale.width, 0);
    for (int i = 0; i != m_height; ++i)
    {
        for (int j = 0; j != m_width; ++j)
        {
            res(i / scale.height, j / scale.width) += (*this)(i, j);
        }
    }
    return res;
}


/*! The method do up-sampling operation on this matrix with a kernel by
 * multiplying each element in the matrix with every element in the kernel,
 * and placing the result in corresponding place.
 * \param kernel
 * \return up-sampled matrix
 * \date 2014-6-6
 */
AcMatrix AcMatrix::UpSample(const AcSize& scale) const
{
    AcMatrix res(m_height * scale.height, m_width * scale.width, 0);
    for (int i = 0; i != m_height; ++i)
    {
        for (int j = 0; j != m_width; ++j)
        {
            for (int x = 0; x != scale.height; ++x)
            {
                for (int y = 0; y != scale.width; ++y)
                {
                    res(i * scale.height + x, j * scale.width + y) = (*this)(i, j);
                }
            }
        }
    }
    return res;
}

/*! The method rotates this matrix with an angle 180
 * \return the rotated matrix
 */
AcMatrix AcMatrix::Rot180()const
{
    AcMatrix res(m_height, m_width);
    for (int i = 0; i != m_height; ++i)
    {
        for (int j = 0; j != m_width; ++j)
        {
            res(i, j) = (*this)(m_height - i - 1, m_width - j - 1);
        }
    }
    return res;
}

/*! The method converts a binary image into a matrix
 * \param a pointer to the image
 * \return a reference to the converted matrix, in another word, this matrix.
 */
AcMatrix& AcMatrix::ConvImage(IplImage* img)
{
    assert(img->nChannels == 1 && img->height == m_height && img->width == m_width);

    CvMat* mat = cvCreateMat(m_height, m_width, CV_64FC1);
    cvConvert(img, mat);
    for (int i = 0; i != img->height; ++i)
    {
        for (int j = 0; j != img->width; ++j)
        {
            double x = cvmGet(mat, i, j);
            (*this)(i, j) = (x < 127 ? -1 : 1);
        }
    }

    return *this;
}

// Operator
/*! Overloaded = operator, which will do deep copying
 * \param the source matrix
 * \return the reference to this matrix
 */
AcMatrix& AcMatrix::operator=(const AcMatrix& mat)
{
    if (this != &mat)
    {
        // need to make sure this matrix is released.
        Release();

        m_height = mat.m_height;
        m_width = mat.m_width;
        m_data = new double[m_height * m_width];
        for (int i = 0; i != m_height * m_width; ++i)
        {
            m_data[i] = mat.m_data[i];
        }
    }
    return *this;
}

/*! Overloaded () operator, playing the role of index operator.
 * \param the row index
 * \param the column index
 * \return the reference to the specific element
 */
double& AcMatrix::operator()(int i, int j)
{
    assert(i >= 0 && j >= 0 && i < m_height && j < m_width);
    return *(m_data + (i * m_width) + j);
}

/*! Overloaded constant () operator playing the role of index operator.
 * \param the row index
 * \param the column index
 * \return the reference to the specific element
 */
const double& AcMatrix::operator()(int i, int j) const
{
    assert(i >= 0 && j >= 0 && i < m_height && j < m_width);
    return *(m_data + (i * m_width) + j);
}

/*! Overloaded ! operator, which will return a transposed matrix of this one.
 * \return a transposed matrix of this one.
 */
AcMatrix AcMatrix::operator!() const
{
    AcMatrix ret(m_width, m_height);
    for (int i = 0; i != m_height; ++i)
    {
        for (int j = 0; j != m_width; ++j)
        {
            ret(j, i) = (*this)(i, j);
        }
    }
    return ret;
}

//! Plus operator with matrix
AcMatrix AcMatrix::operator+(const AcMatrix& mat) const
{
    assert(m_height == mat.m_height && m_width == mat.m_width);
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] += mat.m_data[i];
    }
    return res;
}

//! Plus operator with double
AcMatrix AcMatrix::operator+(const double b) const
{
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] += b;
    }
    return res;
}

//! Plus-assign operator with matrix
AcMatrix& AcMatrix::operator+=(const AcMatrix& mat)
{
    return *this = (*this) + mat;
}

//! Plus-assign operator with double
AcMatrix& AcMatrix::operator+=(const double b)
{
    return *this = (*this) + b;
}

//! Minus operator with matrix
AcMatrix AcMatrix::operator-(const AcMatrix& mat) const
{
    assert(m_height == mat.m_height && m_width == mat.m_width);
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] -= mat.m_data[i];
    }
    return res;
}

//! Minus operator with double
AcMatrix AcMatrix::operator-(const double b) const
{
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] -= b;
    }
    return res;
}

//! Minus-assign operator with matrix
AcMatrix& AcMatrix::operator-=(const AcMatrix& mat)
{
    return *this = (*this) - mat;
}

//! Minus-assign operator with double
AcMatrix& AcMatrix::operator-=(const double b)
{
    return *this = (*this) - b;
}

//! Times operator with matrix
AcMatrix AcMatrix::operator*(const AcMatrix& mat) const
{
    assert(m_height == mat.m_height && m_width == mat.m_width);
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] *= mat.m_data[i];
    }
    return res;
}

//! Times operator with double
AcMatrix AcMatrix::operator*(const double b) const
{
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] *= b;
    }
    return res;
}

//! Times-assign operator with matrix
AcMatrix& AcMatrix::operator*=(const AcMatrix& mat)
{
    return (*this) = (*this) * mat;
}

//! Times-assign operator with double
AcMatrix& AcMatrix::operator*=(const double b)
{
    return *this = (*this) * b;
}

//! Divide operator with matrix
AcMatrix AcMatrix::operator/(const AcMatrix& mat) const
{
    assert(m_height == mat.m_height && m_width == mat.m_width);
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] /= mat.m_data[i];
    }
    return res;
}

//! Divide operator with double
AcMatrix AcMatrix::operator/(const double b) const
{
    AcMatrix res = *this;
    for (int i = 0; i != m_height * m_width; ++i)
    {
        res.m_data[i] /= b;
    }
    return res;
}

//! Divide-assign operator with matrix
AcMatrix& AcMatrix::operator/=(const AcMatrix& mat)
{
    return (*this) = (*this) / mat;
}

//! Divide-assign operator with double
AcMatrix& AcMatrix::operator/=(const double b)
{
    return (*this) = (*this) / b;
}
