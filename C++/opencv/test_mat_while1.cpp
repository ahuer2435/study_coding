#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

/**/
int main(int argc, char **arv)
{
	VideoCapture capture(0);
	if(!capture.isOpened()){
		return 1;
	}

	
	bool stop = false;
	
	while(!stop){
		Mat Frame;	
		capture >> Frame;
		imshow("Video",Frame);
		if(cvWaitKey(30) == 48){
			stop = true;
		}
	}
	return 0;
}
