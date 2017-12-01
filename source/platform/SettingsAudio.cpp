#include "stdafx.h"
#include <stdlib.h>
#include "SettingsAudio.h"
#include "DVP_Agent.h"
#include "config.h"
#include "SourceManager.h"

#if MISC_CLIENT_SET_GAIN

// #define LOUDNESS_MIN   0
// #define MAX_EQ_LOUDNESS_LEVEL   20

//MISC_LOUDNESS_GAIN_T  gLoudNessGain[20] =
//{
//	{0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
//	{0x00100000,0x00e0b3d9,0x000f4e20,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0be81,0x000f43b6,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0c9cb,0x000f38b1,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0d5c0,0x000f2d0a,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0e26a,0x000f20b6,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0efd5,0x000f13ae,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e0fe0b,0x000f05e5,0x001f5636,0x00f0a807,0x005a9c78},
//
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
//	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78}
//};


MISC_LOUDNESS_GAIN_T  gLoudNessGain[20] =
{
	{0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
	{0x00100000,0x00e0b3d9,0x000f4e20,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0be81,0x000f43b6,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0c9cb,0x000f38b1,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0d5c0,0x000f2d0a,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0e26a,0x000f20b6,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0efd5,0x000f13ae,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e0fe0b,0x000f05e5,0x001f5636,0x00f0a807,0x005a9c78},


	{0x00100000,0x00e10d19,0x000ef753,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e11d0a,0x000ee7eb,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e12dee,0x000ed7a2,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e13fd2,0x000ec66c,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e152c6,0x000eb43b,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e166d8,0x000ea103,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e17c1c,0x000e8cb6,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e192a2,0x000e7743,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e1aa7d,0x000e609d,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e1c3c3,0x000e48b2,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e1de87,0x000e2f72,0x001f5636,0x00f0a807,0x005a9c78},
	{0x00100000,0x00e1fae2,0x000e14cc,0x001f5636,0x00f0a807,0x005a9c78}

};



GUINT32 g_ganValues[MAX_EQ_LEVEL]={
	0xFFFF3315,0xFFFF3950,0xFFFF404E,0xFFFF4827,0xFFFF50F5,0xFFFF5AD6,0xFFFF65EB,
	0xFFFF725A,0xFFFF804E,0xFFFF8FF6,0xFFFFA187,0xFFFFB53C,0xFFFFCB5A,0xFFFFE42A,
	0x00000000,0x00001F3C,0x00004248,0x0000699C,0x000095BB,0x0000C73D,0x0000FEC9,
	0x00013D1C,0x0001830A,0x0001D181,0x0002298B,0x00028C52,0x0002FB27,0x00037782,
	0x0004030A
};

GUINT32 g_dryValues[MAX_EQ_LEVEL]={
	0x00003314,0x0000394F,0x0000404D,0x00004826,0x000050F4,0x00005AD5,0x000065EA,
	0x00007259,0x0000804D,0x00008FF5,0x0000A186,0x0000B53B,0x0000CB59,0x0000E429,
	0x00010000,0x00011F3C,0x00014248,0x0001699C,0x000195BB,0x0001C73D,0x0001FEC9,
	0x00023D1C,0x0002830A,0x0002D181,0x0003298B,0x00038C52,0x0003FB27,0x00047782,
	0x0005030A
};



//const GUINT32 _au4TrimValue[TRIM_LEVEL_MAX] =
//{
//	0x0000A1E9,0x0000AB81,0x0000B5AA,0x0000C06E,0x0000CBD5,0x0000D7E9,0x0000E4B4,0x0000F241,
//	0x0001009C,0x00010FD0,0x00011FEB,0x000130FB,0x0001430D,0x00015631,0x00016A78,0x00017FF2,
//	0x000196B2,0x0001AECB,0x0001C852,0x0001E35C,0x00020000,0x00021E57,0x00023E79,0x00026083,
//	0x00028492,0x0002AAC3,0x0002D338,0x0002FE13,0x00032B77,0x00035B8C,0x00038E7B,0x0003C46E,
//	0x0003FD93,0x00043A1B,0x00047A3A,0x0004BE25,0x00050616,0x0005524B,0x0005A303,0x0005F884,
//	0x00065316
//};

// update for balance zero to no sound 

const GUINT32 _au4TrimValue[TRIM_LEVEL_MAX] =
{
	0x00000000,0x0000AB81,0x0000B5AA,0x0000C06E,0x0000CBD5,0x0000D7E9,0x0000E4B4,0x0000F241,
	0x0001009C,0x00010FD0,0x00011FEB,0x000130FB,0x0001430D,0x00015631,0x00016A78,0x00017FF2,
	0x000196B2,0x0001AECB,0x0001C852,0x0001E35C,0x00020000,0x00021E57,0x00023E79,0x00026083,
	0x00028492,0x0002AAC3,0x0002D338,0x0002FE13,0x00032B77,0x00035B8C,0x00038E7B,0x0003C46E,
	0x0003FD93,0x00043A1B,0x00047A3A,0x0004BE25,0x00050616,0x0005524B,0x0005A303,0x0005F884,
	0x00065316
};

const GUINT32 _au4TrimValue_sub[TRIM_LEVEL_MAX] =
{
	0x00000000,0x0000AB81,0x0000B5AA,0x0000C06E,0x0000CBD5,0x0000D7E9,0x0000E4B4,0x0000F241,
	0x0001009C,0x00010FD0,0x00011FEB,0x000130FB,0x0001430D,0x00015631,0x00016A78,0x00017FF2,
	0x000196B2,0x0001AECB,0x0001C852,0x0001E35C,0x00020000,0x00021E57,0x00023E79,0x00026083,
	0x00028492,0x0002AAC3,0x0002D338,0x0002FE13,0x00032B77,0x00035B8C,0x00038E7B,0x0003C46E,
	0x0003FD93,0x00043A1B,0x00047A3A,0x0004BE25,0x00050616,0x0005524B,0x0005A303,0x0005F884,
	/*0x00065316*/0x000A5316
};


MISC_EQ_GAIN_T gEQTypeGain[] = {
	//CFG_EQ_OFF
	{0x00010000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
	//CFG_EQ_ROCK
	{0x00010000,0x0002298B,0x0001830A,0x0000C73D,0x00000000,0xFFFFCB5A,0x00000000,0x00004248,0x000095BB,0x0001830A,0x0002298B},
	//CFG_EQ_POP
	{0x00010000,0xFFFFCB5A,0x00000000,0x000095BB,0x0001830A,0x0002298B,0x0001830A,0x0000C73D,0x00001F3C,0xFFFFCB5A,0xFFFFA187},
	//CFG_EQ_LIVING
	{0x00010000,0x0004030A,0x0002FB27,0x0001830A,0x00004248,0xFFFFCB5A,0x00000000,0x0000FEC9,0x0002298B,0x0001830A,0x000095BB},
	//CFG_EQ_DANCE
	{0x00010000,0x0002298B,0x0001D181,0x00013D1C,0x000095BB,0x00000000,0xFFFFB53C,0xFFFF8FF6,0xFFFFB53C,0xFFFFE42A,0x00000000},
	//CFG_EQ_CLASSICAL
	{0x00010000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFE42A,0xFFFFCB5A,0xFFFFA187,0xFFFF8FF6},
	//CFG_EQ_SOFT
	{0x00010000,0x0000C73D,0x000095BB,0x0000699C,0x00004248,0x00001F3C,0xFFFFE42A,0xFFFFB53C,0xFFFF8FF6,0xFFFF725A,0xFFFF65EB}
};



#endif 
			

SettingsAudio::SettingsAudio()
{
}

SettingsAudio::~SettingsAudio(VOID)
{
}

BYTE SettingsAudio::getSettingsNum(void)
{
	return SETTINGS_AUDIO_END;
}

BOOL SettingsAudio::setValue(BYTE index, UINT32* setValue)
{
	BOOL bReturnValue;
    switch(index)
    {
        case SETTINGS_AUDIO_BASS:
            bReturnValue = setBass(setValue);
            break;
        case SETTINGS_AUDIO_TREBLE:
            bReturnValue = setTreble(setValue);
            break;
		case SETTINGS_AUDIO_BALANCE:
			bReturnValue = setBalance(setValue);
			break;
        case SETTINGS_AUDIO_LOUDNESS:
            setLoudness(setValue);
			break;
		case SETTINGS_AUDIO_KEYBEEP:
			bReturnValue = setKeyBeep(setValue);
			break;
		default:
			bReturnValue = FALSE;
    }

	return bReturnValue;
}

BOOL SettingsAudio::getValue(BYTE index, UINT32* getValue)
{
	BOOL bReturnValue;
    switch(index)
    {
        case SETTINGS_AUDIO_BASS:
            bReturnValue = getBass(getValue);
            break;
        case SETTINGS_AUDIO_TREBLE:
            bReturnValue = getTreble(getValue);
            break;
        case SETTINGS_AUDIO_BALANCE:
            bReturnValue = getBalance(getValue);
			break;
		case SETTINGS_AUDIO_LOUDNESS:
			bReturnValue = getLoudness(getValue);
            break;
        case SETTINGS_AUDIO_KEYBEEP:
            bReturnValue = getKeyBeep(getValue);
            break;
        default:
            bReturnValue = FALSE;
    }

	return bReturnValue;
}

BOOL SettingsAudio::setBass(UINT32* pSetValue)
{
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue;
	//AUD_EQVALUES_T aEqValue;

	UINT32 uTreble;
	AUD_EQVALUES_T    rEQValues;

	INT32 uBassValue = * pSetValue;
	

#if MISC_CLIENT_SET_GAIN
	MISC_EQ_GAIN_T eqGain;
	getTreble(&uTreble);

	eqGain.u4Gain0 = g_dryValues[14];
	eqGain.u4Gain1 = g_ganValues[uBassValue+14];
	eqGain.u4Gain2 = g_ganValues[uBassValue+14];
	eqGain.u4Gain3 = g_ganValues[uBassValue+14];
	eqGain.u4Gain4 = 0;
	eqGain.u4Gain5 = 0;
	eqGain.u4Gain6 = 0;
	eqGain.u4Gain7 = 0;
	eqGain.u4Gain8 = g_ganValues[uTreble+14];
	eqGain.u4Gain9 = g_ganValues[uTreble+14]; 
	eqGain.u4Gain10 = g_ganValues[uTreble+14];

	if ((uBassValue==0) && (uTreble==0))
	{
		u4ReturnValue = GClientSetEQValues(hAudioDev,eqGain,FALSE);
	}
	else
	{
		u4ReturnValue = GClientSetEQValues(hAudioDev,eqGain,TRUE);

	}
#else
	getTreble(&uTreble);

	//aEqValue.u4Band0 = (INT32) *pSetValue;
	aEqValue.i4Band0 = 0;
	aEqValue.i4Band1 = (INT32) *pSetValue;
	aEqValue.i4Band2 = (INT32) *pSetValue;
	aEqValue.i4Band3 = (INT32) *pSetValue;

	aEqValue.i4Band4 = 0;
	aEqValue.i4Band5 = 0;
	aEqValue.i4Band6 = 0;
	aEqValue.i4Band7 = 0;

	aEqValue.i4Band8 = uTreble;
	aEqValue.i4Band9 = uTreble;
	aEqValue.i4Band10 = uTreble;

	u4ReturnValue = GSetEQValues(hAudioDev,aEqValue);


#endif 


    //UINT32 u4RegValue = (AUDIO_BASS_MAX-AUDIO_BASS_MIN) / (AUDIO_BASS_UI_MAX - AUDIO_BASS_UI_MIN) * (*pSetValue - AUDIO_BASS_UI_MIN) + AUDIO_BASS_MIN;
	/*
    u4ReturnValue = GSetEQValue(hAudioDev, 0, (INT32) *pSetValue);
    u4ReturnValue = GSetEQValue(hAudioDev, 1, (INT32) *pSetValue);
	u4ReturnValue = GSetEQValue(hAudioDev, 2, (INT32) *pSetValue);
	u4ReturnValue = GSetEQValue(hAudioDev, 3, (INT32) *pSetValue);
	*/
	/*
	u4ReturnValue = GSetEQValue(hAudioDev, 4, 0);
	u4ReturnValue = GSetEQValue(hAudioDev, 5, 0);
    u4ReturnValue = GSetEQValue(hAudioDev, 6, 0);
	u4ReturnValue = GSetEQValue(hAudioDev, 7, 0);
	u4ReturnValue = GSetEQValue(hAudioDev, 8, 0);
	u4ReturnValue = GSetEQValue(hAudioDev, 9, 0);
    u4ReturnValue = GSetEQValue(hAudioDev, 10,0);
	*/
    GCloseAudioDev(hAudioDev);

#if 0
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
	{
		u4ReturnValue = DVP_GSetEQValue(TRUE,0,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,1,(INT32) *pSetValue);
		u4ReturnValue = DVP_GSetEQValue(TRUE,2,(INT32) *pSetValue);
		u4ReturnValue = DVP_GSetEQValue(TRUE,3,(INT32) *pSetValue);
		u4ReturnValue = DVP_GSetEQValue(TRUE,4,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,5,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,6,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,7,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,8,uTreble);
		u4ReturnValue = DVP_GSetEQValue(TRUE,9,uTreble);
		u4ReturnValue = DVP_GSetEQValue(TRUE,10,uTreble);
	}
#else

	rEQValues.i4Band0 = 0;
	rEQValues.i4Band1 = (INT32) *pSetValue;
	rEQValues.i4Band2 = (INT32) *pSetValue;
	rEQValues.i4Band3 = (INT32) *pSetValue;
	rEQValues.i4Band4 = 0;
	rEQValues.i4Band5 = 0;
	rEQValues.i4Band6 = 0;
	rEQValues.i4Band7 = 0;
	rEQValues.i4Band8 = uTreble;
	rEQValues.i4Band9 = uTreble;
	rEQValues.i4Band10 = uTreble;

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetEQValues(TRUE, rEQValues);

#endif 


    return u4ReturnValue;
}

BOOL SettingsAudio::setTreble(UINT32* pSetValue)
{
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue;

	INT32 uTrebleValue = * pSetValue;
	AUD_EQVALUES_T rEQValues;
	UINT32 uBass;


#if MISC_CLIENT_SET_GAIN
	MISC_EQ_GAIN_T eqGain;
	getBass(&uBass);

	eqGain.u4Gain0 = g_dryValues[14];
	eqGain.u4Gain1 = g_ganValues[uBass+14];
	eqGain.u4Gain2 = g_ganValues[uBass+14];
	eqGain.u4Gain3 = g_ganValues[uBass+14];
	eqGain.u4Gain4 = 0;
	eqGain.u4Gain5 = 0;
	eqGain.u4Gain6 = 0;
	eqGain.u4Gain7 = 0;
	eqGain.u4Gain8 = g_ganValues[uTrebleValue+14];
	eqGain.u4Gain9 = g_ganValues[uTrebleValue+14]; 
	eqGain.u4Gain10 = g_ganValues[uTrebleValue+14];

	if ((uBass==0) && (uTrebleValue==0))
	{
		u4ReturnValue = GClientSetEQValues(hAudioDev,eqGain,FALSE);

	}
	else
	{
		u4ReturnValue = GClientSetEQValues(hAudioDev,eqGain,TRUE);
	}
#else
	getBass(&uBass);

	//aEqValue.u4Band0 = (INT32) *pSetValue;
	aEqValue.i4Band0 = 0;

	aEqValue.i4Band1 = uBass;
	aEqValue.i4Band2 = uBass;
	aEqValue.i4Band3 = uBass;

	aEqValue.i4Band4 = 0;
	aEqValue.i4Band5 = 0;
	aEqValue.i4Band6 = 0;
	aEqValue.i4Band7 = 0;

	aEqValue.i4Band8 = (INT32) *pSetValue;
	aEqValue.i4Band9 = (INT32) *pSetValue;
	aEqValue.i4Band10 = (INT32) *pSetValue;

	u4ReturnValue = GSetEQValues(hAudioDev,aEqValue);

#endif 

    //UINT32 u4RegValue = (AUDIO_TREBLE_MAX-AUDIO_TREBLE_MIN) / (AUDIO_TREBLE_UI_MAX - AUDIO_TREBLE_UI_MIN) * (*pSetValue - AUDIO_TREBLE_UI_MIN) + AUDIO_TREBLE_MIN;
	//u4ReturnValue = GSetEQValue(hAudioDev, 0, (INT32) *pSetValue);
	/*
	u4ReturnValue = GSetEQValue(hAudioDev, 1,0);
	u4ReturnValue = GSetEQValue(hAudioDev, 2,0);
    u4ReturnValue = GSetEQValue(hAudioDev, 3,0);
	u4ReturnValue = GSetEQValue(hAudioDev, 4,0);
	u4ReturnValue = GSetEQValue(hAudioDev, 5,0);
    u4ReturnValue = GSetEQValue(hAudioDev, 6,0);
	u4ReturnValue = GSetEQValue(hAudioDev, 7,0);
	*/
	/*
	u4ReturnValue = GSetEQValue(hAudioDev, 8, (INT32) *pSetValue);
	u4ReturnValue = GSetEQValue(hAudioDev, 9, (INT32) *pSetValue);
    u4ReturnValue = GSetEQValue(hAudioDev, 10, (INT32) *pSetValue);
	*/

    GCloseAudioDev(hAudioDev);

#if 0
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
	{
		u4ReturnValue = DVP_GSetEQValue(TRUE,0,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,1,uBass);
		u4ReturnValue = DVP_GSetEQValue(TRUE,2,uBass);
		u4ReturnValue = DVP_GSetEQValue(TRUE,3,uBass);
		u4ReturnValue = DVP_GSetEQValue(TRUE,4,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,5,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,6,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,7,0);
		u4ReturnValue = DVP_GSetEQValue(TRUE,8,(INT32) *pSetValue);
		u4ReturnValue = DVP_GSetEQValue(TRUE,9,(INT32) *pSetValue);
		u4ReturnValue = DVP_GSetEQValue(TRUE,10,(INT32) *pSetValue);
	}
#else
	rEQValues.i4Band0 = 0;
	rEQValues.i4Band1 = uBass;
	rEQValues.i4Band2 = uBass;
	rEQValues.i4Band3 = uBass;
	rEQValues.i4Band4 = 0;
	rEQValues.i4Band5 = 0;
	rEQValues.i4Band6 = 0;
	rEQValues.i4Band7 = 0;
	rEQValues.i4Band8 = (INT32) *pSetValue;
	rEQValues.i4Band9 = (INT32) *pSetValue;
	rEQValues.i4Band10 = (INT32) *pSetValue;

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetEQValues(TRUE, rEQValues);

#endif

    return u4ReturnValue;
}

BOOL  SettingsAudio::setLoudness(UINT32* setValue)
{
	UINT32 u4ReturnValue = 0;
	GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4LoudnessValue = *setValue;

#if MISC_CLIENT_SET_GAIN

	if ((0<=u4LoudnessValue) && (u4LoudnessValue<MAX_EQ_LOUDNESS_LEVEL))
	{
		GClientSetLoudNess(hAudioDev, u4LoudnessValue,gLoudNessGain[u4LoudnessValue]);
	}
#else

    u4ReturnValue = GSetLoudNess(hAudioDev, u4LoudnessValue);

#endif 
	
    GCloseAudioDev(hAudioDev);

    return u4ReturnValue;

}
BOOL SettingsAudio::setBalance(UINT32* pSetValue)
{
	MISC_DVP_BALANCES_T     rBalanceValues;
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue;
    INT32 i4RightValue = (INT32) *pSetValue+20;
    INT32 i4LeftValue = 40 - i4RightValue;
    //UINT32 u4RegValue = (AUDIO_BALANCE_MAX-AUDIO_BALANCE_MIN) / (AUDIO_BALANCE_UI_MAX - AUDIO_BALANCE_UI_MIN) * (*pSetValue - AUDIO_BALANCE_UI_MIN) + AUDIO_BALANCE_MIN;

#if MISC_CLIENT_SET_GAIN
	u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[i4LeftValue], BALANCE_FRONT_LEFT);
    u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[i4LeftValue], BALANCE_REAR_LEFT);
    u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[i4RightValue], BALANCE_FRONT_RIGHT);
    u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[i4RightValue], BALANCE_REAR_RIGHT);

#else

    u4ReturnValue = GSetBalance(hAudioDev, i4LeftValue, BALANCE_FRONT_LEFT);
    u4ReturnValue = GSetBalance(hAudioDev, i4LeftValue, BALANCE_REAR_LEFT);
    u4ReturnValue = GSetBalance(hAudioDev, i4RightValue, BALANCE_FRONT_RIGHT);
    u4ReturnValue = GSetBalance(hAudioDev, i4RightValue, BALANCE_REAR_RIGHT);
#endif 
    GCloseAudioDev(hAudioDev);

	i4RightValue = *pSetValue;


	//i4LeftValue  = -*pSetValue;
	//此处警告可忽略处理
	i4LeftValue = UINT32( -((int)*pSetValue) );



#if 0
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
	{
		u4ReturnValue = DVP_GSetBalance(TRUE,i4LeftValue,BALANCE_FRONT_LEFT);
		u4ReturnValue = DVP_GSetBalance(TRUE,i4LeftValue,BALANCE_REAR_LEFT);
		u4ReturnValue = DVP_GSetBalance(TRUE,i4RightValue,BALANCE_FRONT_RIGHT);
		u4ReturnValue = DVP_GSetBalance(TRUE,i4RightValue,BALANCE_REAR_RIGHT);
	}
#else
	rBalanceValues.i4FLeftValue = i4LeftValue;
	rBalanceValues.i4FRightValue = i4RightValue;
	rBalanceValues.i4RLeftValue = i4LeftValue;
	rBalanceValues.i4RRightValue = i4RightValue;

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )
		u4ReturnValue  = DVP_GSetBalances(TRUE,rBalanceValues);
#endif 

    return u4ReturnValue;
}

typedef void (WINAPI *DLL_AUDIOUPDATEFROMREGISTRY)();// 定义一个新类型指针，指向WINAPI

BOOL SettingsAudio::setKeyBeep(UINT32* setValue)
{
	HKEY    hKey;
	HRESULT lRt = 0;
	UINT32  curValue; 

	if((int)*setValue > SETTING_AUDIO_KEYBEEP_ON)
		*setValue = SETTING_AUDIO_KEYBEEP_OFF;
	else if((int) *setValue < SETTING_AUDIO_KEYBEEP_OFF)
		*setValue = SETTING_AUDIO_KEYBEEP_ON;

	if ((int)*setValue == SETTING_AUDIO_KEYBEEP_OFF)
	{
		curValue = 0;
	}
	else if ((int)*setValue == SETTING_AUDIO_KEYBEEP_ON)
	{
		curValue = 65538;

	}

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("ControlPanel\\Volume"), 0, 0, &hKey)) 
	{
		lRt=RegSetValueEx(hKey, TEXT("Screen"), 0, REG_DWORD, (GBYTE*)&curValue, sizeof(GDWORD));
		RegCloseKey(hKey);
	}
 
	DLL_AUDIOUPDATEFROMREGISTRY Dll_AudioUpdateFromRegistry = NULL;
	HINSTANCE hCoreDll = LoadLibrary(TEXT("coredll.dll"));
	if (hCoreDll)
	{
		Dll_AudioUpdateFromRegistry = (DLL_AUDIOUPDATEFROMREGISTRY)GetProcAddress(hCoreDll, _T("AudioUpdateFromRegistry"));
		if (Dll_AudioUpdateFromRegistry)
		{
			(Dll_AudioUpdateFromRegistry)();
		}
		else
		{
			return FALSE;
		}
		FreeLibrary(hCoreDll);
	}
	else
	{
		return FALSE;
	}
  
    return TRUE;
}


BOOL SettingsAudio::getBass(UINT32* getValue)
{
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue;
	MISC_EQ_GAIN_T mEQgain; 

	int  i = 0;
	int  iIndex = 0;


    //u4ReturnValue = GGetBass(hAudioDev, &u4RegValue);
    //*getValue = (u4RegValue - AUDIO_BASS_MIN) / ((AUDIO_BASS_MAX-AUDIO_BASS_MIN) / (AUDIO_BASS_UI_MAX - AUDIO_BASS_UI_MIN)) + AUDIO_BASS_UI_MIN;
#if MISC_CLIENT_SET_GAIN
    //u4ReturnValue = GGetEQValue(hAudioDev, 0, (INT32*) getValue);
	//u4ReturnValue = GGetEQValue(hAudioDev, 1, (INT32*) getValue);
	u4ReturnValue = GClientGetEQValues(hAudioDev,&mEQgain);

	//*getValue = mEQgain.u4Gain1;
	for(i=0;i < MAX_EQ_LEVEL;i++)
	{
		if (mEQgain.u4Gain1 == g_ganValues[i])
		{
			iIndex = i;
		}
	}

	if ((iIndex < 0) ||
		(iIndex > MAX_EQ_LEVEL))
	{
		*getValue = 0;
	}
	else
	{
		*getValue = iIndex - 14; 
	}
#else
	u4ReturnValue = GGetEQValue(hAudioDev, 1, (INT32*) getValue);

#endif 

    GCloseAudioDev(hAudioDev);
    return u4ReturnValue;
}
BOOL SettingsAudio::getTreble(UINT32* getValue)
{
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue;

	int i = 0;
	int iIndex = 0;

	MISC_EQ_GAIN_T mEQgain; 

    //u4ReturnValue = GGetTreble(hAudioDev, &u4RegValue);
    //*getValue = (u4RegValue - AUDIO_TREBLE_MIN) / ((AUDIO_TREBLE_MAX-AUDIO_TREBLE_MIN) / (AUDIO_TREBLE_UI_MAX - AUDIO_TREBLE_UI_MIN)) + AUDIO_TREBLE_UI_MIN;
#if MISC_CLIENT_SET_GAIN
	u4ReturnValue = GClientGetEQValues(hAudioDev,&mEQgain);

	for(i=0;i < MAX_EQ_LEVEL;i++)
	{
		if (mEQgain.u4Gain10 == g_ganValues[i])
		{
			iIndex = i;
		}
	}

	if ((iIndex < 0) ||
		(iIndex > MAX_EQ_LEVEL))
	{
		*getValue = 0;
	}
	else
	{
		*getValue = iIndex - 14; 
	}

#else

    u4ReturnValue = GGetEQValue(hAudioDev, 10, (INT32*) getValue);
#endif 

    GCloseAudioDev(hAudioDev);
    return u4ReturnValue;
}

BOOL  SettingsAudio::getLoudness(UINT32* setValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue=0;

	MISC_LOUDNESS_GAIN_T uLoudNessGain;

#if MISC_CLIENT_SET_GAIN

	u4ReturnValue = GClientGetLoudNess(hAudioDev, (GUINT8 *)setValue,&uLoudNessGain);

#else

    u4ReturnValue = GGetLoudNess(hAudioDev, (GUINT8 *)setValue);

#endif 
    GCloseAudioDev(hAudioDev);

    return u4ReturnValue;
}

BOOL SettingsAudio::getBalance(UINT32* getValue)
{
    GHANDLE hAudioDev = GOpenAudioDev();
    UINT32 u4ReturnValue, u4RegValue=0;
    INT32 i4ReadValue = 0;

	GUINT32 i4LeftValue;
	int i;
	//TCHAR szGetString[4];
	//getStorageValue(INI_TYPE_USER, INI_KEY_AUDIO_BALANCE_NAME, szGetString, INI_KEY_AUDIO_BALANCE_DEFAULT);
	//*getValue = _wtoi(szGetString);

    //u4ReturnValue = GGetBalance(hAudioDev, &u4RegValue);
    //*getValue = (u4RegValue - AUDIO_TREBLE_MIN) / ((AUDIO_TREBLE_MAX-AUDIO_TREBLE_MIN) / (AUDIO_TREBLE_UI_MAX - AUDIO_TREBLE_UI_MIN)) + AUDIO_TREBLE_UI_MIN;

#if MISC_CLIENT_SET_GAIN
	u4ReturnValue = GClientGetBalance(hAudioDev, &i4LeftValue, BALANCE_FRONT_LEFT);

	for (i=0;i<TRIM_LEVEL_MAX;i++)
	{
		if (_au4TrimValue[i] == i4LeftValue)
		{
			i4ReadValue = i;
			break;
		}

	}
#else
    u4ReturnValue = GGetBalance(hAudioDev, &i4ReadValue, BALANCE_FRONT_LEFT);
#endif 

    if(u4ReturnValue)
    {
        *getValue = (UINT32) (20-i4ReadValue);
    }
    GCloseAudioDev(hAudioDev);
    return u4ReturnValue;
}

BOOL SettingsAudio::getKeyBeep(UINT32* getValue)
{
	HKEY    hKey;
	HRESULT lRt = 0;
	GDWORD   u4Type=REG_DWORD;
	GDWORD   u4Count=4;
	UINT32  curValue; 

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("ControlPanel\\Volume"), 0, 0, &hKey)) 
	{
		//lRt=RegSetValueEx(hKey, TEXT("Screen"), 0, REG_DWORD, (GBYTE*)&curValue, sizeof(GDWORD));
		lRt=RegQueryValueEx(hKey, TEXT("Screen"), 0, (LPDWORD)&u4Type, (GBYTE*)&curValue, (LPDWORD)&u4Count);
		RegCloseKey(hKey);
	}

	if (curValue != 0)
	{
		*getValue = SETTING_AUDIO_KEYBEEP_ON;

	}
	else
	{
		*getValue = SETTING_AUDIO_KEYBEEP_OFF;

	}

	//TCHAR szGetString[4];
	//getStorageValue(INI_TYPE_USER, INI_KEY_AUDIO_KEYBEEP_NAME, szGetString, INI_KEY_AUDIO_KEYBEEP_DEFAULT);
	//*getValue = _wtoi(szGetString);

	//TODO
    //*getValue = 0;
    return TRUE;
}

BOOL SettingsAudio::setEQ(UINT32* pSetValue)
{
	return newSetEQ_smooth( pSetValue );




	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;
	int iIndex = 0;

	if((int)*pSetValue > SETTING_AUDIO_EQ_SOFT)
		*pSetValue = SETTING_AUDIO_EQ_NONE;
	else if((int) *pSetValue < SETTING_AUDIO_EQ_NONE)
		*pSetValue = SETTING_AUDIO_EQ_SOFT;

#if MISC_CLIENT_SET_GAIN
	iIndex = *pSetValue;
	if ((iIndex >= SETTING_AUDIO_EQ_NONE) && (SETTING_AUDIO_EQ_NONE<=SETTING_AUDIO_EQ_SOFT))
	{
		u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)*pSetValue,gEQTypeGain[iIndex]);
	}

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		DVP_GSetEQType(TRUE,(EQTYPE_T)*pSetValue);

#else


#if NEW_API
	u4ReturnValue = GSetEQType(hAudioDev, (EQTYPE_T)*pSetValue);
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		DVP_GSetEQType(TRUE,(EQTYPE_T)*pSetValue);
#else
	u4ReturnValue = GSetEQType(hAudioDev, (EQTYPE_T)*pSetValue);
#endif

#endif 

	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}


void GetSmoothValue( GUINT32 nDestV,GUINT32 nCurV,GUINT32 &fv,GUINT32 &mv,GUINT32 &bv )
{
	//GUINT32 nFlag = 0xFF000000;
	//if ( (nDestV&nFlag) !=(nCurV&nFlag) )
	//	mv = 0x00000000;
	int nIndexDest = -1;
	int nIndexCur = -1;
	for ( int i=0;i<MAX_EQ_LEVEL;i++ )
	{
		if ( nDestV==g_ganValues[i] )
			nIndexDest = i;
		if ( nCurV==g_ganValues[i] )
			nIndexCur = i;
	}
	if ( -1==nIndexDest || -1==nIndexCur )
	{
		RETAILMSG(1,(L"####### EQ GetSmoothValue GetEQ index ERROR (nDestV=0x%X) (nCurV=0x%X) \r\n",nDestV,nCurV ));
		fv = mv= bv = 0x00000000;
	}
	
	int nMidIndex,nFIndex,nBIndex;
	nMidIndex = (nIndexCur-nIndexDest)/2 + nIndexDest;
	nFIndex = (nMidIndex-nIndexCur)/2 + nIndexCur; 
	nBIndex = (nMidIndex-nIndexDest)/2 + nIndexDest;

	fv = g_ganValues[nFIndex];
	mv = g_ganValues[nMidIndex];
	bv  = g_ganValues[nBIndex];

}

BOOL	SettingsAudio::newSetEQ_smooth( UINT32* pSetValue )
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;

	if((int)*pSetValue > SETTING_AUDIO_EQ_SOFT)
		*pSetValue = SETTING_AUDIO_EQ_NONE;
	else if((int) *pSetValue < SETTING_AUDIO_EQ_NONE)
		*pSetValue = SETTING_AUDIO_EQ_SOFT;


	int iIndex = *pSetValue;
	MISC_EQ_GAIN_T uEqGain;
	memset( &uEqGain,0,sizeof(MISC_EQ_GAIN_T) );
	UINT32 getValue = SETTING_AUDIO_EQ_NONE;
	GClientGetEQType( hAudioDev, (EQTYPE_T *) &getValue,&uEqGain );


	//每个EQ频点 使用3个值进行过度
	MISC_EQ_GAIN_T uEqGainMid, uEqGainFront,uEqGainBack;
	uEqGainMid.u4Gain0 = uEqGainFront.u4Gain0 = uEqGainBack.u4Gain0 = 0x00010000;
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain1, uEqGain.u4Gain1,  uEqGainFront.u4Gain1, uEqGainMid.u4Gain1, uEqGainBack.u4Gain1 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain2, uEqGain.u4Gain2,  uEqGainFront.u4Gain2, uEqGainMid.u4Gain2, uEqGainBack.u4Gain2 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain3, uEqGain.u4Gain3,  uEqGainFront.u4Gain3, uEqGainMid.u4Gain3, uEqGainBack.u4Gain3 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain4, uEqGain.u4Gain4,  uEqGainFront.u4Gain4, uEqGainMid.u4Gain4, uEqGainBack.u4Gain4 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain5, uEqGain.u4Gain5,  uEqGainFront.u4Gain5, uEqGainMid.u4Gain5, uEqGainBack.u4Gain5 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain6, uEqGain.u4Gain6,  uEqGainFront.u4Gain6, uEqGainMid.u4Gain6, uEqGainBack.u4Gain6 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain7, uEqGain.u4Gain7,  uEqGainFront.u4Gain7, uEqGainMid.u4Gain7, uEqGainBack.u4Gain7 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain8, uEqGain.u4Gain8,  uEqGainFront.u4Gain8, uEqGainMid.u4Gain8, uEqGainBack.u4Gain8 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain9, uEqGain.u4Gain9,  uEqGainFront.u4Gain9, uEqGainMid.u4Gain9, uEqGainBack.u4Gain9 );
	GetSmoothValue( gEQTypeGain[iIndex].u4Gain10, uEqGain.u4Gain10,  uEqGainFront.u4Gain10, uEqGainMid.u4Gain10, uEqGainBack.u4Gain10 );


	u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)getValue, uEqGainFront );
	u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)getValue, uEqGainMid );
	u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)getValue, uEqGainBack );



	if ((iIndex >= SETTING_AUDIO_EQ_NONE) && (SETTING_AUDIO_EQ_NONE<=SETTING_AUDIO_EQ_SOFT))
	{
		u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)*pSetValue,gEQTypeGain[iIndex]);
	}

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		DVP_GSetEQType(TRUE,(EQTYPE_T)*pSetValue);


	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}


BOOL SettingsAudio::SetCustomEQValue(MISC_EQ_GAIN_T eqGain )
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue=0;
	u4ReturnValue=GClientSetEQValues(hAudioDev,eqGain,TRUE);
	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}


BOOL SettingsAudio::getEQ(UINT32* getValue)
{
	UINT32 u4ReturnValue = 0;
	GHANDLE hAudioDev = GOpenAudioDev();

	MISC_EQ_GAIN_T uEqGain;

	//TODO
	*getValue = 0;

#if MISC_CLIENT_SET_GAIN

	GClientGetEQType(hAudioDev, (EQTYPE_T *) getValue,&uEqGain);

#else

#if NEW_API
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GGetEQType(TRUE, (EQTYPE_T *) getValue);
#else
	u4ReturnValue = GGetEQType(hAudioDev, (EQTYPE_T *) getValue);
#endif

#endif 
	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}


BOOL SettingsAudio::setFrontVolume(UINT32* pSetValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;

	GUINT32 uVolGain = 0;


#if MISC_CLIENT_SET_GAIN

	uVolGain = (*pSetValue)*FULL_VOLUME_GAIN/MAX_VOLUME_INDEX;
	u4ReturnValue = GClientSetVolume(hAudioDev, uVolGain);
#else

	u4ReturnValue = GSetVolume(hAudioDev, *pSetValue);
#endif 
	GCloseAudioDev(hAudioDev);

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetVolume(TRUE, *pSetValue);

	return u4ReturnValue;
}

BOOL SettingsAudio::setRearVolume(UINT32* pSetValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;

	GUINT32 uVolGain = 0;

#if MISC_CLIENT_SET_GAIN 
	uVolGain = (*pSetValue)*FULL_VOLUME_GAIN/MAX_VOLUME_INDEX;
	u4ReturnValue = GClientSetRearVolume(hAudioDev, uVolGain);
#else
	u4ReturnValue = GSetRearVolume(hAudioDev, *pSetValue);
#endif
	GCloseAudioDev(hAudioDev);

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetRearVolume(TRUE,*pSetValue);

	return u4ReturnValue;
}

//BOOL SettingsAudio::getFrontVolume(UINT32* getValue)
//{
//	GHANDLE hAudioDev = GOpenAudioDev();
//	UINT32 u4ReturnValue;
//
//	GUINT32 uMode = 0;
//
//	GUINT32 uVolGain = 0;
//
//#if MISC_CLIENT_SET_GAIN
//
//	u4ReturnValue = GClientGetVolume(hAudioDev, &uVolGain);
//	*getValue = (uVolGain*MAX_VOLUME_INDEX)/FULL_VOLUME_GAIN;
//
//	uMode = (uVolGain*MAX_VOLUME_INDEX) % FULL_VOLUME_GAIN;
//
//	if (uMode!=0)
//	{
//		*getValue = *getValue + 1;
//
//	}
//#else
//	u4ReturnValue = GGetVolume(hAudioDev, (INT32*) getValue);
//#endif 
//
//	GCloseAudioDev(hAudioDev);
//	return u4ReturnValue;
//}
//
BOOL SettingsAudio::getRearVolume(UINT32* getValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;

	GUINT32 uMode = 0;
	GUINT32 uVolGain = 0;

#if MISC_CLIENT_SET_GAIN
	u4ReturnValue = GClientGetRearVolume(hAudioDev, &uVolGain);
	*getValue = uVolGain*MAX_VOLUME_INDEX/FULL_VOLUME_GAIN;

	uMode = (uVolGain*MAX_VOLUME_INDEX) % FULL_VOLUME_GAIN;

	if (uMode!=0)
	{
		*getValue = *getValue + 1;

	}

#else
	u4ReturnValue = GGetRearVolume(hAudioDev, (INT32*) getValue);
#endif 

	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}




void  SettingsAudio::setUpmix()
{
	GHANDLE hAudioDev = GOpenAudioDev();

	MISC_UPMIX_GAIN_T rUpmixGain;

	INT32 i4Upmix = AUD_UPMIX_ON;
	{
		rUpmixGain.u4Gain0 = 0x004CCCCC;
		rUpmixGain.u4Gain1 = 0x00199999;
		rUpmixGain.u4Gain2 = 0x00199999;
		rUpmixGain.u4Gain3 = 0x004CCCCC;
		rUpmixGain.u4Gain4 = 0x00333333;
		rUpmixGain.u4Gain5 = 0x00333333;
		rUpmixGain.u4Gain6 = 0x00666666;
		rUpmixGain.u4Gain7 = 0x00666666;

	}
	GClientSetUpMix(hAudioDev,(AUD_UPMIX_T) i4Upmix,rUpmixGain);
	GCloseAudioDev(hAudioDev);
}

#define SPEAKER_LAYOUT_MASK 0xFF
void SettingsAudio::setDownMix()
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT u4SpeakerSize = 0;
	UINT32 u4SpeakerLayout;	

	GGetSpeakerLayout(hAudioDev, &u4SpeakerLayout, &u4SpeakerSize);
	u4SpeakerSize = 0;	// 所有的用小喇叭
	//  	u4SpeakerSize &= (~(SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE));
	//  	u4SpeakerSize &= (~(SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE));
	// 	u4SpeakerSize &= (~(SPEAKER_LAYOUT_C_LARGE));

	// 打开所有声道
	u4SpeakerLayout = (u4SpeakerLayout & (~SPEAKER_LAYOUT_MASK)) | SPEAKER_LAYOUT_LRCLSRS | SPEAKER_LAYOUT_SUBWOOFER;

	GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);

	GCloseAudioDev(hAudioDev);

}

void	SettingsAudio::InitRearSpeaker(  )
{
	//setDownMix();
	//setUpmix();
	//return;

	if ( config::get_config_functions()->is_support(SOURCE_DVD) 
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)
	
	{
		DVP_Init();
	}

	GHANDLE hAudioDev = GOpenAudioDev();

	UINT32 u4SpeakerSize = 0;
	UINT32 u4SpeakerLayout = SPEAKER_LAYOUT_STEREO;

	//SetFront();
	UINT32 u4ReturnValue;
	u4SpeakerSize |= (SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE | SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE);

	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);


	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);


	//SetRear();
	UINT32 u4TempSpeakerLayout;
	u4SpeakerSize &= (~(SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE));

	//设置是否带中置音  SPEAKER_LAYOUT_LRCLSRS
	u4TempSpeakerLayout = SPEAKER_LAYOUT_LRLSRS; // SPEAKER_LAYOUT_LRCLSRS  SPEAKER_LAYOUT_LRLSRS; 
	u4SpeakerLayout = (u4SpeakerLayout & (~SPEAKER_LAYOUT_MASK)) | u4TempSpeakerLayout;
	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);


	//SetUpMix();
	INT32 i4Upmix;
	MISC_UPMIX_GAIN_T rUpmixGain;
	i4Upmix = AUD_UPMIX_ON;

	//1.5797835966168101567500723704814
	//1.7993405494535817053022720651029
	//1.6901960800285136614244325171853



	//rUpmixGain.u4Gain0 = 0x004CCCCC;
	//rUpmixGain.u4Gain1 = 0x00199999;
	//rUpmixGain.u4Gain2 = 0x00199999;
	//rUpmixGain.u4Gain3 = 0x004CCCCC;

	//rUpmixGain.u4Gain0 =   0x007fffff;
	//rUpmixGain.u4Gain3 =   0x007fffff;

	rUpmixGain.u4Gain0 =   0x00620000;
	rUpmixGain.u4Gain1 =   0x00000000;
	rUpmixGain.u4Gain2 =   0x00000000;
	rUpmixGain.u4Gain3 =   0x00620000;

	rUpmixGain.u4Gain4 = 0x00333333;
	rUpmixGain.u4Gain5 = 0x00333333;
	rUpmixGain.u4Gain6 = 0x00666666;
	rUpmixGain.u4Gain7 = 0x00666666;
	GClientSetUpMix(hAudioDev,(AUD_UPMIX_T) i4Upmix,rUpmixGain);


	//SetSub();
	u4SpeakerLayout |= SPEAKER_LAYOUT_SUBWOOFER;
	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);

	//SetAudioFuncOption();
	MISC_AUD_FUNC_OPTION_T rFuncOptionSet = {0};
	rFuncOptionSet.u4FuncOption0 = 1;
	//根据MTK提示，调节EQ时，把平滑开关打开
	rFuncOptionSet.u4FuncOption0 = 1 | (1<<2);

	GAudioFuncOptionSet(hAudioDev, rFuncOptionSet);

	GCloseAudioDev(hAudioDev);

	if ( config::get_config_functions()->is_support(SOURCE_DVD)
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)
	{
		DVP_Deinit();
	}

}

void   SettingsAudio::GetEqTypeBassTreble( UINT32 nEQType,UINT32* bassValue, UINT32* trebleValue )
{
	if( nEQType> SETTING_AUDIO_EQ_SOFT)
		nEQType = SETTING_AUDIO_EQ_NONE;
	else if( nEQType < SETTING_AUDIO_EQ_NONE)
		nEQType = SETTING_AUDIO_EQ_SOFT;

	for(int i=0;i < MAX_EQ_LEVEL;i++)
	{
		if ( gEQTypeGain[nEQType].u4Gain1 == g_ganValues[i])
		{
			*bassValue = i;
		}
		if ( gEQTypeGain[nEQType].u4Gain9 == g_ganValues[i])
		{
			*trebleValue = i;
		}
	}

}

void    SettingsAudio::CalculatorBalance( POINT ptPos,MISC_DVP_BALANCES_T& bv )
{
	// 设计成在中间时,四个喇叭最大音量,往上移时,后喇叭衰减,前喇叭不变,反之亦然
	// 往左移时，右喇叭衰减,左喇叭不变,反之亦然
	// pt范围是-20~20, Balance表有40级,所以一个点衰减二级
	
	// 确保参数合法
	ptPos.x = min( ptPos.x,MAX_EQ_BALANCE_LEVEL/2);
	ptPos.x = max( ptPos.x,-(MAX_EQ_BALANCE_LEVEL/2) );
	ptPos.y = min( ptPos.y,MAX_EQ_BALANCE_LEVEL/2);
	ptPos.y = max( ptPos.y,-(MAX_EQ_BALANCE_LEVEL/2) );

	// 四个喇叭的衰减值,如果上下，左右都对该喇叭有影响,使用衰减大的那个值
	int attenuate_fl = 0;
	int attenuate_fr = 0;
	int attenuate_rl = 0;
	int attenuate_rr = 0;
	if (ptPos.x >=0)	// 前左后左要衰减
	{
		attenuate_fl = ptPos.x;
		attenuate_rl = ptPos.x;
	}
	else	// 前右后后
	{
		attenuate_fr = abs(ptPos.x);
		attenuate_rr = abs(ptPos.x);
	}

	if (ptPos.y >=0)	// 后左，后右
	{
		attenuate_rl = max(attenuate_rl, ptPos.y);
		attenuate_rr = max(attenuate_rr, ptPos.y);
	}
	else	// 前左前右
	{
		attenuate_fl = max(attenuate_fl, abs(ptPos.y));
		attenuate_fr = max(attenuate_fr, abs(ptPos.y));
	}

	// 缺省在中间时,每个喇叭最大音量
	bv.i4FLeftValue = MAX_EQ_BALANCE_LEVEL - attenuate_fl*2;
	bv.i4FRightValue = MAX_EQ_BALANCE_LEVEL - attenuate_fr*2;
	bv.i4RLeftValue = MAX_EQ_BALANCE_LEVEL - attenuate_rl*2;
	bv.i4RRightValue = MAX_EQ_BALANCE_LEVEL - attenuate_rr*2;

}

BOOL SettingsAudio::SetBalance( MISC_DVP_BALANCES_T &rbv )
{
	BOOL br = FALSE;
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;

	if ( hAudioDev )
	{
		u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[rbv.i4FLeftValue], BALANCE_FRONT_LEFT);
		u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[rbv.i4FRightValue],BALANCE_FRONT_RIGHT  );
		u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[rbv.i4RLeftValue], BALANCE_REAR_LEFT );
		u4ReturnValue = GClientSetBalance(hAudioDev, _au4TrimValue[rbv.i4RRightValue], BALANCE_REAR_RIGHT);
		GCloseAudioDev(hAudioDev);
		br =  TRUE;
	}

	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		u4ReturnValue  = DVP_GSetBalances(TRUE,rbv);

	return br;
}

void	SettingsAudio::DebugMTK_AudioInter()
{
	return;
#if 1


#else
	UINT32 EQ_USer = CFG_EQ_ROCK;

	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)EQ_USer,gEQTypeGain[EQ_USer]);
	
	MISC_EQ_GAIN_T EQGain;
	memset( &EQGain,0,sizeof(MISC_EQ_GAIN_T) );
	UINT32 ur =  GClientGetEQValues( hAudioDev, &EQGain );
	GCloseAudioDev(hAudioDev);


	hAudioDev = GOpenAudioDev();
	EQ_USer = 7;
	MISC_EQ_GAIN_T	eqGainUser = {0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A,0xFFFFCB5A};
	UINT32 u4ReturnValue1 = GClientSetEQType(hAudioDev, (EQTYPE_T)EQ_USer,eqGainUser);
	GCloseAudioDev(hAudioDev);

	hAudioDev = GOpenAudioDev();
	EQ_USer = 0;
	MISC_EQ_GAIN_T	eqGainUserGet;
	u4ReturnValue = GClientGetEQType(hAudioDev, (EQTYPE_T*)(&EQ_USer),&eqGainUserGet);
	GCloseAudioDev(hAudioDev);




	MISC_EQ_GAIN_T	eqGainUser2 = { 0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B,0x0002298B };
	u4ReturnValue = GClientSetEQType(hAudioDev, (EQTYPE_T)EQ_USer,eqGainUser2);
	GCloseAudioDev(hAudioDev);

#endif;

}


BOOL SettingsAudio::SetSubwoofer(UINT32 setValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue=0;

#if MISC_CLIENT_SET_GAIN
	u4ReturnValue=GClientSetBalance(hAudioDev, _au4TrimValue_sub[setValue], BALANCE_SUB_WOOFER);
#else
#endif 
	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}

BOOL SettingsAudio::GetSubwoofer(UINT32* getValue)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue=0;
	INT32 i4ReadValue = 0;

#if MISC_CLIENT_SET_GAIN
	GUINT32 i4LeftValue;
	u4ReturnValue = GClientGetBalance(hAudioDev, &i4LeftValue, BALANCE_SUB_WOOFER);

	for (int i=0;i<TRIM_LEVEL_MAX;i++)
	{
		if (_au4TrimValue_sub[i] == i4LeftValue)
		{
			i4ReadValue = i;
			break;
		}
	}
	*getValue=i4ReadValue;
#else
#endif 
	GCloseAudioDev(hAudioDev);
	return u4ReturnValue;
}

//////////////////////////////////////////////////////////////////
///////                add by tiger              /////////////////

SettingsAudio* SettingsAudio::GetInstance()
{
	static SettingsAudio set_audio;
	return &set_audio;
}

BOOL SettingsAudio::GetEQValues(UINT eqType, UINT32* pValues)
{
	if ( eqType < SETTING_AUDIO_EQ_NONE || eqType> SETTING_AUDIO_EQ_SOFT || !pValues)
	{
		return FALSE;
	}


	for(int i=0;i < MAX_EQ_LEVEL;i++)
	{
		if ( gEQTypeGain[eqType].u4Gain1 == g_ganValues[i])
		{
			*(pValues+0) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain2 == g_ganValues[i])
		{
			*(pValues+1) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain3 == g_ganValues[i])
		{
			*(pValues+2) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain4 == g_ganValues[i])
		{
			*(pValues+3) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain5 == g_ganValues[i])
		{
			*(pValues+4) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain6 == g_ganValues[i])
		{
			*(pValues+5) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain7 == g_ganValues[i])
		{
			*(pValues+6) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain8 == g_ganValues[i])
		{
			*(pValues+7) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain9 == g_ganValues[i])
		{
			*(pValues+8) = i;
		}
		if ( gEQTypeGain[eqType].u4Gain10 == g_ganValues[i])
		{
			*(pValues+9) = i;
		}
	}

	return TRUE;
}

UINT SettingsAudio::GetEQGainbyIndex(int index)
{
	if (index>=0 && index<MAX_EQ_LEVEL)
	{
		return g_ganValues[index];
	}

	return g_ganValues[MAX_EQ_LEVEL/2];
}

BOOL SettingsAudio::EnableSubwoofer( BOOL bEnable )
{
	if ( config::get_config_functions()->is_support(SOURCE_DVD) 
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)
	{
		DVP_Init();
	}

	GHANDLE hAudioDev = GOpenAudioDev();
	UINT32 u4ReturnValue;
	UINT32 u4SpeakerSize = 0;

	UINT32 u4SpeakerLayout = SPEAKER_LAYOUT_LRLSRS;
	if ( bEnable )
	{
		u4SpeakerLayout |= SPEAKER_LAYOUT_SUBWOOFER;
		u4SpeakerSize &= (~(SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE | SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE));
	}
	else
	{
		u4SpeakerSize &= (~(SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE | SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE));
	}

	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
	{
		u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);
	}
	GCloseAudioDev(hAudioDev);

	if ( config::get_config_functions()->is_support(SOURCE_DVD) 
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)

	{
		DVP_Deinit();
	}

	return TRUE;
}


void SettingsAudio::SetCustomEQbyIndex(UINT* pvalue)
{
	if (pvalue)
	{
		MISC_EQ_GAIN_T eqGain;
		eqGain.u4Gain0 = 0x00010000;
		UINT *pgain = &eqGain.u4Gain1;
		for (int i=0; i<10; i++)
		{
			*(pgain+i) = GetEQGainbyIndex(pvalue[i]);
		}
		SetCustomEQValue(eqGain);
	}

}