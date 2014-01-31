#include "stdafx.h"
#include "engine.h"
#include "tonegenerator.h"
#include "utils.h"
#include <math.h>
#include <QMetaObject>
#include <QSet>
#include <QThread>

const long long BufferDurationUs = 10 * 1000000;	//缓冲区时间
const int NotifyIntervalMs = 100;					//通知间隔(也是频谱更新时间间隔)
const int LevelWindowUs = 0.1 * 1000000;			//在微秒级计算窗口的大小

//重新实现"<<"操作符
QDebug& operator<<(QDebug &debug, const QAudioFormat &format)
{
    debug << format.frequency() << "Hz"
          << format.channels() << "channels";
    return debug;
}

//播放引擎构造函数
Engine::Engine(QObject *parent)
    :   QObject(parent)
    ,   m_mode(QAudio::AudioInput)
    ,   m_state(QAudio::StoppedState)
    ,   m_generateTone(false)
    ,   m_file(0)
    ,   m_analysisFile(0)
    ,   m_audioInputDevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(0)
    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_playPosition(0)
	,	m_recordPosition(0)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_dataLength(0)
    ,   m_levelBufferLength(0)
    ,   m_rmsLevel(0.0)
    ,   m_peakLevel(0.0)
    ,   m_spectrumBufferLength(0)
    ,   m_spectrumAnalyser()
    ,   m_spectrumPosition(0)
    ,   m_count(0)
{
	//注册Qt信号槽参数
    qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");
    qRegisterMetaType<WindowFunction>("WindowFunction");
    connect(&m_spectrumAnalyser, SIGNAL(spectrumChanged(FrequencySpectrum)), this, SLOT(spectrumChanged(FrequencySpectrum)));	//频谱改变
    this ->initialize();	//初始化播放引擎
}

Engine::~Engine()
{

}

//加载文件
bool Engine::loadFile(const QString &fileName)
{
    this ->reset();	//重置播放引擎
    bool result = false;
    m_file = new WavFile(this);						//新建一个wav文件
    if (m_file ->open(fileName))					//打开文件
	{
        if (isPCMS16LE(m_file ->fileFormat()))		//判断文件格式是否为pcm16le
		{
            result = this ->initialize();			//初始化播放引擎
        }
		else 
		{
            emit errorMessage(tr("不支持的音频格式"), formatToString(m_file ->fileFormat()));
        }
    }
	else
	{
        emit errorMessage(tr("不能够打开文件"), fileName);
    }
    if (result)		//初始化播放引擎成功
	{	
		//分析文件指针
        m_analysisFile = new WavFile(this);
        m_analysisFile ->open(fileName);
    }
    return result;
}

//初始化播放引擎
bool Engine::initialize()
{
	bool result = false;			//初始化播放引擎失败
	QAudioFormat format = m_format;	//音频数据格式

	if (this ->selectFormat())		//选择音频数据格式
	{
		if (m_format != format)
		{
			this ->resetAudioDevices();	//重置音频设备
			if (m_file)
			{
				emit bufferLengthChanged(bufferLength());
				emit dataLengthChanged(dataLength());
				emit bufferChanged(0, 0, m_buffer);
				this ->setRecordPosition(bufferLength());
				result = true;
			}
			else
			{
				m_bufferLength = audioLength(m_format, BufferDurationUs);	//缓冲区长度
				m_buffer.resize(m_bufferLength);							//重置缓冲区
				m_buffer.fill(0);											//初始化缓冲区
				emit bufferLengthChanged(bufferLength());
				if (m_generateTone)
				{
					if (0 == m_tone.endFreq) 
					{
						const double nyquist = nyquistFrequency(m_format);
						m_tone.endFreq = qMin(double(SpectrumHighFreq), nyquist);
					}
					//在utils.h调用函数中定义，在全局范围内
					::generateTone(m_tone, m_format, m_buffer);
					m_dataLength = m_bufferLength;
					emit dataLengthChanged(dataLength());
					emit bufferChanged(0, m_dataLength, m_buffer);
					this ->setRecordPosition(m_bufferLength);
					result = true;
				} 
				else
				{
					emit bufferChanged(0, 0, m_buffer);
					m_audioInput = new QAudioInput(m_audioInputDevice, m_format, this);		//音频输入
					m_audioInput ->setNotifyInterval(NotifyIntervalMs);						//设定通知时间间隔
					result = true;
				}
			}
			m_audioOutput = new QAudioOutput(m_audioOutputDevice, m_format, this);			//音频输出
			m_audioOutput->setNotifyInterval(NotifyIntervalMs);								//设定通知时间间隔
		}
	}
	else	//选择音频格式失败
	{
		if (m_file)
		{
			emit errorMessage(tr("不支持的音频格式"), formatToString(m_format));
		}
		else if (m_generateTone)
		{
			emit errorMessage(tr("没有找到合适的格式"), "");
		}
		else
		{
			emit errorMessage(tr("找不到共同的输入/输出格式"), "");
		}
	}

	qDebug() << "Engine::initialize" << "缓冲区大小:" << m_bufferLength;
	qDebug() << "Engine::initialize" << "数据量大小:" << m_dataLength;
	qDebug() << "Engine::initialize" << "音频数据格式:" << m_format;

	return result;
}

//选择音频数据格式
bool Engine::selectFormat()
{
	bool foundSupportedFormat = false;		//默认的音频输出设备不支持该音频格式
	if (m_file || QAudioFormat() != m_format) 
	{
		QAudioFormat format = m_format;
		if (m_file)
		{
			//报头是从WAV文件读取，只需要检查是否它支持的音频输出装置
			format = m_file ->fileFormat();
		}
		if (m_audioOutputDevice.isFormatSupported(format)) 
		{
			this ->setFormat(format);		//设置音频数据格式
			foundSupportedFormat = true;	//默认的音频输出设备支持该音频格式
		}
	}
	else 
	{
		QList<int> frequenciesList;											//频率列表

#ifdef Q_OS_WIN
		//Windows的音频后端不正确报告格式支持（见QTBUG-9100）。此外，虽然音频子系统捕获在11025Hz，由此产生的音频被损坏
		frequenciesList += 8000;
#endif

		if (!m_generateTone)
		{
			frequenciesList += m_audioInputDevice.supportedFrequencies();	//获取输入设备支持的频率
		}

		frequenciesList += m_audioOutputDevice.supportedFrequencies();		//获取输出设备支持的频率
		frequenciesList = frequenciesList.toSet().toList();					//删除重复
		qSort(frequenciesList);												//对频率列表排序
		qDebug() << "Engine::initialize 输入输出设备支持的取样频率:" << frequenciesList;

		QList<int> channelsList;											//通道列表
		channelsList += m_audioInputDevice.supportedChannels();				//获取输入设备支持的通道
		channelsList += m_audioOutputDevice.supportedChannels();			//获取输出设备支持的通道
		channelsList = channelsList.toSet().toList();						//删除重复
		qSort(channelsList);												//对通道列表排序
		qDebug() << "Engine::initialize 输入输出设备支持的通道:" << channelsList;

		//设置音频数据格式
		QAudioFormat format;
		format.setByteOrder(QAudioFormat::LittleEndian);					//设置字节顺序
		format.setCodec("audio/pcm");										//编码格式
		format.setSampleSize(16);											//采样大小
		format.setSampleType(QAudioFormat::SignedInt);						//采样类型

		int frequency, channels;
		//遍历频率列表
		foreach (frequency, frequenciesList)
		{
			if (foundSupportedFormat)
			{
				break;
			}
			format.setFrequency(frequency);									//频率

			//遍历通道列表
			foreach (channels, channelsList) 
			{
				format.setChannels(channels);								//通道
				const bool inputSupport = m_generateTone || m_audioInputDevice.isFormatSupported(format);	//输入设备支持
				const bool outputSupport = m_audioOutputDevice.isFormatSupported(format);					//输出设备支持
				qDebug() << "Engine::initialize 音频数据格式:" << format << ",输入:" << inputSupport << ",输出:" << outputSupport;
				if (inputSupport && outputSupport)
				{
					foundSupportedFormat = true;	//默认的音频输出设备支持该音频格式
					break;
				}
			}
		}
		if (!foundSupportedFormat)
		{
			format = QAudioFormat();
		}
		this ->setFormat(format);		//设置音频数据格式
	}
	return foundSupportedFormat;
}

//播放
void Engine::slot_StartPlayback()
{
	if (m_audioOutput) 
	{
		//如果当前模式为输出模式并且当前状态为暂停状态
		if (QAudio::AudioOutput == m_mode && QAudio::SuspendedState == m_state)
		{
			m_audioOutput ->resume();	//重新开始,继续播放
		}
		//当前状态为停止状态
		else if (QAudio::StoppedState == m_state)
		{
			//第一次播放
			m_spectrumAnalyser.cancelCalculation();					//取消频谱计算
			this ->spectrumChanged(0, 0, FrequencySpectrum());		//初始化频谱
			this ->setPlayPosition(0, true);						//设置音频开始位置
			m_mode = QAudio::AudioOutput;							//当前模式为输出模式

			connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStateChanged(QAudio::State)));	//音频状态改变
			connect(m_audioOutput, SIGNAL(notify()), this, SLOT(audioNotify()));										//音频通知(每100ms通知一次)
			m_count = 0;
			if (m_file) 
			{
				m_file ->seek(0);				//文件指针指向文件头
				m_bufferPosition = 0;			//设置缓冲区位置
				m_dataLength = 0;				//设置数据长度
				m_audioOutput ->start(m_file);	//开始播放
			} 
			else 
			{
				m_audioOutputIODevice.close();
				m_audioOutputIODevice.setBuffer(&m_buffer);
				m_audioOutputIODevice.open(QIODevice::ReadOnly);
				m_audioOutput ->start(&m_audioOutputIODevice);
			}
		}
	}
}

//暂停播放
void Engine::suspend()
{
	//如果当前状态为播放状态
	if (QAudio::ActiveState == m_state || QAudio::IdleState == m_state) 
	{
		switch (m_mode) 
		{
		case QAudio::AudioInput:	//输入模式
			m_audioInput ->suspend();
			break;
		case QAudio::AudioOutput:	//输出模式
			m_audioOutput ->suspend();
			break;
		default:
			break;
		}
	}
}

//停止播放
void Engine::stopPlayback()
{
	if (m_audioOutput)
	{
		m_audioOutput ->stop();		//停止播放
		QCoreApplication::instance() ->processEvents();
		m_audioOutput ->disconnect();
		this ->setPlayPosition(0);	//重置播放位置
	}
}

//重置音频设备
void Engine::resetAudioDevices()
{
	delete m_audioInput;
	m_audioInput = 0;
	this ->setRecordPosition(0);
	delete m_audioOutput;
	m_audioOutput = 0;
	this ->setPlayPosition(0);
	m_spectrumPosition = 0;
}

//重置播放引擎
void Engine::reset()
{
	this ->stopPlayback();											//停止播放
	this ->setState(QAudio::AudioInput, QAudio::StoppedState);		//设置状态
	this ->setFormat(QAudioFormat());								//设置音频格式
	m_generateTone = false;											//没有生成声音
	if (m_file) { delete m_file; m_file = 0; }
	if (m_analysisFile) { delete m_analysisFile; m_analysisFile = 0; }
	m_buffer.clear();												//清空缓冲区
	m_bufferPosition = 0;											//缓冲区位置
	m_bufferLength = 0;												//缓冲区长度
	m_dataLength = 0;												//数据部分长度
	emit dataLengthChanged(0);										//发射数据量改变信号
	this ->resetAudioDevices();										//重置音频设备
}

//设置记录位置
void Engine::setRecordPosition(qint64 position, bool forceEmit)
{
	const bool changed = (m_recordPosition != position);
	m_recordPosition = position;
	if (changed || forceEmit)
	{
		emit recordPositionChanged(m_recordPosition);
	}
}

//设置播放位置
void Engine::setPlayPosition(long long position, bool forceEmit)
{
	const bool changed = (m_playPosition != position);
	m_playPosition = position;
	if (changed || forceEmit)
	{
		emit playPositionChanged(m_playPosition);
	}
}

//生成声音
bool Engine::generateTone(const Tone &tone)
{
    this ->reset();
    Q_ASSERT(!m_generateTone);
    Q_ASSERT(!m_file);
    m_generateTone = true;
    m_tone = tone;
    qDebug() << "Engine::generateTone"
                 << "startFreq" << m_tone.startFreq
                 << "endFreq" << m_tone.endFreq
                 << "amp" << m_tone.amplitude;
    return this ->initialize();
}

//清除声音
bool Engine::generateSweptTone(double amplitude)
{
    Q_ASSERT(!m_generateTone);
    Q_ASSERT(!m_file);
    m_generateTone = true;
    m_tone.startFreq = 1;
    m_tone.endFreq = 0;
    m_tone.amplitude = amplitude;
    qDebug() << "Engine::generateSweptTone"
                 << "startFreq" << m_tone.startFreq
                 << "amp" << m_tone.amplitude;
    return this ->initialize();
}

//获取播放引擎内部缓冲区长度
long long Engine::bufferLength() const
{
    return m_file ? m_file->size() : m_bufferLength;
}

//设置窗口功能
void Engine::setWindowFunction(WindowFunction type)
{
    m_spectrumAnalyser.setWindowFunction(type);
}

//音频通知(每100ms通知一次)
void Engine::audioNotify()
{
    switch (m_mode) 
	{
    case QAudio::AudioOutput:	//输出模式
		{
            const long long playPosition = audioLength(m_format, m_audioOutput->processedUSecs());	//获取播放位置
            this ->setPlayPosition(qMin(bufferLength(), playPosition));								//设置播放位置
            const long long levelPosition = playPosition - m_levelBufferLength;						//获取水平位置
            const long long spectrumPosition = playPosition - m_spectrumBufferLength;				//获取频谱位置
            if (m_file) 
			{
                if (levelPosition > m_bufferPosition || spectrumPosition > m_bufferPosition || qMax(m_levelBufferLength, m_spectrumBufferLength) > m_dataLength)
				{
                    m_bufferPosition = 0;	//缓冲区位置
                    m_dataLength = 0;		//数据长度

                    //数据需要被读入到m_buffer以待分析
                    const long long readPos = qMax(long long(0), qMin(levelPosition, spectrumPosition));		//获取阅读开始位置
                    const long long readEnd = qMin(m_analysisFile ->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));	//获取阅读结束位置
                    const long long readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);//获取阅读长度
                    //qDebug() << "Engine::audioNotify [1]"
                    //         << "文件大小:" << m_analysisFile ->size()
                    //         << "已读位置:" << readPos
                    //         << "读取长度:" << readLen;
                    if (m_analysisFile ->seek(readPos + m_analysisFile ->headerLength())) 
					{
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile ->read(m_buffer.data(), readLen);
                        //qDebug() << "Engine::audioNotify [2]" << "缓冲区位置:" << m_bufferPosition << "数据量:" << m_dataLength;
                    } 
					else 
					{
                        qDebug() << "Engine::audioNotify [2]" << "文件寻找失败";
                    }
					emit bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                }
            } 
			else
			{	
				//如果播放的位置大于等于数据长度则停止播放
                if (playPosition >= m_dataLength)
				{
                    this ->stopPlayback();
				}
            }
            if (spectrumPosition >= 0 && spectrumPosition + m_spectrumBufferLength < m_bufferPosition + m_dataLength)
			{
                this ->calculateSpectrum(spectrumPosition);	//计算频谱
			}
        }
        break;
    }
}

//计算频谱
void Engine::calculateSpectrum(long long position)
{
	//qDebug() << "Engine::calculateSpectrum" << QThread::currentThread()
	//	<< "数量:" << m_count << "频谱位置:" << position << "频谱缓冲区长度:" << m_spectrumBufferLength
	//	<< "频谱分析仪是否就绪:" << m_spectrumAnalyser.isReady();

	//判断频谱分析仪是否开始工作
	if (m_spectrumAnalyser.isReady()) 
	{
		m_spectrumBuffer = QByteArray::fromRawData(m_buffer.constData() + position - m_bufferPosition, m_spectrumBufferLength);
		m_spectrumPosition = position;
		m_spectrumAnalyser.calculate(m_spectrumBuffer, m_format);	//频谱分析仪进行计算
	}
}

//音频状态改变
void Engine::audioStateChanged(QAudio::State state)
{
    qDebug() << "Engine::audioStateChanged from" << m_state << "to" << state;

    if (QAudio::IdleState == state && m_file && m_file ->pos() == m_file->size())
	{
        this ->stopPlayback();	//停止播放
		emit sig_Finished();	//发送播放完成信号
		qDebug() << "完成!";
    } 
	else
	{
        if (QAudio::StoppedState == state) 
		{
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (m_mode) 
			{
            case QAudio::AudioInput:
                error = m_audioInput->error();
                break;
            case QAudio::AudioOutput:
                error = m_audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) 
			{
                this ->reset();
                return;
            }
        }
        this ->setState(state);	//设置状态
    }
}

//频谱改变
void Engine::spectrumChanged(const FrequencySpectrum &spectrum)
{
    //qDebug() << "Engine::spectrumChanged" << "位置:" << m_spectrumPosition;
    emit spectrumChanged(m_spectrumPosition, m_spectrumBufferLength, spectrum);	//发射频谱改变信号
}

//设置状态
void Engine::setState(QAudio::State state)
{
    const bool changed = (m_state != state);
    m_state = state;
    if (changed)
	{
        emit stateChanged(m_mode, m_state);
	}
}

//设置状态
void Engine::setState(QAudio::Mode mode, QAudio::State state)
{
    const bool changed = (m_mode != mode || m_state != state);
    m_mode = mode;
    m_state = state;
    if (changed)
	{
        emit stateChanged(m_mode, m_state);
	}
}

//设置音频格式
void Engine::setFormat(const QAudioFormat &format)
{
    const bool changed = (format != m_format);						//判断格式是否已经改变
    m_format = format;												//设置音频格式
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);		//获得水平缓冲区长度
    m_spectrumBufferLength = SpectrumLengthSamples * (m_format.sampleSize() / 8) * m_format.channels();	//频谱缓冲区长度
    if (changed)
	{
        emit formatChanged(m_format);		//发射音频格式改变信号
	}
}
