#define BOOST_LOG_DYN_LINK 1
#ifndef BST_MSG_QUEUE_H
#define BST_MSG_QUEUE_H
#include "Msg.hpp"
#include "Queue.hpp"
#include <string>
#include <fstream>
#include <boost/log/trivial.hpp>
#include "bstLog.h"
//#define LogPrint BOOST_LOG_TRIVIAL(info)

//const std::string sPath = "../DebugData/";
const std::string sPath = "./";
template <typename dataType>
class bstMsgQueue
{
public:
    bstMsgQueue(std::string sQueueName,int maxObject = 1)
        :m_bRecordData(false)
        ,m_message(0)
        ,m_queueName(sQueueName)
        ,m_StoreFunc(NULL)
        ,m_ReleaseFunc(NULL)
        ,m_maxObject(maxObject)
    {

    }
    
    ~bstMsgQueue()
    {
        if(m_DataFile.is_open())
        {
            m_DataFile.close();
        }
    }

    void setDataRecord(std::string sFileName,bool bRecordData,bool turn,void (*func)(std::ofstream&,dataType&)=NULL)
    {
        m_bRecordData = bRecordData;
        m_bSaveDataTurn = turn;
        m_StoreFunc = func;
        if(m_bRecordData && m_bSaveDataTurn)
        {
            std::string sFilePath = sFileName;
            //LogPrint << sFilePath;
            m_DataFile.open(sFileName,std::ios::binary);
            //std::ofstream datafile("test.dat",std::ios::binary);
            if(!m_DataFile.is_open())
            {
                LogError << "failed to open file";
            }
        }
    }

    void setReleaseCb(void (*cb)(dataType&)=NULL)
    {
        m_ReleaseFunc = cb;
    }

    void put(dataType& Data)
    {
        if(m_bRecordData && m_bSaveDataTurn)
        {
            if(m_StoreFunc==NULL)
            {
                m_DataFile.write((char*)&Data,sizeof(dataType));
            }
            else
            {
                m_StoreFunc(m_DataFile,Data);
            }
            m_DataFile.flush();    
        }
       
        while(m_queue.size() > (m_maxObject-1)){
            m_queue.get();
        }
        
        m_queue.put(PolyM::DataMsg<dataType>(m_message++,Data));

        if (m_ReleaseFunc) {
            m_ReleaseFunc(Data);
            printf("m_queue size %d\n", size());
            m_queue.get();
        }
        
        LogDebug << m_queueName <<" got Message " << m_message-1;
    }

    void get(dataType& data,int timeoutMillis=0)
    {
        auto Message = m_queue.get();
        auto& dMessage = dynamic_cast<PolyM::DataMsg<dataType>&>(*Message);
        data = std::move(dMessage.getPayload()); 
        LogDebug << m_queueName <<" output Message " << dMessage.getMsgId();
    }

    int size()
    {
        return m_queue.size();
    }

    bool empty()
    {
        return m_queue.empty();
    }
    void ResetDataRecord(bool bRecord,bool turn)
    {
        m_bRecordData = bRecord;
        m_bSaveDataTurn = turn;
    }
    void SaveDataFlush()
    {
        m_DataFile.flush();
    }
    typedef void (*st_func)(std::ofstream&,dataType&);
    typedef void (*bufReleaseFunc)(dataType&);
private:
    PolyM::Queue  m_queue;
    bool          m_bRecordData;
    bool          m_bSaveDataTurn;
    long long     m_message;
    std::string   m_queueName;
    std::ofstream m_DataFile;
    st_func    m_StoreFunc;
    bufReleaseFunc m_ReleaseFunc;
    int m_maxObject;
};


#endif