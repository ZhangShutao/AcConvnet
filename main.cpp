#include <iostream>
#include <cstdio>
#include "AcConvNet.h"
#include <cv.h>
#include <highgui.h>

using namespace std;

inline double seg_rand(double a, double b)
{
    return (double)rand() / RAND_MAX * (b - a) + a;
}

IplImage* CreateSingleTrans(IplImage* src)
{
    IplImage* dst = cvCloneImage(src);
    IplImage* tmp = cvCloneImage(src);
    dst->origin = src->origin;
    cvZero(dst);
    CvMat* trans_mat = cvCreateMat(3, 3, CV_32FC1);
    CvMat* rot_mat = cvCreateMat(2, 3, CV_32FC1);

    CvPoint2D32f srcQuad[4], dstQuad[4];
    srcQuad[0].x = 0;
    srcQuad[0].y = 0;
    srcQuad[1].x = src->width - 1;
    srcQuad[1].y = 0;
    srcQuad[2].x = 0;
    srcQuad[2].y = src->height - 1;
    srcQuad[3].x = src->width - 1;
    srcQuad[3].y = src->height - 1;

    dstQuad[0].x = seg_rand(-5, 5);
    dstQuad[0].y = seg_rand(-5, 5);
    dstQuad[1].x = src->width + seg_rand(-5, 5);
    dstQuad[1].y = seg_rand(-5, 5);
    dstQuad[2].x = seg_rand(-5, 5);
    dstQuad[2].y = src->height + seg_rand(-5, 5);
    dstQuad[3].x = src->width + seg_rand(-5, 5);
    dstQuad[3].y = src->height + seg_rand(-5, 5);

    cvGetPerspectiveTransform(srcQuad, dstQuad, trans_mat);
    cvWarpPerspective(src, tmp, trans_mat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalar(0, 0, 0, 0));
    cvCopy(tmp, dst);

    cvReleaseMat(&trans_mat);
    cvReleaseMat(&rot_mat);
    int blur_size = seg_rand(0, 1);
    if (blur_size != 0)
    {
        cvSmooth(dst, tmp, CV_GAUSSIAN, blur_size * 2 + 1);
        cvCopy(tmp, dst);
    }
    cvReleaseImage(&tmp);
    return dst;
}

IplImage* GetSample(int label)
{
    char filename[20];
    sprintf(filename, "sample/%d.png", label);
    IplImage* origin = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* img = CreateSingleTrans(origin);
    cvReleaseImage(&origin);
    return img;
}

int main(int argc, char* argv[])
{
    AcConvNet net;
    cout<<"loading net: "<< net.Load("config") << endl;

    for (int i = 0; i != 1000; ++i)
    {
        cout << "training iteration: " << i << endl;
        for (int k = 0; k != 10; ++k)
        {
            IplImage* img = GetSample(k);
            net.Train(img, k);
            cvReleaseImage(&img);
        }
    }

    freopen("log", "w", stdout);
    int errorcnt = 0, cnt = 0;
    for (int i = 0; i != 50; ++i)
    {
        for (int k = 0; k != 10; ++k)
        {
            IplImage* img = GetSample(k);
            AcResult res = net.Classify(img);
            cvReleaseImage(&img);
            cout << k << " " << res.label << endl;
            ++cnt;
            if (res.label != k)
            {
                ++errorcnt;
            }
        }
    }
    cout << errorcnt * 1.0 / cnt << endl;
    net.Store("config");

    ofstream ferror("error", ios::app);
    ferror << errorcnt * 1.0 / cnt <<endl;
    ferror.close();
    return 0;
}
