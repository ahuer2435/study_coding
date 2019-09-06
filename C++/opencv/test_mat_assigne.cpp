#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

const string filename="test.jpeg";

int main(int argc, char **arv)
{

	Mat dst;
	Mat src = imread(filename);
	dst = src;	
	cout << src.rows << src.cols << hex << src.data<<std::cout;
	cout << dst.rows << dst.cols << hex << dst.data<<std::cout;	
	return 0;
}
