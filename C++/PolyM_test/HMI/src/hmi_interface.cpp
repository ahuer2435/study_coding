#include "CommonParam.h"
#include "hmi_interface.h"

static double get_currtime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime,NULL);
    double dTimeStamp = currentTime.tv_sec + 0.000001 * currentTime.tv_usec;
    return dTimeStamp;
}


void HMI_dispaly()
{
    try
    {
         
        cv::namedWindow("HMI",CV_WINDOW_NORMAL);        
        //cv::resizeWindow("HMI",1280,480);
        cv::resizeWindow("HMI",1360,768);
        cv::moveWindow("HMI",0,0);      
        
        
        ADAS_PERCEPTION_INFOPack adasPerceptionData_last;
        double begin_time = 0;
        bool is_adas_curr = false;
        bool is_dms_curr = false;
        double start_time,end_time;        
        
        bool first_adas_frame_valid = false;
        
        ADAS_PERCEPTION_INFOPack adasPerceptionData;
        cv::Mat tmp_dms_img,dms_img;
        cv::Mat adas_img;
        cv::Mat dst_adas_img;
        cv::Mat combine_img;


        while(1)
        {
            start_time = get_currtime();
            begin_time = get_currtime();
            if(MsgQueueAdasPerceptionData.size() > 0){
                is_adas_curr = true;
                first_adas_frame_valid = true;
                MsgQueueAdasPerceptionData.get(adasPerceptionData);
                std::cout <<"adasPerceptionData " << adasPerceptionData.frame.rows<<" "<<adasPerceptionData.frame.cols<<" " << adasPerceptionData.frame.channels()<<std::endl;
                adasPerceptionData_last.frame = adasPerceptionData.frame.clone();                    
            }else{
                is_adas_curr = false;
                adasPerceptionData = adasPerceptionData_last;
            }                
            
            std::cout << "HMI_dispaly step_C: " << get_currtime() - begin_time << " ms"  << std::endl;
            begin_time = get_currtime();            
            
            if(first_adas_frame_valid){
                adas_img = adasPerceptionData.frame.clone();            
            }
            //std::cout << "HMI_dispaly step_D: " << get_currtime() - begin_time << " ms"  << std::endl;
            //begin_time = get_currtime();
            
            if(first_adas_frame_valid){                
                cv::Size rsize(640,480);
                cv::resize(adas_img, dst_adas_img, rsize);
            }
            std::cout << "HMI_dispaly step_E: " << get_currtime() - begin_time << " ms"  << std::endl;
            begin_time = get_currtime();
                        
             if(first_adas_frame_valid){
                //std::cout << "hmi 6b" <<std::endl;                
                cv::imshow("HMI",dst_adas_img);
                //cv::moveWindow("HMI",0,0);
                cv::waitKey(1);
            }
            end_time = get_currtime();
            std::cout << "HMI_dispaly step_E: " << end_time - begin_time << " ms"  << std::endl;
            std::cout <<"quit HMI_dispaly...";
        }
    }
    catch(...)
    {
        std::cout << "HMI_dispaly failed";
    }     
}

