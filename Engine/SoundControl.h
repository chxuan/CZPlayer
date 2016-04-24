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
* \file   SoundControl.h
* \author chengxuan   787280310@qq.com
* \date   2015-02-06
* \brief  音量控制类头文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-02-06    by chengxuan
*
* \endverbatim
*
*/

#ifndef SOUNDCONTROL_H 
#define SOUNDCONTROL_H   

/*! \def SOUNDCONTROL_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define SOUNDCONTROL_VERSION   0x030000

/*! 音量控制类
*/
class SoundControl
{
public:
	/*! 设置设备的音量
	 *  \param volume 表示音量的大小, 设置与返回音量的值用的是百分比，即音量从0 -100，而不是设备的绝对值   
	 *  \param device 设备类型 0,1,2 分别表示主音量, 波形,MIDI,LINE,IN,默认为0
	 * \return true 表示设置音量的大小的操作成功，否则失败
	 */
	static bool setVolume(long volume, long device = 0);

	/*! 得到设备的音量
	 *  \param device 设备类型 0,1,2 分别表示主音量, 波形,MIDI,LINE,IN,默认为0
	 *  \return 音量
	 */
	static unsigned getVolume(int dev);			

private:
	/*! 构造函数.*/
	SoundControl();

	/*! 析构函数.*/
	~SoundControl();

	/*! 得到音量控制
	 *  \param hmixer HMIXER混合器
	 *  \param componentType 组成类型
	 *  \param ctrlType 控制类型
	 *  \param mxc MIXERCONTROL类型
	 * \return true代表成功，否则失败
	 */
	static bool getVolumeControl(HMIXER hmixer , long componentType, long ctrlType, MIXERCONTROL *mxc); 

	/*! 得到音量
	 *  \param hmixer HMIXER混合器
	 *  \param mxc MIXERCONTROL类型
	 * \return 音量
	 */
	static unsigned getVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc);   

	/*! 得到音量
	 *  \param hmixer HMIXER混合器
	 *  \param mxc MIXERCONTROL类型
	 *  \param volume 音量
	 *  \return true代表成功，否则失败
	 */
	static bool setVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume);   
};
 
#endif
