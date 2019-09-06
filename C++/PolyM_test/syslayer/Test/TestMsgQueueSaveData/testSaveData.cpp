#include<bstMsgQueue.h>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <bstReadData.h>
#include <bstLog.h>
#include <CommonParam.h>
struct DataType
{
    int a = 1;
    int b = 2;
    float c = 3.5;
};

struct DataType2
{
    float a = 5.0f;
    float b = 6.0f;
    cv::Mat frame;
    // void printData(DataType2& data)
    // {
    //     LogPrint << "a:" << data.a<<" " <<",b:" <<data.b;
    //     //LogPrint <<"row:" <<data.frame.row << "col:" << data.frame.col;
    //     LogPrint << data.frame.at<float>(0,0) <<"," <<data.frame.at<float>(0,1)<<"," << data.frame.at<float>(0,2)<<"," << data.frame.at<float>(0,3);
    //     LogPrint << data.frame.at<float>(1,0) <<"," <<data.frame.at<float>(1,1)<<"," << data.frame.at<float>(1,2)<<"," << data.frame.at<float>(1,3);
    //     LogPrint << data.frame.at<float>(2,1) <<"," <<data.frame.at<float>(2,1)<<"," << data.frame.at<float>(2,2)<<"," << data.frame.at<float>(2,3);
    //     LogPrint << data.frame.at<float>(3,2) <<"," <<data.frame.at<float>(3,1)<<"," << data.frame.at<float>(3,2)<<"," << data.frame.at<float>(3,3);
    // }
    static int getDataSize()
    {
        return 84;
    }
    static void StoreDataType2(std::ofstream& filestream,DataType2& data)
    {
        filestream.write((char*)&data.a,sizeof(float));
        filestream.write((char*)&data.b,sizeof(float));
        int matrow = data.frame.rows;
        int matcol = data.frame.cols;
        int mattype = data.frame.type();
        
        filestream.write((char*)&matrow,sizeof(int));
        filestream.write((char*)&matcol,sizeof(int));
        filestream.write((char*)&mattype,sizeof(int));
        filestream.write((char*)data.frame.data,data.frame.total()*sizeof(float));
    }

    static void ReadDataType2(std::ifstream& infilestream,DataType2& data,int)
    {
        infilestream.read((char*)&data.a,sizeof(float));
        infilestream.read((char*)&data.b,sizeof(float));
        int matrow,matcol,mattype;
        
        infilestream.read((char*)&matrow,sizeof(int));
        infilestream.read((char*)&matcol,sizeof(int));
        infilestream.read((char*)&mattype,sizeof(int));
        //LogPrint <<"a:"<<data.a <<"b:"<<data.b<<"matrow:" <<matrow << ",matcol:" << matcol;
        data.frame.create(matrow,matcol,CV_32FC1);
        infilestream.read((char*)data.frame.data,matrow*matcol*sizeof(float));


        LogInfo << "a:" << data.a<<" " <<",b:" <<data.b;
        LogInfo << data.frame.at<float>(0,0) <<"," <<data.frame.at<float>(0,1)<<"," << data.frame.at<float>(0,2)<<"," << data.frame.at<float>(0,3);
        LogInfo << data.frame.at<float>(1,0) <<"," <<data.frame.at<float>(1,1)<<"," << data.frame.at<float>(1,2)<<"," << data.frame.at<float>(1,3);
        LogInfo << data.frame.at<float>(2,1) <<"," <<data.frame.at<float>(2,1)<<"," << data.frame.at<float>(2,2)<<"," << data.frame.at<float>(2,3);
        LogInfo << data.frame.at<float>(3,2) <<"," <<data.frame.at<float>(3,1)<<"," << data.frame.at<float>(3,2)<<"," << data.frame.at<float>(3,3);
    }
};

// void ReadData(std::string sFilename)
// {
//     std::ifstream infilestream(sFilename,std::ios::binary|std::ios::ate);
//     int len = infilestream.tellg();
//     //LogPrint << "size:" << len;
//     DataType data;
//     infilestream.seekg(0,infilestream.beg);
//     infilestream.read((char*)&data,sizeof(DataType));
//     infilestream.close();
//     //LogPrint << data.a << "," << data.b << "," << data.c;
// }
// int main()
// {
//     LOGFILE("TESTLOG");
//     bstMsgQueue<DataType> MsgQueue("testqueue");
//     MsgQueue.setDataRecord("TestSaveData.dat",true,true);
//     LogDebug << "Push DataType start";
//     for(int i = 0; i < 10;i++)
//     {
//         DataType data;
//         data.a = i;
//         data.b = i+1;
//         data.c = i+0.5f;
//         MsgQueue.put(data);
//         LogInfo <<"Index:"<<i<< "a:" << data.a << ",b" << data.b << ",c" << data.c;
//     }
//     LogDebug << "Push DataType End";
//     MsgQueue.SaveDataFlush();
    
//     BstReadData<DataType> readdata1;
//     readdata1.setReadData("TestSaveData.dat");
//     for(int i = 0; i < 12;i++)
//     {
//         DataType data1;
//         if(readdata1.get(data1))
//         {
//             LogInfo << data1.a << ",  " << data1.b << ",  " << data1.c;    
//         }
//     }
//     readdata1.finishRead();

    
    
//     bstMsgQueue<DataType2> MsgQueue2("testqueue2");
//     MsgQueue2.setDataRecord("TestSaveData2.dat",true,true,DataType2::StoreDataType2);
//     for(int i = 0; i < 10;i++)
//     {
//         DataType2 data2;
//         data2.a = float(i);
//         data2.b = float(i+1);
        
//         data2.frame = cv::Mat::ones(4,4,CV_32FC1);
//         data2.frame *= i;
//         MsgQueue2.put(data2);
        
//     }
//     MsgQueue2.SaveDataFlush();

//     BstReadData<DataType2> readdata2;
//     readdata2.setReadData("TestSaveData2.dat",DataType2::getDataSize(),DataType2::ReadDataType2);
//     for(int i = 0; i < 10;i++)
//     {
//         DataType2 data21;
//         if(!readdata2.get(data21))
//         {
//             LogWarning << "not enough data";
//             break;
//         }

//     }
    
//     readdata2.finishRead();
//     return 0;
    
// }   

int main()
{
    LOGFILE("IMU_GPS");
    bstMsgQueue<IMU_GPS_SRV_Data> MsgQueue("IMU_GPS");
    MsgQueue.setDataRecord("IMU_GPS_DATA.dat",true,true);
    LogDebug << "Push DataType start";
    for(int i = 0; i < 10000;i++)
    {
        IMU_GPS_SRV_Data data;
        data.dTimeStamp = 100.0 + i * 300;
        data.fspeed   =  float(30 + i * 10);
        data.fRotAngle = float(i);
        MsgQueue.put(data);
        //LogInfo <<"Index:"<<i<< "a:" << data.a << ",b" << data.b << ",c" << data.c;
    }
    LogDebug << "Push DataType End";
    MsgQueue.SaveDataFlush();
    
    BstReadData<IMU_GPS_SRV_Data> readdata1;
    readdata1.setReadData("IMU_GPS_DATA.dat");
    for(int i = 0; i < 10000;i++)
    {
        IMU_GPS_SRV_Data data1;
        if(readdata1.get(data1))
        {
            LogDebug << data1.dTimeStamp << ",  " << data1.fspeed << ",  " << data1.fRotAngle;    
        }
    }
    readdata1.finishRead();
    LogInfo<<"test1";


    LOGFILE("DMS_STATE");
    bstMsgQueue<DMS_STATE> MsgQueue1("DMS_STATE");
    MsgQueue1.setDataRecord("DMS_STATE_DATA.dat",true,true);
    LogDebug << "Push DataType start";
    for(int i = 0; i < 10000;i++)
    {
        DMS_STATE data;
        data.iFatigueLeve = i;
        data.bEyeClose   =  true;
        data.bMouseClose =  true;
        data.dTimeStamp = double(i);
        MsgQueue1.put(data);
        //LogInfo <<"Index:"<<i<< "a:" << data.a << ",b" << data.b << ",c" << data.c;
    }
    LogDebug << "Push DataType End";
    MsgQueue1.SaveDataFlush();
    
    BstReadData<DMS_STATE> readdata11;
    readdata11.setReadData("DMS_STATE_DATA.dat");
    for(int i = 0; i < 10000;i++)
    {
        DMS_STATE data1;
        if(readdata11.get(data1))
        {
            LogDebug << data1.dTimeStamp << ",  " << data1.bMouseClose << ",  " << data1.bEyeClose<<", "<<data1.iFatigueLeve;    
        }
    }
    readdata11.finishRead();
    LogInfo<<"test2";

    
    LOGFILE("DMS_CAM");
    bstMsgQueue<DMS_CAM_INFOPack> MsgQueue2("DMSCAM");
    MsgQueue2.setDataRecord("DMS_CAM_DATA.dat",true,true,DMS_CAM_INFOPack::StoreData);
    for(int i = 0; i < 10000;i++)
    {
        DMS_CAM_INFOPack data2;
        data2.dTimeStamp = 4000.0 + i * 300;
        data2.imu_gps_srv_Data.dTimeStamp = data2.dTimeStamp;
        data2.imu_gps_srv_Data.fspeed = 160 + 10 * i;
        data2.imu_gps_srv_Data.fRotAngle = float(i+13);
        data2.cvFrame = cv::Mat::ones(4,4,CV_8UC1);
        data2.cvFrame *= i;
        MsgQueue2.put(data2);
    }
    MsgQueue2.SaveDataFlush();

    BstReadData<DMS_CAM_INFOPack> readdata2;
    readdata2.setReadData("DMS_CAM_DATA.dat",DMS_CAM_INFOPack::getDataSize(),DMS_CAM_INFOPack::ReadData);
    for(int i = 0; i < 10000;i++)
    {
        DMS_CAM_INFOPack data21;
        if(!readdata2.get(data21))
        {
            LogWarning << "not enough data";
            break;
        }
    }
    LogInfo << "test log!";
    readdata2.finishRead();
    return 0;
    
}   