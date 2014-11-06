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

/*!\file AcMatrix.h
 * \author ZHANG Shu-tao
 * \description declaration of double-type matrix class and size
 * \date 2014-6-3
 * \modify 2014-6-6 Added class AcSize, ZHANG Shu-tao
 * \modify 2014-6-6 modified AcMatrix, added some methods, ZHANG Shu-tao
 * \modify 2014-6-10 modifying up-sample and down-sample.
 */

#ifndef ACMATRIX_H_INCLUDED
#define ACMATRIX_H_INCLUDED

#include <cv.h>

class AcSize
{
public:
    AcSize(int h = 0, int w = 0);
    AcSize(const AcSize& b);
    ~AcSize();

    bool operator==(const AcSize& b) const;
    bool operator!=(const AcSize& b) const;
    AcSize& operator=(const AcSize& b);
public:
    int height; //!< the height
    int width; //!< the width
};

typedef double (*MATRIX_FUNC)(double x);

class AcMatrix
{
public:
    // Constructor & Destructor
    //! Constructor
    AcMatrix(int height = 0, int width = 0, double val = 0);

    //! Constructor
    AcMatrix(const AcMatrix& mat);

    //! Destructor
    ~AcMatrix();

    // Method
    //! Create the matrix.
    int Create(int height = 0, int width = 0, double val = 0);

    //! Release the matrix.
    int Release();

    //! Get the height;
    int GetHeight() const;

    //! Get the width
    int GetWidth() const;

    //! Get the size
    AcSize GetSize() const;

    //! Randomly set the value of each element
    int SetRand(double left, double right);

    //! Set the value of each element as val
    int SetValue(double val);

    //! Calculate operator, calculate each element
    AcMatrix Calcu(MATRIX_FUNC pfunc);

    //! Convolution operation
    AcMatrix ConvValid(const AcMatrix& mat) const;

    //! Convolution operation
    AcMatrix ConvFull(const AcMatrix& mat) const;

    //! Sub-sampling operation
    AcMatrix DownSample(const AcSize& scale) const;

    //! Up-sampling operation
    AcMatrix UpSample(const AcSize& scale) const;

    //! Rotate operation
    AcMatrix Rot180()const;

    //! Convert a binary image into a matrix
    AcMatrix& ConvImage(IplImage* img);

    // Operator
    //! assignment operator
    AcMatrix& operator=(const AcMatrix& mat);

    //! Index operator
    double& operator()(int i, int j);

    //! Constant index operator
    const double& operator()(int i, int j) const;

    //! Trans operator
    AcMatrix operator!() const;

    //! Plus operator with matrix
    AcMatrix operator+(const AcMatrix& mat) const;

    //! Plus operator with double
    AcMatrix operator+(const double b) const;

    //! Plus-assign operator with matrix
    AcMatrix& operator+=(const AcMatrix& mat);

    //! Plus-assign operator with double
    AcMatrix& operator+=(const double b);

    //! Minus operator with matrix
    AcMatrix operator-(const AcMatrix& mat) const;

    //! Minus operator with double
    AcMatrix operator-(const double b) const;

    //! Minus-assign operator with matrix
    AcMatrix& operator-=(const AcMatrix& mat);

    //! Minus-assign operator with double
    AcMatrix& operator-=(const double b);

    //! Times operator with matrix
    AcMatrix operator*(const AcMatrix& mat) const;

    //! Times operator with double
    AcMatrix operator*(const double b) const;

    //! Times-assign operator with matrix
    AcMatrix& operator*=(const AcMatrix& mat);

    //! Times-assign operator with double
    AcMatrix& operator*=(const double b);

    //! Divide operator with matrix
    AcMatrix operator/(const AcMatrix& mat) const;

    //! Divide operator with double
    AcMatrix operator/(const double b) const;

    //! Divide-assign operator with matrix
    AcMatrix& operator/=(const AcMatrix& mat);

    //! Divide-assign operator with double
    AcMatrix& operator/=(const double b);

protected:
    double* m_data;
    int m_height;
    int m_width;
};

#endif // ACMATRIX_H_INCLUDED
