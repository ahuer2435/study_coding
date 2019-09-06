#ifndef BST_READ_DATA_H
#define BST_READ_DATA_H
#define BOOST_LOG_DYN_LINK 1
#include <fstream>
#include <string>
#include <boost/log/trivial.hpp>
//#include "bstMsgQueue.h"
#define LogPrint BOOST_LOG_TRIVIAL(info)

template <typename DataType>
class BstReadData
{
public:
    
    // static void ReadDataCommon(std::ifstream& filestream,DataType& data,int idataSize)
    // {
    //     filestream.read((char*)&data,idataSize);
    // }

    bool setReadData(std::string filename,int datasize = sizeof(DataType),void (*pfunc)(std::ifstream&,DataType&,int)=NULL)
    {
        m_DataFile.open(filename,std::ios::binary|std::ios::in|std::ios::ate);
        if(!m_DataFile||!m_DataFile.is_open())
        {
            LogError << "failed to open the file " << filename;
            return false;
        }
        m_bOpenFile = true;
        m_iDataSize = datasize;
        m_iDataNum = m_DataFile.tellg()/datasize;
        LogDebug << "m_iDataNum:" << m_iDataNum << std::endl;
        m_DataFile.seekg(0,std::ios::beg);
        m_ReadFunc = pfunc;
        // if(m_ReadFunc == NULL)
        // {
        //     m_ReadFunc = ReadDataCommon;
        // }
        m_iOutputDataNum = 0;
        return true;
    }

    bool get(DataType& data)
    {
        bool bGot = false;
        if(m_iOutputDataNum++ < m_iDataNum && m_bOpenFile)
        {
            if(m_ReadFunc == NULL)
            {
                m_DataFile.read((char*)&data,m_iDataSize);
            }
            else
            {
               m_ReadFunc(m_DataFile,data,m_iDataSize);   
            }
            bGot = true;
        }
        return bGot;
    }

    void finishRead()
    {
        m_DataFile.close();
    }

    int getDataNum()
    {
        return m_iDataNum;
    }
    typedef void (*pFunc)(std::ifstream&,DataType&,int);
private:
    std::ifstream  m_DataFile;
    int            m_iDataNum;
    int            m_iDataSize;
    int            m_iOutputDataNum;
    
    pFunc          m_ReadFunc;
    bool           m_bOpenFile;
};

#endif