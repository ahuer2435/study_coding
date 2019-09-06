#include <sys/time.h>
#include "CommonParam.h"
#include <vector>
#include <string>
using namespace std;
using namespace cv;	

std::vector<String> strPics_;
int image_number;

#define START_TIMESTAMPE 1.0

static double get_currtime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime,NULL);
    double dTimeStamp = currentTime.tv_sec + 0.000001 * currentTime.tv_usec;
    return dTimeStamp;
}

static int load_images(void)
{
	cout << "loading pictures!" << endl;
    string pattern = "./source/pics";
	strPics_.clear();
	cv::glob(pattern, strPics_, false);
	image_number = int(strPics_.size());
	cout << "Totally "<< image_number<<" images"  << endl;
	if (strPics_.empty()) {
		cout << "cannot find any pics!" << endl;
	}
	return 0;
}

void* bstAdas_perception_Handler(void* args)
{
    try
    {
        {
            std::cout<< "adas_perception load images." << std::endl;
            load_images();
        }
        
        int frame_id = 0;
        double begin_time = 0;
        std::string filename;
        double start_time = START_TIMESTAMPE;
        double end_time = START_TIMESTAMPE;
        cv::Mat src_cv_img,cv_img;
        ADAS_PERCEPTION_INFOPack adasPerceptionData;
        
              
        while(1)
        {
            start_time = get_currtime();
            filename = strPics_[frame_id%image_number];
            begin_time = get_currtime();
            src_cv_img = imread(filename);
                            
            std::cout << "img = (" << src_cv_img.cols << "," << src_cv_img.rows << "," << src_cv_img.channels() << ")" << std::endl;           
            adasPerceptionData.frame = src_cv_img;
            adasPerceptionData.width = frame_id;
            adasPerceptionData.height = frame_id;
            MsgQueueAdasPerceptionData.put(adasPerceptionData);
                        
            std::cout << "adas step7: " << get_currtime() - begin_time << " ms"  << std::endl;
            end_time = get_currtime();
            frame_id++;
            std::cout << "adas fps : " << floor(1000/(end_time - start_time))  << std::endl;
        }
        std::cout  <<"quit bstAdas_perception_Handler..."<<std::endl;
    }
    catch(...)
    {        
        std::cout << "bstAdas_perception_Handler failed"<<std::endl;
    }
    std::cout <<"quit bstAdas_perception_Handler..."<<std::endl;
    pthread_exit(NULL);    
}

