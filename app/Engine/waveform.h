#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtGui>
#include <QtMultimedia/QAudioFormat>
#include <QPixmap>
#include <QScopedPointer>

QT_FORWARD_DECLARE_CLASS(QByteArray)

//波形类
class Waveform : public QLabel
{
    Q_OBJECT

public:
    Waveform(QWidget *parent = 0);
    ~Waveform();

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

	//初始化波形
    void initialize(const QAudioFormat &format, long long audioBufferSize, long long windowDurationUs);		
    void reset();									//重置波形
    void setAutoUpdatePosition(bool enabled);		//设置自动更新位置

public slots:
    void bufferChanged(long long position, long long length, const QByteArray &buffer);		//缓冲区改变
    void audioPositionChanged(long long position);											//音频位置改变

private:
    static const int NullIndex = -1;

    void deletePixmaps();							//删除像素图
    void createPixmaps(const QSize &newSize);		//创建所有像素图，重绘和更新显示触发更新
    void setWindowPosition(long long position);		//更新窗口位置触发更新
    long long tilePosition(int index) const;		//瓷砖的基础地位

	//标识点瓷砖的类
    struct TilePoint
    {
        TilePoint(int idx = 0, long long pos = 0, long long pix = 0)
        :   index(idx), positionOffset(pos), pixelOffset(pix)
        { }
        int index;					//瓷砖指数
        long long positionOffset;	//从瓦片的开始的字节数
        int pixelOffset;			//从像素对应的像素图的左边数
    };

	//转换中m_buffer位置成瓦片索引和以像素为单位的偏移量 
	//到相应的象素图参数位置偏移到m_buffer，
	//以字节为单位 如果位置是tile数组外，指数NullIndex和偏移为零。
    TilePoint tilePoint(long long position) const;

	//以字节为单位成砖偏移转换成像素偏移
    int tilePixelOffset(long long positionOffset) const;

	//以字节为单位到窗口偏移量转换成像素偏移
    int windowPixelOffset(long long positionOffset) const;

    bool paintTiles();						//绘制瓷砖
    void paintTile(int index);				//绘制瓷砖
	void shuffleTiles(int n);				//移动第一n瓦片到数组的末尾，并将它们标记为不涂
    void resetTiles(long long newStartPos);	//复位瓷砖阵列

private:
	long long  m_bufferPosition;
	long long  m_bufferLength;
    QByteArray m_buffer;
    long long m_audioPosition;
    QAudioFormat m_format;
    bool m_active;
    QSize m_pixmapSize;
    QVector<QPixmap*> m_pixmaps;

    struct Tile 
	{
        QPixmap *pixmap;			//指针到父m_pixmaps阵列
        bool painted;				//标志，表示该瓷砖是否已经绘
    };

    QVector<Tile> m_tiles;
    long long m_tileLength;			//在每瓦描绘字节的音频数据的长度
    long long m_tileArrayStart;		//在所述第一瓦片的字节位置上，相对于m_buffer
    long long m_windowPosition;
    long long m_windowLength;
};

#endif // WAVEFORM_H
