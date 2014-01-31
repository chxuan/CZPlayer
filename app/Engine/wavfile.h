#ifndef WAVFILE_H
#define WAVFILE_H

#include <QtCore/qobject.h>
#include <QtCore/qfile.h>
#include <QtMultimedia/qaudioformat.h>

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = 0);

    bool open(const QString &fileName);			//打开文件
    const QAudioFormat &fileFormat() const;		//得到文件格式
    long long headerLength() const;				//得到文件头长度

	long int getMusicTime(const QString &fileName);					//得到播放时长

private:
    bool readHeader();							//读取文件头

private:
    QAudioFormat m_fileFormat;					//文件音频数据格式
    long long m_headerLength;					//文件头长度
};

#endif

