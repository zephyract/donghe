
#ifndef __CSetVideoParams_H__
#define __CSetVideoParams_H__


#include <windows.h>
#include "IniDefine.h"
#include "GSysServices.h"


enum
{
    SETTINGS_VIDEO_BRIGHTNESS,
    SETTINGS_VIDEO_CONTRAST,
    SETTINGS_VIDEO_HUE,
    SETTINGS_VIDEO_SATURATION,
    SETTINGS_VIDEO_BACKLIGHT,
    SETTINGS_VIDEO_END,
};


extern int  YC_VIDEO_BRIGHTNESS_MIN;       
extern int  YC_VIDEO_BRIGHTNESS_MAX;   





#pragma once

class CSetVideoParams
{
public:
	CSetVideoParams();
	~CSetVideoParams();
public:

	static BOOL	setValue(BYTE index, UINT32* setValue);
	static BOOL	getValue(BYTE index, UINT32* getValue);


    static BOOL    setBrightness(UINT32* setValue);
    static BOOL    setContrast(UINT32* setValue);
    static BOOL    setHue(UINT32* setValue);
    static BOOL    setSaturation(UINT32* setValue);
    static BOOL    setBacklight(UINT32* setValue);

    static BOOL    getBrightness(UINT32* getValue);
    static BOOL    getContrast(UINT32* getValue);
    static BOOL    getHue(UINT32* getValue);
    static BOOL    getSaturation(UINT32* getValue);
    static BOOL    getBacklight(UINT32* getValue);

};

#endif