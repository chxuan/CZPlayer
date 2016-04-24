/***************************************************************************
 *   Copyright (C) 2012-2015 Highway-9 Studio.                             *
 *   787280310@qq.com									                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   CUIT Highway-9 Studio, China.									       *
 ***************************************************************************/

/*!
 * \file   RadioPanel.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  电台面板头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-02-23    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef RADIOPANEL_H
#define RADIOPANEL_H

/*! \def RADIOPANEL_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define RADIOPANEL_VERSION   0x030000

#include <QWidget>
#include <QList>
#include <QMap>
#include <QMediaPlayer>
#include "RadioInfoEntity.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if RADIOINFOENTITY_VERSION < 0x030000
#error  "RadioInfoEntity version error."
#endif

class QLabel;
class QTableWidget;
class QPushButton;

/*! 电台面板
*/
class RadioPanel : public QWidget
{
    Q_OBJECT

public:
    /*! 构造函数
    *  \param parent 父窗口指针，默认为NULL
    */
    RadioPanel(QWidget *parent = NULL);

    /*! 析构函数.*/
    ~RadioPanel();

private slots:
    /*! 双击电台响应函数
    *  \param row 行
    */
    void handlePlayRadio(const int &row);

    /*! 播放电台
    */
    void handlePlayRadio();

    /*! 上一电台
    */
    void handlePreRadio();

    /*! 下一电台
    */
    void handleNextRadio();

    /*! 播放错误
    *  \param error 错误类型
    */
    void handleError(QMediaPlayer::Error error);

private:
    /*! 创建界面
     */
    void createUI();

    /*! 信号与槽
     */
    void createConnect();

    /*! 加载电台信息
    *  \param entity 电台信息实体
    */
    void addRadioInfo(const RadioInfoEntity &entity);

    /*! 设置表格风格
     */
    void setTableWidgetStyle();

    /*! 读取radio配置文件
    *  \param filePath 文件路径
    *  \return radio信息集合
    */
    QList<RadioInfoEntity> readXML(const QString &filePath);

    /*! 错误代码转换成字符串
     *  \param networkError 网络错误代码
     */
    QString errorCodeToString(QMediaPlayer::Error error);

private:
    QTableWidget *m_radioTable;                 /*!< radio表格*/
    QMap<int, RadioInfoEntity> m_radioInfoMap;  /*!< key：表格行数，value：电台信息实体*/

    QMediaPlayer *m_radioPlayer;                 /*!< 电台播放器*/

    QPushButton *m_playButton;                  /*!< 播放/暂停按钮*/
    QPushButton *m_preButton;                   /*!< 上一首*/
    QPushButton *m_nextButton;                  /*!< 下一首*/

    QLabel *m_currentRadioLabel;                /*!< 当前电台*/
    RadioInfoEntity m_radioInfo;                /*!< 当前电台信息*/
    int m_currentRow;                           /*!< 当前播放行*/

    /*! 播放模式*/
    enum PlayStatus
    {
        Playing = 1,     /*!< 播放*/
        Stoped = 2         /*!< 停止*/
    };
    enum PlayStatus m_playStatus;   /*!< 播放模式*/
};

#endif // RADIOPANEL_H
