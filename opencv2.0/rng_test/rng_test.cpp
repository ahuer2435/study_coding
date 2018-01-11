/******************************************************
    测试程序 006 - 【 随机绘制 】
    时间：2016年8月22日
******************************************************/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/// Global Variables
const int NUMBER = 100;     //绘制个数
const int DELAY = 5;        //延时

const int window_width = 900;
const int window_height = 600;
int x_1 = -window_width/2;
int x_2 = window_width*3/2;
int y_1 = -window_width/2;
int y_2 = window_width*3/2;

/// Function headers
static Scalar randomColor( RNG& rng );
int Drawing_Random_Lines( Mat image, char* window_name, RNG rng );
int Drawing_Random_Rectangles( Mat image, char* window_name, RNG rng );
int Drawing_Random_Ellipses( Mat image, char* window_name, RNG rng );


int Drawing_Random_Polylines( Mat image, char* window_name, RNG rng );
int Drawing_Random_Filled_Polygons( Mat image, char* window_name, RNG rng );
int Drawing_Random_Circles( Mat image, char* window_name, RNG rng );
int Displaying_Random_Text( Mat image, char* window_name, RNG rng );
int Displaying_Big_End( Mat image, char* window_name, RNG rng );
static Scalar randomColor( RNG& rng );

/**
 * @function main
 */
int main( void )
{
    int c;

    /// Start creating a window
    char window_name[] = "Drawing_2 Tutorial";

    // 【1、随机数对象，初始化】Also create a random object (RNG)
    RNG rng( 0xFFFFFFFF );	//初始化rng对象

    int r = (unsigned) rng;
    cout<<r<<endl;
    cout<<rng.uniform( -100, 100 )<<endl;   //规定化，随机数范围

    // 【2、0矩阵初始化图像】
    Mat image = Mat::zeros( window_height, window_width, CV_8UC3 );
    imshow( window_name, image );       // Show it in a window during DELAY ms
    waitKey( DELAY );
#if 0
    /// 【3、绘制直线】Now, let's draw some lines
    c = Drawing_Random_Lines(image, window_name, rng);  //绘制随机直线
    if( c != 0 ) return 0;                              //

    /// Go on drawing, this time nice rectangles
    c = Drawing_Random_Rectangles(image, window_name, rng);
    if( c != 0 ) return 0;

    /// Draw some ellipses
    c = Drawing_Random_Ellipses( image, window_name, rng );
    if( c != 0 ) return 0;
#endif
    
    /// Now some polylines
    c = Drawing_Random_Polylines( image, window_name, rng );
    if( c != 0 ) return 0;

    /// Draw filled polygons
    c = Drawing_Random_Filled_Polygons( image, window_name, rng );
    if( c != 0 ) return 0;

    /// Draw circles
    c = Drawing_Random_Circles( image, window_name, rng );
    if( c != 0 ) return 0;

    /// Display text in random positions
    c = Displaying_Random_Text( image, window_name, rng );
    if( c != 0 ) return 0;

    /// 【4、绘制结局】Displaying the big end!
    c = Displaying_Big_End( image, window_name, rng );
    if( c != 0 ) return 0;

    waitKey(0); //永远等待
    return 0;
}

/// Function definitions

/**
 * @function 【随机色彩】
 * @brief Produces a random color given a random object
 */
static Scalar randomColor( RNG& rng )
{
    int icolor = (unsigned) rng;                //随机数，rng用法本身可以作为数字使用
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );     //随机数取高低位
}


/**
 * @function 【随机线段】
 */
int Drawing_Random_Lines( Mat image, char* window_name, RNG rng )
{
  Point pt1, pt2;

  for( int i = 0; i < NUMBER; i++ )     //绘制起终点直线，NUMBER = 100次
  {
    pt1.x = rng.uniform( x_1, x_2 );	//使用rng的uniform函数，产生范围数
    pt1.y = rng.uniform( y_1, y_2 );
    pt2.x = rng.uniform( x_1, x_2 );
    pt2.y = rng.uniform( y_1, y_2 );

    line( image, pt1, pt2, randomColor(rng), rng.uniform(1, 10), 8 );	//颜色和粗细是随机的
    imshow( window_name, image );
    if( waitKey( DELAY ) >= 0 )     //等待 5ms，完毕，返回-1
      { return -1; }
  }
  return 0;                         //循环完毕，返回0——》C
}

/**
 * @function 【随机矩形】
 */
int Drawing_Random_Rectangles( Mat image, char* window_name, RNG rng )
{
  Point pt1, pt2;
  int lineType = 8;
  int thickness = rng.uniform( -3, 10 );

  for( int i = 0; i < NUMBER; i++ )
  {
    pt1.x = rng.uniform( x_1, x_2 );
    pt1.y = rng.uniform( y_1, y_2 );
    pt2.x = rng.uniform( x_1, x_2 );
    pt2.y = rng.uniform( y_1, y_2 );

    rectangle( image, pt1, pt2, randomColor(rng), MAX( thickness, -1 ), lineType );	//颜色和粗细是随机的

    imshow( window_name, image );
    if( waitKey( DELAY ) >= 0 )
      { return -1; }
  }

  return 0;
}

/**
 * @function 【随机椭圆】
 */
int Drawing_Random_Ellipses( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for ( int i = 0; i < NUMBER; i++ )
  {
    Point center;
    center.x = rng.uniform(x_1, x_2);
    center.y = rng.uniform(y_1, y_2);

    Size axes;
    axes.width = rng.uniform(0, 200);
    axes.height = rng.uniform(0, 200);

    double angle = rng.uniform(0, 180);

    ellipse( image, center, axes, angle, angle - 100, angle + 200,
             randomColor(rng), rng.uniform(-1,9), lineType );	//中心，长轴、短轴随机

    imshow( window_name, image );

    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }

  return 0;
}

/**
 * @function 【随机折线】
 */
int Drawing_Random_Polylines( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for( int i = 0; i< NUMBER; i++ )
  {
    Point pt[2][3];
    pt[0][0].x = rng.uniform(x_1, x_2);
    pt[0][0].y = rng.uniform(y_1, y_2);
    pt[0][1].x = rng.uniform(x_1, x_2);
    pt[0][1].y = rng.uniform(y_1, y_2);
    pt[0][2].x = rng.uniform(x_1, x_2);
    pt[0][2].y = rng.uniform(y_1, y_2);
    pt[1][0].x = rng.uniform(x_1, x_2);
    pt[1][0].y = rng.uniform(y_1, y_2);
    pt[1][1].x = rng.uniform(x_1, x_2);
    pt[1][1].y = rng.uniform(y_1, y_2);
    pt[1][2].x = rng.uniform(x_1, x_2);
    pt[1][2].y = rng.uniform(y_1, y_2);

    const Point* ppt[2] = {pt[0], pt[1]};	//点首地址
    int npt[] = {3, 3};				//点个数

    polylines(image, ppt, npt, 2, true, randomColor(rng), rng.uniform(1,10), lineType);

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }
  waitKey(0);
  return 0;
}

/**
 * @function 【随机填充多边形】
 */
int Drawing_Random_Filled_Polygons( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for ( int i = 0; i < NUMBER; i++ )
  {
    Point pt[2][3];
    pt[0][0].x = rng.uniform(x_1, x_2);
    pt[0][0].y = rng.uniform(y_1, y_2);
    pt[0][1].x = rng.uniform(x_1, x_2);
    pt[0][1].y = rng.uniform(y_1, y_2);
    pt[0][2].x = rng.uniform(x_1, x_2);
    pt[0][2].y = rng.uniform(y_1, y_2);
    pt[1][0].x = rng.uniform(x_1, x_2);
    pt[1][0].y = rng.uniform(y_1, y_2);
    pt[1][1].x = rng.uniform(x_1, x_2);
    pt[1][1].y = rng.uniform(y_1, y_2);
    pt[1][2].x = rng.uniform(x_1, x_2);
    pt[1][2].y = rng.uniform(y_1, y_2);

    const Point* ppt[2] = {pt[0], pt[1]};
    int npt[] = {3, 3};

    fillPoly( image, ppt, npt, 2, randomColor(rng), lineType );

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
       { return -1; }
  }
  return 0;
}

/**
 * @function 【随机圆】
 */
int Drawing_Random_Circles( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for (int i = 0; i < NUMBER; i++)
  {
    Point center;
    center.x = rng.uniform(x_1, x_2);
    center.y = rng.uniform(y_1, y_2);

    circle( image, center, rng.uniform(0, 300), randomColor(rng),
            rng.uniform(-1, 9), lineType );

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }

  return 0;
}

/**
 * @function 【随机文本】
 */
int Displaying_Random_Text( Mat image, char* window_name, RNG rng )
{
    int lineType = 8;

    for ( int i = 1; i < NUMBER; i++ )
    {
        Point org;
        org.x = rng.uniform(x_1, x_2);
        org.y = rng.uniform(y_1, y_2);

        putText( image, "Testing text rendering", org, rng.uniform(0,8),
                    rng.uniform(0,100)*0.05+0.1, randomColor(rng), rng.uniform(1, 10), lineType);

        imshow( window_name, image );
        if( waitKey(DELAY) >= 0 )
            { return -1; }
    }

    return 0;
}

/**
 * @function 【大结局】
 */
int Displaying_Big_End( Mat image, char* window_name, RNG )
{
    Size textsize = getTextSize("OpenCV forever!", FONT_HERSHEY_COMPLEX, 3, 5, 0);
    Point org((window_width - textsize.width)/2, (window_height - textsize.height)/2);
    int lineType = 8;

    Mat image2;

    for( int i = 0; i < 255; i += 2 )
    {
        image2 = image - Scalar::all(i);                                    //背景【减除】渐变
        putText( image2, "OpenCV forever!", org, FONT_HERSHEY_COMPLEX, 3,
                    Scalar(i, i, 255), 5, lineType );                           //文字色彩渐变

        imshow( window_name, image2 );
        if( waitKey(DELAY) >= 0 )
            { return -1; }
    }

    return 0;
}

//使用rng绘制各种图形和文字
//绘制文字的接口：putText