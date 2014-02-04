#ifndef SOUNDCONTROL_H   
#define SOUNDCONTROL_H   

//音量控制类
class SoundControl
{
public:
	SoundControl();
	~SoundControl();

public:
	//下面四个是输出函数，可以自己调用   
	static bool SetVolume(long dev, long vol);	//设置设备的音量   
	static unsigned GetVolume(int dev);			//得到设备的音量dev=0主音量，1WAVE   ,2MIDI   ,3   LINE   IN   
	static bool SetMute(long dev, bool vol);	//设置设备静音   
	static bool GetMute(long dev);  

private:
	static bool GetVolumeControl(HMIXER hmixer , long componentType, long ctrlType, MIXERCONTROL *mxc);   
	static long GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc);   
	static unsigned GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc);   
	static bool SetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume);   
	static bool SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute);  
};
 
#endif