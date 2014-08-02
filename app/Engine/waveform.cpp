#include "StdAfx.h"
#include "waveform.h"
#include "utils.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

Waveform::Waveform(QWidget *parent)
    :   QLabel(parent)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_audioPosition(0)
    ,   m_active(false)
    ,   m_tileLength(0)
    ,   m_tileArrayStart(0)
    ,   m_windowPosition(0)
    ,   m_windowLength(0)
{
	this->resize(190, 78);
}

Waveform::~Waveform()
{
    this->deletePixmaps();
}

void Waveform::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    //painter.fillRect(rect(), Qt::black);

    if (m_active)
	{
        long long pos = m_windowPosition;
        const long long windowEnd = m_windowPosition + m_windowLength;
        int destLeft = 0;
        int destRight = 0;
        while (pos < windowEnd)
		{
            const TilePoint point = tilePoint(pos);

            if (point.index != NullIndex) 
			{
                const Tile &tile = m_tiles[point.index];
                if (tile.painted)
				{
                    const long long sectionLength = qMin((m_tileLength - point.positionOffset), (windowEnd - pos));
                    Q_ASSERT(sectionLength > 0);

                    const int sourceRight = tilePixelOffset(point.positionOffset + sectionLength);
                    destRight = windowPixelOffset(pos - m_windowPosition + sectionLength);

                    QRect destRect = rect();
                    destRect.setLeft(destLeft);
                    destRect.setRight(destRight);

                    QRect sourceRect(QPoint(), m_pixmapSize);
                    sourceRect.setLeft(point.pixelOffset);
                    sourceRect.setRight(sourceRight);

                    painter.drawPixmap(destRect, *tile.pixmap, sourceRect);

                    destLeft = destRight;
                    if (point.index < m_tiles.count())
					{
                        pos = tilePosition(point.index + 1);
                       // qDebug() << "Waveform::paintEvent" << "pos->" << pos;
                    } 
					else 
					{
                        //qDebug() << "Waveform::paintEvent" << "reached end of tile array";
                        break;
                    }
                }
				else 
				{
                    //qDebug() << "Waveform::paintEvent" << "tile" << point.index << "not painted";
                    break;
                }
            } 
			else
			{
                //qDebug() << "Waveform::paintEvent" << "pos" << pos << "past end of tile array";
                break;
            }
        }

        //qDebug() << "Waveform::paintEvent" << "final pos" << pos << "final x" << destRight;
    }
}

void Waveform::resizeEvent(QResizeEvent *event)
{
    if (event->size() != event->oldSize())
	{
        createPixmaps(event->size());
	}
}

void Waveform::initialize(const QAudioFormat &format, long long audioBufferSize, long long windowDurationUs)
{
    this->reset();
    m_format = format;
    m_tileLength = audioBufferSize;								//计算平铺尺寸
    m_windowLength = audioLength(m_format, windowDurationUs);	//计算窗口大小

    //计算所需的瓷砖数量
    int nTiles;
    if (m_tileLength > m_windowLength)
	{
        nTiles = 2;
    } 
	else 
	{
        nTiles = m_windowLength / m_tileLength + 1;
        if (m_windowLength % m_tileLength)
		{
            ++nTiles;
		}
    }

    m_pixmaps.fill(0, nTiles);
    m_tiles.resize(nTiles);
    this->createPixmaps(rect().size());
    m_active = true;
}

//重置波形
void Waveform::reset()
{
    m_bufferPosition = 0;
    m_buffer = QByteArray();
    m_audioPosition = 0;
    m_format = QAudioFormat();
    m_active = false;
    deletePixmaps();
    m_tiles.clear();
    m_tileLength = 0;
    m_tileArrayStart = 0;
    m_windowPosition = 0;
    m_windowLength = 0;
}

//缓冲区改变
void Waveform::bufferChanged(long long position, long long length, const QByteArray &buffer)
{
    m_bufferPosition = position;
    m_bufferLength = length;
    m_buffer = buffer;
    paintTiles();
}

//音频位置改变
void Waveform::audioPositionChanged(long long position)
{
    if (position >= m_bufferPosition) 
	{
        if (position + m_windowLength > m_bufferPosition + m_bufferLength)
		{
            position = qMax(long long(0), m_bufferPosition + m_bufferLength - m_windowLength);
		}
        m_audioPosition = position;
        setWindowPosition(position);
    }
}

//删除像素图
void Waveform::deletePixmaps()
{
    QPixmap *pixmap;
    foreach (pixmap, m_pixmaps)
	{
        delete pixmap;
	}
    m_pixmaps.clear();
}

//创建所有像素图，重绘和更新显示触发更新
void Waveform::createPixmaps(const QSize &widgetSize)
{
    m_pixmapSize = widgetSize;
    m_pixmapSize.setWidth(double(widgetSize.width()) * m_tileLength / m_windowLength);

    Q_ASSERT(m_tiles.count() == m_pixmaps.count());

    //创建所有像素图
    for (int i=0; i<m_pixmaps.size(); ++i)
	{
        delete m_pixmaps[i];
        m_pixmaps[i] = 0;
        m_pixmaps[i] = new QPixmap(m_pixmapSize);
    }

    //为重新粉刷瓷砖更新pixmap的指针和标志
    for (int i=0; i<m_tiles.count(); ++i) 
	{
        m_tiles[i].pixmap = m_pixmaps[i];
        m_tiles[i].painted = false;
    }
}

//更新窗口位置触发更新
void Waveform::setWindowPosition(long long position)
{
    const long long oldPosition = m_windowPosition;
    m_windowPosition = position;

    if ((m_windowPosition >= oldPosition) &&
        (m_windowPosition - m_tileArrayStart < (m_tiles.count() * m_tileLength)))
	{
        // Work out how many tiles need to be shuffled
        const long long offset = m_windowPosition - m_tileArrayStart;
        const int nTiles = offset / m_tileLength;
        shuffleTiles(nTiles);
    } 
	else
	{
        resetTiles(m_windowPosition);
    }

    if (!paintTiles() && m_windowPosition != oldPosition)
	{
        update();
	}
}

//瓷砖的基础地位
long long Waveform::tilePosition(int index) const
{
    return m_tileArrayStart + index * m_tileLength;
}

//转换中m_buffer位置成瓦片索引和以像素为单位的偏移量 
//到相应的象素图参数位置偏移到m_buffer，
//以字节为单位 如果位置是tile数组外，指数NullIndex和偏移为零。
Waveform::TilePoint Waveform::tilePoint(long long position) const
{
    TilePoint result;
    if (position >= m_tileArrayStart) 
	{
        const long long tileArrayEnd = m_tileArrayStart + m_tiles.count() * m_tileLength;
        if (position < tileArrayEnd)
		{
            const long long offsetIntoTileArray = position - m_tileArrayStart;
            result.index = offsetIntoTileArray / m_tileLength;
            Q_ASSERT(result.index >= 0 && result.index <= m_tiles.count());
            result.positionOffset = offsetIntoTileArray % m_tileLength;
            result.pixelOffset = tilePixelOffset(result.positionOffset);
            Q_ASSERT(result.pixelOffset >= 0 && result.pixelOffset <= m_pixmapSize.width());
        }
    }

    return result;
}

//以字节为单位成砖偏移转换成像素偏移
int Waveform::tilePixelOffset(long long positionOffset) const
{
    Q_ASSERT(positionOffset >= 0 && positionOffset <= m_tileLength);
    const int result = (double(positionOffset) / m_tileLength) * m_pixmapSize.width();
    return result;
}

//以字节为单位到窗口偏移量转换成像素偏移
int Waveform::windowPixelOffset(long long positionOffset) const
{
    Q_ASSERT(positionOffset >= 0 && positionOffset <= m_windowLength);
    const int result = (double(positionOffset) / m_windowLength) * rect().width();
    return result;
}

//绘制瓷砖
bool Waveform::paintTiles()
{
    //qDebug() << "Waveform::paintTiles";
    bool updateRequired = false;

    for (int i = 0; i < m_tiles.count(); ++i) 
	{
        const Tile &tile = m_tiles[i];
        if (!tile.painted)
		{
            const long long tileStart = m_tileArrayStart + i * m_tileLength;
            const long long tileEnd = tileStart + m_tileLength;
            if (m_bufferPosition <= tileStart && m_bufferPosition + m_bufferLength >= tileEnd)
			{
                paintTile(i);
                updateRequired = true;
            }
        }
    }

    if (updateRequired)
	{
        update();
	}

    return updateRequired;
}

//绘制瓷砖
void Waveform::paintTile(int index)
{
    const long long tileStart = m_tileArrayStart + index * m_tileLength;
    //qDebug() << "Waveform::paintTile"
    //               << "index" << index
    //               << "bufferPosition" << m_bufferPosition
    //               << "bufferLength" << m_bufferLength
    //               << "start" << tileStart
    //               << "end" << tileStart + m_tileLength;

    Q_ASSERT(m_bufferPosition <= tileStart);
    Q_ASSERT(m_bufferPosition + m_bufferLength >= tileStart + m_tileLength);

    Tile &tile = m_tiles[index];
    Q_ASSERT(!tile.painted);

    const short* base = reinterpret_cast<const short*>(m_buffer.constData());
    const short* buffer = base + ((tileStart - m_bufferPosition) / 2);
    const int numSamples = m_tileLength / (2 * m_format.channels());

    QPainter painter(tile.pixmap);

    //painter.fillRect(tile.pixmap->rect(), Qt::black);

    //QPen pen(Qt::white);
	QPen pen(QColor(5, 184, 204));
    painter.setPen(pen);

    //计算初始值的PCM
    short previousPcmValue = 0;
    if (buffer > base)
	{
        previousPcmValue = *(buffer - m_format.channels());
	}

    //计算初始点
    const double previousRealValue = pcmToReal(previousPcmValue);
    const int originY = ((previousRealValue + 1.0) / 2) * m_pixmapSize.height();
    const QPoint origin(0, originY);

    QLine line(origin, origin);

    for (int i = 0; i < numSamples; ++i)
	{
        const short* ptr = buffer + i * m_format.channels();

        const int offset = reinterpret_cast<const char*>(ptr) - m_buffer.constData();
        Q_ASSERT(offset >= 0);
        Q_ASSERT(offset < m_bufferLength);

        const short pcmValue = *ptr;
        const double realValue = pcmToReal(pcmValue);

        const int x = tilePixelOffset(i * 2 * m_format.channels());
        const int y = ((realValue + 1.0) / 2) * m_pixmapSize.height();

        line.setP2(QPoint(x, y));
        painter.drawLine(line);
        line.setP1(line.p2());
    }
    tile.painted = true;
}

//移动第一n瓦片到数组的末尾，并将它们标记为不涂
void Waveform::shuffleTiles(int n)
{
    //qDebug() << "Waveform::shuffleTiles" << "n" << n;
    while (n--)
	{
        Tile tile = m_tiles.first();
        tile.painted = false;
        m_tiles.erase(m_tiles.begin());
        m_tiles += tile;
        m_tileArrayStart += m_tileLength;
    }
    //qDebug() << "Waveform::shuffleTiles" << "tileArrayStart" << m_tileArrayStart;
}

//复位瓷砖阵列
void Waveform::resetTiles(long long newStartPos)
{
    //qDebug() << "Waveform::resetTiles" << "newStartPos" << newStartPos;
    QVector<Tile>::iterator i = m_tiles.begin();
    for ( ; i != m_tiles.end(); ++i)
	{
        i->painted = false;
	}
    m_tileArrayStart = newStartPos;
}

