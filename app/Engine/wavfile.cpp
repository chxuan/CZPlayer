#include "stdafx.h"
#include <QtCore/qendian.h>
#include <QVector>
#include <QDebug>
#include "utils.h"
#include "wavfile.h"

//文件描述符
struct chunk
{
    char			 id[4];			    //id
    unsigned int     size;				//文件大小
};

//数据头
struct DATAHeader
{
	chunk       descriptor;				//文件描述符
};

//RIFF头
struct RIFFHeader
{
    chunk       descriptor;				// "RIFF"(资源交换文件标志)
    char        type[4];				// "WAVE"
};

//wave格式文件头
struct WAVEHeader
{
    chunk			   descriptor;		//文件描述符
    unsigned short	   audioFormat;		//音频数据格式
    unsigned short	   numChannels;		//通道数
    unsigned int	   sampleRate;		//采样率
    unsigned int	   byteRate;		//每秒数据量
    unsigned short     blockAlign;		//块对齐
    unsigned short     bitsPerSample;	//采样位数
};

//文件头
struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};

WavFile::WavFile(QObject *parent) : QFile(parent) , m_headerLength(0)
{
}

//打开文件
bool WavFile::open(const QString &fileName)
{
    close();														//打开文件之前先关闭文件
    setFileName(fileName);											//设置打开文件的路径
    return QFile::open(QIODevice::ReadOnly) && this ->readHeader();	//打开文件并且读取文件头
}

//读取文件头
bool WavFile::readHeader()
{
    seek(0);					//把文件指针指向文件首
    CombinedHeader header;
    bool result = read(reinterpret_cast<char *>(&header), sizeof (CombinedHeader)) == sizeof (CombinedHeader);
    if (result) 
	{
        if ((memcmp(header.riff.descriptor.id, "RIFF", 4) == 0					//判断文件描述符是否为"RIFF"
            || memcmp(header.riff.descriptor.id, "RIFX", 4) == 0)				//判断文件描述符是否为"RIFX"
            && memcmp(header.riff.type, "WAVE", 4) == 0							//判断是否是wave格式文件
            && memcmp(header.wave.descriptor.id, "fmt ", 4) == 0				//判断文件波形格式标志
            && (header.wave.audioFormat == 1 || header.wave.audioFormat == 0))	//判断音频数据格式
		{
			qDebug() << "文件描述符:" << header.riff.descriptor.id;
			qDebug() << "文件格式:" << header.riff.type;
			qDebug() << "文件描述符2:" << header.wave.descriptor.id;
			qDebug() << "音频数据格式:" << header.wave.audioFormat;

   //         DATAHeader dataHeader;	//数据头
   //         if (qFromLittleEndian<unsigned int>(header.wave.descriptor.size) > sizeof (WAVEHeader)) 
			//{
   //             unsigned short extraFormatBytes;	//可扩展的数据
   //             if (peek((char*)&extraFormatBytes, sizeof(unsigned short)) != sizeof (unsigned short))
			//	{
   //                 return false;
			//	}
   //             const long long throwAwayBytes = sizeof(unsigned short) + qFromLittleEndian<unsigned short>(extraFormatBytes);
   //             if (read(throwAwayBytes).size() != throwAwayBytes)
			//	{
   //                 return false;
			//	}
   //         }

   //         if (read((char*)&dataHeader, sizeof(DATAHeader)) != sizeof(DATAHeader))
			//{
   //             return false;
			//}

            //建立格式
            if (memcmp(header.riff.descriptor.id, "RIFF", 4) == 0)
			{
                m_fileFormat.setByteOrder(QAudioFormat::LittleEndian);	//设置字节顺序
			}
            else
			{
                m_fileFormat.setByteOrder(QAudioFormat::BigEndian);
			}

            int bps = qFromLittleEndian<unsigned short>(header.wave.bitsPerSample);
            m_fileFormat.setChannels(qFromLittleEndian<unsigned short>(header.wave.numChannels));			//通道
            m_fileFormat.setCodec("audio/pcm");																//编码格式
            m_fileFormat.setFrequency(qFromLittleEndian<unsigned int>(header.wave.sampleRate));				//频率
            m_fileFormat.setSampleSize(qFromLittleEndian<unsigned short>(header.wave.bitsPerSample));		//采样大小
            m_fileFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);		//采样类型
        } 
		else
		{
            result = false;
        }
    }
    m_headerLength = pos();
    return result;
}

//得到文件格式
const QAudioFormat &WavFile::fileFormat() const
{
	return m_fileFormat;
}

//得到文件头长度
long long WavFile::headerLength() const
{
	return m_headerLength;
}

//得到播放时长
long int WavFile::getMusicTime(const QString &fileName)
{
	long int lnMusicDataSize = 0;
	long int lnByteRate = 0;

	close();
	setFileName(fileName);
	QFile::open(QIODevice::ReadOnly);

	//波形数据传输速率（每秒平均字节数,单位:Byte）(1CH~1FH)
	seek(28L);
	read(reinterpret_cast<char*>(&lnByteRate), 4);

	//声音数据的总的字节数(单位:Byte)(偏移地址:28H~2BH)
	seek(40L);
	read(reinterpret_cast<char*>(&lnMusicDataSize), 4);
	close();

	return lnMusicDataSize / lnByteRate;
}
