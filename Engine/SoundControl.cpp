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
* \file   SoundControl.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-02-06
* \brief  音量控制类实现文件。
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

#include <Windows.h>
#include <mmreg.h>
#include <mmsystem.h>
#include "SoundControl.h"

SoundControl::SoundControl()
{
	/*Do nothing.*/
}

SoundControl::~SoundControl()
{
	/*Do nothing.*/
}

unsigned SoundControl::getVolume(int dev)
{   
	long device;
	unsigned rt = 0;     
	MIXERCONTROL volCtrl;     
	HMIXER hmixer;

	switch (dev)   
	{   
	case 1:     
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;  
		break;     
	case 2:     
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;  
		break;     
	case 3:      
		device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;  
		break;     
	case 4:   
		device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		break;   
	default:   
		device = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		break;
	}   

	if (mixerOpen(&hmixer, 0, 0, 0, 0))
	{
		return 0;  
	}
	/*Else do nothing, and continue.*/

	if (!SoundControl::getVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl))
	{
		return 0;
	}
	/*Else do nothing, and continue.*/

	rt = SoundControl::getVolumeValue(hmixer, &volCtrl) * 100 / volCtrl.Bounds.lMaximum;   
	mixerClose(hmixer);   

	return rt;   
}

bool SoundControl::setVolume(long vol, long dev)
{     
	long  device;   
	bool  rc = false;   
	MIXERCONTROL volCtrl;   
	HMIXER hmixer;   

	switch (dev)   
	{   
	case 1:   
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;  
		break;
	case 2:   
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;  
		break;   
	case 3:   
		device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;  
		break;   
	case 4:   
		device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		break;   
	default:     
		device = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;     
		break;
	}     

	if (mixerOpen(&hmixer, 0, 0, 0, 0))  
	{
		return 0;   
	}
	/*Else do nothing, and continue.*/

	if (SoundControl::getVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl))     
	{     
		vol = vol * volCtrl.Bounds.lMaximum / 100;     
		if (SoundControl::setVolumeValue(hmixer, &volCtrl, vol))   
		{
			rc = true;   
		}
		/*Else do nothing, and continue.*/
	}   
	/*Else do nothing, and continue.*/

	mixerClose(hmixer);     

	return rc;     
}        

bool SoundControl::getVolumeControl(HMIXER hmixer, long componentType, long ctrlType, MIXERCONTROL *mxc)   
{   
	MIXERLINECONTROLS mxlc;   
	MIXERLINE mxl;   
	mxl.cbStruct = sizeof (mxl);   
	mxl.dwComponentType = componentType;   
	
	if (!mixerGetLineInfo((HMIXEROBJ)hmixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE))   
	{   
		mxlc.cbStruct = sizeof (mxlc);   
		mxlc.dwLineID = mxl.dwLineID;   
		mxlc.dwControlType = ctrlType;   
		mxlc.cControls = 1;   
		mxlc.cbmxctrl = sizeof (MIXERCONTROL);   
		mxlc.pamxctrl = mxc;   
		if (mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE))   
		{
			return 0;   
		}
		else   
		{
			return 1;   
		}
	}
	/*Else do nothing, and continue.*/

	return 0;   
}

unsigned SoundControl::getVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc)   
{     
	MIXERCONTROLDETAILS mxcd;   
	MIXERCONTROLDETAILS_UNSIGNED vol;   
	vol.dwValue = 0;     
	mxcd.hwndOwner = 0;   
	mxcd.cbStruct = sizeof (mxcd);     
	mxcd.dwControlID = mxc->dwControlID;     
	mxcd.cbDetails = sizeof (vol);     
	mxcd.paDetails = &vol;   
	mxcd.cChannels = 1;   
	if (mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE))
	{
		return -1;     
	}
	/*Else do nothing, and continue.*/

	return vol.dwValue;     
}

bool SoundControl::setVolumeValue(HMIXER hmixer , MIXERCONTROL *mxc, long volume)   
{     
	MIXERCONTROLDETAILS mxcd;     
	MIXERCONTROLDETAILS_UNSIGNED vol;
	vol.dwValue = volume;     
	mxcd.hwndOwner = 0;   
	mxcd.dwControlID = mxc->dwControlID;     
	mxcd.cbStruct = sizeof (mxcd);   
	mxcd.cbDetails = sizeof (vol);   
	mxcd.paDetails = &vol;   
	mxcd.cChannels = 1;   

	if (mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE))   
	{
		return 0;   
	}
	/*Else do nothing, and continue.*/

	return 1;   
}