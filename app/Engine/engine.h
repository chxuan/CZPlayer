#ifndef ENGINE_H
#define ENGINE_H

#include "spectrum.h"
#include "spectrumanalyser.h"
#include "wavfile.h"
#include <QtCore>
#include <QtMultimedia>
#include <stdio.h>
#include <stdlib.h>

class FrequencySpectrum;
QT_FORWARD_DECLARE_CLASS(QAudioInput)
QT_FORWARD_DECLARE_CLASS(QAudioOutput)
QT_FORWARD_DECLARE_CLASS(QFile)

//播放引擎
class Engine : public QObject 
{
    Q_OBJECT

public:
    Engine(QObject *parent = 0);
    ~Engine();

    QAudio::Mode mode() const { return m_mode; }					//获取播放模式
    QAudio::State state() const { return m_state; }					//获取播放状态
    const QAudioFormat& format() const  { return m_format; }		//返回当前的音频格式
    void reset();													//重置播放引擎
    bool loadFile(const QString &fileName);							//加载文件
    bool generateTone(const Tone &tone);							//生成声音
    bool generateSweptTone(double amplitude);						//生成声音
    double rmsLevel() const { return m_rmsLevel; }					//最近处理的一组音频样本的RMS电平,在范围内的回报水平（0.0，1.0）
    double peakLevel() const  { return m_peakLevel; }				//最近处理的一组音频样本的峰值电平,在范围内的回报水平（0.0，1.0）
    long long playPosition() const  { return m_playPosition; }		//音频输出设备的位置,以字节为单位返回的位置
    long long bufferLength() const;									//播放引擎内部缓冲区长度
    long long dataLength() const  { return m_dataLength; }			//在缓冲区中保留的数据量
    void setWindowFunction(WindowFunction type);					//设置窗口功能
	void stopPlayback();											//停止播放

public slots:
    void slot_StartPlayback();											//开始播放
    void suspend();														//暂停播放

signals:
    void stateChanged(QAudio::Mode mode, QAudio::State state);			//状态改变
    void infoMessage(const QString &message, int durationMs);			//信息
    void errorMessage(const QString &heading, const QString &detail);	//错误消息
	void bufferLengthChanged(long long duration);
    void formatChanged(const QAudioFormat &format);						//音频数据格式改变
    void dataLengthChanged(long long duration);							//缓冲区数据量改变
	void recordPositionChanged(long long position);						//记录位置发生改变
    void playPositionChanged(long long position);							//音频输出装置的位置发生了变化
	//频谱参数改变
    void spectrumChanged(long long position, long long length, const FrequencySpectrum &spectrum);
	void bufferChanged(long long position, long long length, const QByteArray &buffer);
	void sig_Finished();												//播放完成

private slots:
    void audioNotify();													//音频通知
    void audioStateChanged(QAudio::State state);						//音频状态改变
    void spectrumChanged(const FrequencySpectrum &spectrum);			//频谱改变

private:
	void resetAudioDevices();											//重置音频设备
    bool initialize();													//初始化播放引擎
    bool selectFormat();												//选择音频数据格式
    void setState(QAudio::State state);									//设置状态
    void setState(QAudio::Mode mode, QAudio::State state);				//设置状态
    void setFormat(const QAudioFormat &format);							//设置音频数据格式
	void setRecordPosition(long long position, bool forceEmit = false);	//设置记录位置
    void setPlayPosition(long long position, bool forceEmit = false);	//设置音频输出位置
    void calculateSpectrum(long long position);							//计算频谱

private:
    QAudio::Mode        m_mode;							//音频模式
    QAudio::State       m_state;						//音频状态
    bool                m_generateTone;					//是否生成声音
    SweptTone           m_tone;							//生成声音
    WavFile*            m_file;							//wav文件指针
    WavFile*            m_analysisFile;					//第二个wav文件指针，主要用于把文件里面的内容读出放入m_buffer
    QAudioFormat        m_format;						//音频数据格式
    QAudioDeviceInfo    m_audioInputDevice;				//音频输入设备信息
    QAudioInput*        m_audioInput;					//音频输入
    QAudioDeviceInfo    m_audioOutputDevice;			//音频输出设备信息
    QAudioOutput*       m_audioOutput;					//音频输出
    long long           m_playPosition;					//音频播放位置
    QBuffer             m_audioOutputIODevice;			//音频输出设备IO缓冲区
    QByteArray          m_buffer;						//二进制数组缓冲区
    long long           m_bufferPosition;				//缓冲区位置
    long long           m_bufferLength;					//缓冲区长度
    long long           m_dataLength;					//数据部分长度
	long long             m_recordPosition;				//记录位置
    int                 m_levelBufferLength;			//水平缓冲区长度
    double              m_rmsLevel;						//RMS电平
    double              m_peakLevel;					//峰值电平
    int                 m_spectrumBufferLength;			//频谱缓冲区长度
    QByteArray          m_spectrumBuffer;				//频谱缓冲区
    SpectrumAnalyser    m_spectrumAnalyser;				//频谱分析仪
    long long           m_spectrumPosition;				//频谱位置
    int					m_count;						//数量
};

#endif // ENGINE_H
