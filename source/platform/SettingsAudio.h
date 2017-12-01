#ifndef __SETTINGSAUDIO_H__
#define __SETTINGSAUDIO_H__


#include <windows.h>
//#include "IniDefine.h"
#include "GSysServices.h"

enum
{
	SETTING_AUDIO_EQ_NONE = 0,          //CFG_EQ_OFF
	SETTING_AUDIO_EQ_ROCK,              //CFG_EQ_ROCK
	SETTING_AUDIO_EQ_POP,               //CFG_EQ_POP
	SETTING_AUDIO_EQ_LIVE,              //CFG_EQ_LIVING
	SETTING_AUDIO_EQ_DANCE,             //CFG_EQ_DANCE
	SETTING_AUDIO_EQ_CLASSIC,           //CFG_EQ_CLASSICAL
	SETTING_AUDIO_EQ_SOFT,              //CFG_EQ_SOFT
	SETTING_AUDIO_EQ_CUSTOM,	// add by tiger
};

enum
{
	SETTING_AUDIO_REVERB_NONE,
	SETTING_AUDIO_REVERB_LIVING,
	SETTING_AUDIO_REVERB_HALL,
	SETTING_AUDIO_REVERB_CONCERT,
	SETTING_AUDIO_REVERB_CAVE,
	SETTING_AUDIO_REVERB_BATHROOM,
	SETTING_AUDIO_REVERB_ARENA,
};



enum
{
    SETTINGS_AUDIO_BASS,
    SETTINGS_AUDIO_TREBLE,
    SETTINGS_AUDIO_BALANCE,
	SETTINGS_AUDIO_LOUDNESS,
    SETTINGS_AUDIO_KEYBEEP,
    SETTINGS_AUDIO_END,
};


enum
{
	SETTING_AUDIO_KEYBEEP_OFF = 0,
	SETTING_AUDIO_KEYBEEP_ON,
};

#define TRIM_LEVEL_MAX		41
#define MAX_EQ_LEVEL			29
#define MAX_EQ_LOUDNESS_LEVEL	20

#define  MAX_EQ_BALANCE_LEVEL				40

extern const GUINT32	_au4TrimValue[TRIM_LEVEL_MAX];
extern GUINT32			g_ganValues[MAX_EQ_LEVEL];
extern MISC_EQ_GAIN_T	gEQTypeGain[];


#define FULL_VOLUME_GAIN	0x20000
#define MAX_VOLUME_INDEX	100




#pragma once

class SettingsAudio
{
public:
	SettingsAudio();
	~SettingsAudio();

	// add by tiger ++
	static SettingsAudio* GetInstance();
	// ��ȡָ��EQ��10��Ƶ���ֵ
	// pValues : Ϊ��������,�������10��Ԫ��,����ֵ�洢�ڸ�������
	BOOL GetEQValues(UINT eqType, UINT32* pValues);

	// ��������(0~28),����g_ganValues����ȷ�е�EQֵ
	UINT GetEQGainbyIndex(int index);

	// ��������ֵ������EQ, 10��Ƶ�������
	void SetCustomEQbyIndex(UINT* pvalue);
	// add by tiger --
public:
    BOOL    getValue(BYTE index, UINT32* getValue);
    BOOL    setValue(BYTE index, UINT32* setValue);  
	BYTE	getSettingsNum(void);


    BOOL    setVolume(UINT32* setValue);
    BOOL    setBass(UINT32* setValue);
    BOOL    setTreble(UINT32* setValue);
    BOOL    setBalance(UINT32* setValue);

	BOOL    setLoudness(UINT32* setValue);

    BOOL    setKeyBeep(UINT32* setValue);
    BOOL    setAudAdv(UINT32* setValue);


	BOOL    setEQ(UINT32* setValue);
	BOOL	setFrontVolume(UINT32* pSetValue);
	BOOL	setRearVolume(UINT32* pSetValue);

	BOOL	SetCustomEQValue(MISC_EQ_GAIN_T eqGain );
	BOOL	newSetEQ_smooth( UINT32* pSetValue );


    //BOOL    getVolume(UINT32* getValue);
    BOOL    getBass(UINT32* getValue);
    BOOL    getTreble(UINT32* getValue);
    BOOL    getBalance(UINT32* getValue);

	BOOL    getLoudness(UINT32* setValue);

    BOOL    getKeyBeep(UINT32* getValue);
	BOOL    getEQ(UINT32* getValue);
	//BOOL	getFrontVolume(UINT32* getValue);
	BOOL	getRearVolume(UINT32* getValue);

	//   calculator banlance value
	void    CalculatorBalance( POINT ptPos,MISC_DVP_BALANCES_T& bv );
	BOOL	SetBalance( MISC_DVP_BALANCES_T &rbv );

public:
// rear speaker no volumn 
	void	InitRearSpeaker(  );
	void	setDownMix();
	void	setUpmix();

	//ѡȡ��Ч������11�������ĵڶ����͵�10���ֱ��������͸������ж�
	void    GetEqTypeBassTreble( UINT32 nEQType,UINT32* bassValue, UINT32* trebleValue );


	static void	DebugMTK_AudioInter();

	BOOL		EnableSubwoofer( BOOL bEnable );
	BOOL		SetSubwoofer(UINT32 setValue);
	BOOL		GetSubwoofer(UINT32* getValue);


};	

#endif