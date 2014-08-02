#include "stdafx.h"
#include <Windows.h>
#include <mmreg.h>
#include <mmsystem.h>
#include "SoundControl.h"
#pragma comment(lib, "./Engine/Winmm.lib")

SoundControl::SoundControl()
{

}

SoundControl::~SoundControl()
{

}

//得到音量,得到设备的音量dev=0主音量，1WAVE, 2MIDI , 3 LINE IN
unsigned SoundControl::GetVolume(int dev)
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
		//   device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;   //   cd   音量     
		//   device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;   break;   //麦克风音量     
		//   device=MIXERLINE_COMPONENTTYPE_SRC_LINE;   break;   //PC   扬声器音量     
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
	if (!SoundControl::GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl))
	{
		return 0;
	}
	rt = SoundControl::GetVolumeValue(hmixer, &volCtrl) * 100 / volCtrl.Bounds.lMaximum;   
	mixerClose(hmixer);   

	return rt;   
}

//设置设备的音量
bool SoundControl::SetVolume(long dev, long vol)
{   
	//   dev   =0,1,2   分别表示主音量,波形,MIDI   ,LINE   IN   
	//   vol=0-100   表示音量的大小   ,   设置与返回音量的值用的是百分比，即音量从0   -   100，而不是设备的绝对值   
	//   retrun   false   表示设置音量的大小的操作不成功   
	//   retrun   true   表示设置音量的大小的操作成功   

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
	if (SoundControl::GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl))     
	{     
		vol = vol * volCtrl.Bounds.lMaximum / 100;     
		if (SoundControl::SetVolumeValue(hmixer, &volCtrl, vol))   
		{
			rc = true;   
		}
	}     
	mixerClose(hmixer);     

	return rc;     
}     

//设置设备静音   
bool SoundControl::SetMute(long dev, bool vol)
{   
	//   dev   =0,1,2   分别表示主音量,波形,MIDI   ,LINE   IN   
	//   vol=false,true   分别表示取消静音,设置静音   
	//   retrun   false   表示取消或设置静音操作不成功   
	//   retrun   true   表示取消或设置静音操作成功   

	long device;   
	bool rc = false;   
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
	if (SoundControl::GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_MUTE, &volCtrl))
	{
		if (SoundControl::SetMuteValue(hmixer, &volCtrl, vol))     
		{
			rc = true;   
		}
		//vol = vol * volCtrl.Bounds.lMaximum / 100;     
		//if (SoundControl::SetVolumeValue(hmixer, &volCtrl, vol))   
		//{
		//	rc = true;   
		//}
	}
	mixerClose(hmixer);     

	return rc;   
}     

//检查设备是否静音  
bool SoundControl::GetMute(long dev) 
{   
	//dev   =0,1,2   分别表示主音量，波形,MIDI   ,LINE   IN   
	//   retrun   false   表示没有静音   
	//   retrun   true   表示静音   

	long device;
	bool rc = false;   
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
	if (SoundControl::GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_MUTE, &volCtrl))   
	{
		rc = GetMuteValue(hmixer, &volCtrl);   
	}
	mixerClose(hmixer);   

	return rc;   
}

//得到声音控制
bool SoundControl::GetVolumeControl(HMIXER hmixer, long componentType, long ctrlType, MIXERCONTROL *mxc)   
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
	return 0;   
}

//得到静音值
long SoundControl::GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc)   
{   
	MIXERCONTROLDETAILS mxcd;     
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;   
	mxcd.hwndOwner = 0;     
	mxcd.cbStruct = sizeof (mxcd);     
	mxcd.dwControlID = mxc->dwControlID;     
	mxcd.cbDetails = sizeof (mxcdMute);   
	mxcd.paDetails = &mxcdMute;     
	mxcd.cChannels = 1;   
	mxcd.cMultipleItems = 0;     
	if (mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE))     
	{
		return -1;     
	}
	return mxcdMute.fValue;   
}

//得到音量
unsigned SoundControl::GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc)   
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
	return vol.dwValue;     
}

//设置静音值
bool SoundControl::SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute)   
{     
	MIXERCONTROLDETAILS mxcd;     
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	mxcdMute.fValue = mute;     
	mxcd.hwndOwner = 0;   
	mxcd.dwControlID = mxc->dwControlID;     
	mxcd.cbStruct = sizeof (mxcd);   
	mxcd.cbDetails = sizeof (mxcdMute);     
	mxcd.paDetails = &mxcdMute;     
	mxcd.cChannels = 1;     
	mxcd.cMultipleItems = 0;   
	if (mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE))
	{
		return 0;   
	}
	return 1;     
}

//设置音量
bool SoundControl::SetVolumeValue(HMIXER hmixer , MIXERCONTROL *mxc, long volume)   
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
	return 1;   
}

//void ShowVolume(void);   //Prototype the function early in the app
//
//void ShowVolume(void)
//{
//	// This is the function that can be added to the Generic Sample to
//	// illustrate the use of waveOutGetVolume() and waveOutSetVolume().
//
//	char buffer[40];
//	char printbuf[80];
//	UINT uRetVal, uNumDevs;
//	DWORD volume;
//	long lLeftVol, lRightVol;
//
//	WAVEOUTCAPS waveCaps;
//
//	// Make sure there is at least one
//	// wave output device to work with.
//	if (uNumDevs = waveOutGetNumDevs())
//	{
//		itoa((int)uNumDevs, buffer, 10);
//		//wsprintf(printbuf, "Number of devices is %s\n", (LPSTR)buffer);
//		//MessageBox(GetFocus(), printbuf, "NumDevs", MB_OK);
//	}
//
//	// This sample uses a hard-coded 0 as the device ID, but retail
//	// applications should loop on devices 0 through N-1, where N is the
//	// number of devices returned by waveOutGetNumDevs().
//	if (!waveOutGetDevCaps(0,(LPWAVEOUTCAPS)&waveCaps,
//		sizeof(WAVEOUTCAPS)))
//
//	{
//		// Verify the device supports volume changes
//		if(waveCaps.dwSupport & WAVECAPS_VOLUME)
//		{
//			// The low word is the left volume, the high word is the right.
//			// Set left channel: 2000h is one-eighth volume (8192 base ten).
//			// Set right channel: 4000h is quarter volume (16384 base ten).
//			uRetVal = waveOutSetVolume(0, (DWORD)0x40002000UL);
//
//			// Now get and display the volumes.
//			uRetVal = waveOutGetVolume(0, (LPDWORD)&volume);
//
//			lLeftVol = (long)LOWORD(volume);
//			lRightVol = (long)HIWORD(volume);
//
//			ltoa(lLeftVol, buffer, 10);
//			//wsprintf(printbuf, "Left Volume is %s\n", (LPSTR)buffer);
//			//MessageBox(GetFocus(), printbuf, "Left Volume", MB_OK);
//
//			ltoa(lRightVol, buffer, 10);
//			//wsprintf(printbuf, "Right Volume is %s\n", (LPSTR)buffer);
//			//MessageBox(GetFocus(), printbuf, "Right Volume", MB_OK);
//
//			// The low word is the left volume, the high word is the right.
//			// Set left channel: 8000h is half volume (32768 base ten).
//			// Set right channel: 4000h is quarter volume (16384 base ten).
//			uRetVal = waveOutSetVolume(0, (DWORD)0x40008000UL);
//
//			// Now get and display the volumes.
//			uRetVal = waveOutGetVolume(0, (LPDWORD)&volume);
//
//			lLeftVol = (long)LOWORD(volume);
//			lRightVol = (long)HIWORD(volume);
//
//			ltoa(lLeftVol, buffer, 10);
//			//wsprintf(printbuf, "Left Volume is %s\n", (LPSTR)buffer);
//			//MessageBox(GetFocus(), printbuf, "Left Volume", MB_OK);
//
//			ltoa(lRightVol, buffer, 10);
//			//wsprintf(printbuf, "Right Volume is %s\n", (LPSTR)buffer);
//			//MessageBox(GetFocus(), printbuf, "Right Volume", MB_OK);
//
//		}
//	}
//}