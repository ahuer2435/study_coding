#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//矩阵掩码操作表示使用相邻像素，作用于目标像素的一种操作表示，起滤波器的作用。
static void help(char* progName)
{
    cout << endl
        <<  "This program shows how to filter images with mask: the write it yourself and the"
        << "filter2d way. " << endl
        <<  "Usage:"                                                                        << endl
        << progName << " [image_name -- default lena.jpg] [G -- grayscale] "        << endl << endl;
}


void Sharpen(const Mat& myImage,Mat& Result);

int main( int argc, char* argv[])
{
    help(argv[0]);
    const char* filename = argc >=2 ? argv[1] : "test.png";

    Mat I, J, K;

    if (argc >= 3 && !strcmp("G", argv[2]))
        I = imread( filename, CV_LOAD_IMAGE_GRAYSCALE);
    else
        I = imread( filename, CV_LOAD_IMAGE_COLOR);

    //namedWindow("Input", WINDOW_AUTOSIZE);
    //namedWindow("Output", WINDOW_AUTOSIZE);

    imshow("Input", I);
    double t = (double)getTickCount();

    Sharpen(I, J);

    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Hand written function times passed in seconds: " << t << endl;

    imshow("Output", J);
    waitKey(0);

    //核函数,速度较快
    Mat kern = (Mat_<char>(3,3) <<  0, -1,  0,
                                   -1,  5, -1,
                                    0, -1,  0);
    t = (double)getTickCount();
    //I输入，k输出，I.depth每个像素占用bit数。
    filter2D(I, K, I.depth(), kern );
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Built-in filter2D time passed in seconds:      " << t << endl;

    imshow("Output", K);

    waitKey(0);
    return 0;
}

void Sharpen(const Mat& myImage,Mat& Result)
{
    CV_Assert(myImage.depth() == CV_8U);  // accept only uchar images

    const int nChannels = myImage.channels();
    Result.create(myImage.size(),myImage.type());   //创建一个同size和type的mat

    for(int j = 1 ; j < myImage.rows-1; ++j)
    {
        const uchar* previous = myImage.ptr<uchar>(j - 1);
        const uchar* current  = myImage.ptr<uchar>(j    );
        const uchar* next     = myImage.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);   //output初始时，指向行地址，但是每次增加一个像素，与其类型相关。

        for(int i= nChannels;i < nChannels*(myImage.cols-1); ++i)
        {
            *output++ = saturate_cast<uchar>(5*current[i]
                         -current[i-nChannels] - current[i+nChannels] - previous[i] - next[i]);
        }
    }

    Result.row(0).setTo(Scalar(255));
    Result.row(Result.rows-1).setTo(Scalar(255));
    Result.col(0).setTo(Scalar(255));
    Result.col(Result.cols-1).setTo(Scalar(255));
}
