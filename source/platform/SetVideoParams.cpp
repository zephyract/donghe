#include "stdafx.h"
#include <stdlib.h>
#include "SetVideoParams.h"

int  YC_VIDEO_BRIGHTNESS_MIN	 =  VIDEO_BRIGHTNESS_MIN;
int  YC_VIDEO_BRIGHTNESS_MAX    =  VIDEO_BRIGHTNESS_MAX;   //0xFFFFFFFF

CSetVideoParams::CSetVideoParams()
{
}

CSetVideoParams::~CSetVideoParams()
{
}

BOOL CSetVideoParams::setValue(BYTE index, UINT32* setValue)
{
	BOOL bReturnValue;
    switch(index)
    {
		case SETTINGS_VIDEO_BRIGHTNESS:
			bReturnValue = setBrightness(setValue);
			break;
		case SETTINGS_VIDEO_CONTRAST:
			bReturnValue = setContrast(setValue);
			break;
		case SETTINGS_VIDEO_HUE:
			bReturnValue = setHue(setValue);
			break;
        case SETTINGS_VIDEO_SATURATION:
            bReturnValue = setSaturation(setValue);
            break;
		case SETTINGS_VIDEO_BACKLIGHT:
			bReturnValue = setBacklight(setValue);
			break;
		default:
			bReturnValue = FALSE;
    }

	return bReturnValue;
}

BOOL CSetVideoParams::getValue(BYTE index, UINT32* getValue)
{
	BOOL bReturnValue;
    switch(index)
    {
        case SETTINGS_VIDEO_BRIGHTNESS:
            bReturnValue = getBrightness(getValue);
            break;
        case SETTINGS_VIDEO_CONTRAST:
            bReturnValue = getContrast(getValue);
            break;
        case SETTINGS_VIDEO_HUE:
            bReturnValue = getHue(getValue);
            break;
        case SETTINGS_VIDEO_SATURATION:
            bReturnValue = getSaturation(getValue);
            break;
        case SETTINGS_VIDEO_BACKLIGHT:
            bReturnValue = getBacklight(getValue);
            break;
        default:
            bReturnValue = FALSE;
			break;
    }
	return bReturnValue;
}

BOOL CSetVideoParams::setBrightness(UINT32* pSetValue)
{
    UINT32 u4RegValue = (YC_VIDEO_BRIGHTNESS_MAX-YC_VIDEO_BRIGHTNESS_MIN)* (*pSetValue - VIDEO_BRIGHTNESS_UI_MIN) / (VIDEO_BRIGHTNESS_UI_MAX - VIDEO_BRIGHTNESS_UI_MIN)  + YC_VIDEO_BRIGHTNESS_MIN;
    return GSetBrightnessLevel(u4RegValue);
}
BOOL CSetVideoParams::setContrast(UINT32* pSetValue)
{
    UINT32 u4RegValue = (VIDEO_CONTRAST_MAX-VIDEO_CONTRAST_MIN) *(*pSetValue - VIDEO_CONTRAST_UI_MIN) / (VIDEO_CONTRAST_UI_MAX - VIDEO_CONTRAST_UI_MIN)   + VIDEO_CONTRAST_MIN;
    return GSetContrastLevel(u4RegValue);
}
BOOL CSetVideoParams::setHue(UINT32* pSetValue)
{
    UINT32 u4RegValue = (VIDEO_HUE_MAX-VIDEO_HUE_MIN) / (VIDEO_HUE_UI_MAX - VIDEO_HUE_UI_MIN) * (*pSetValue - VIDEO_HUE_UI_MIN) + VIDEO_HUE_MIN;
    return GSetHueLevel(u4RegValue);
}
BOOL CSetVideoParams::setSaturation(UINT32* pSetValue)
{
    UINT32 u4RegValue = (VIDEO_SATUTATION_MAX-VIDEO_SATUTATION_MIN) / (VIDEO_SATUTATION_UI_MAX - VIDEO_SATUTATION_UI_MIN) * (*pSetValue - VIDEO_SATUTATION_UI_MIN) + VIDEO_SATUTATION_MIN;
    return GSetSaturationLevel(u4RegValue);
}
BOOL CSetVideoParams::setBacklight(UINT32* pSetValue)
{
    UINT32 u4RegValue = (VIDEO_BACKLIGHT_MAX-VIDEO_BACKLIGHT_MIN) / (VIDEO_BACKLIGHT_UI_MAX - VIDEO_BACKLIGHT_UI_MIN) * (*pSetValue - VIDEO_BACKLIGHT_UI_MIN) + VIDEO_BACKLIGHT_MIN;
    return GSetBackLightLevel(u4RegValue);
    //return TRUE;
}


BOOL CSetVideoParams::getBrightness(UINT32* pGetValue)
{
    UINT32 u4RegValue;
    if(!GGetBrightnessFromHW(&u4RegValue))
    {
		if ((int)u4RegValue < YC_VIDEO_BRIGHTNESS_MIN )
		{
			u4RegValue = YC_VIDEO_BRIGHTNESS_MIN;
		}
        //*pGetValue = (u4RegValue - VIDEO_BRIGHTNESS_MIN) / ((VIDEO_BRIGHTNESS_MAX-VIDEO_BRIGHTNESS_MIN) / (VIDEO_BRIGHTNESS_UI_MAX - VIDEO_BRIGHTNESS_UI_MIN)) + VIDEO_BRIGHTNESS_UI_MIN;
		*pGetValue = (u4RegValue - YC_VIDEO_BRIGHTNESS_MIN) * (VIDEO_BRIGHTNESS_UI_MAX - VIDEO_BRIGHTNESS_UI_MIN)/(YC_VIDEO_BRIGHTNESS_MAX-YC_VIDEO_BRIGHTNESS_MIN) + VIDEO_BRIGHTNESS_UI_MIN;
        return TRUE;
    }
    else
    {
        *pGetValue = _wtoi(INI_KEY_VIDEO_BRIGHTNESS_DEFAULT);
        return FALSE;
    }
}
BOOL CSetVideoParams::getContrast(UINT32* pGetValue)
{
    UINT32 u4RegValue;
    if(!GGetContrastFromHW(&u4RegValue))
    {
		if (u4RegValue < VIDEO_CONTRAST_MIN )
		{
			u4RegValue = VIDEO_CONTRAST_MIN;
		}
        *pGetValue = (u4RegValue - VIDEO_CONTRAST_MIN)* (VIDEO_CONTRAST_UI_MAX - VIDEO_CONTRAST_UI_MIN) / (VIDEO_CONTRAST_MAX-VIDEO_CONTRAST_MIN ) + VIDEO_CONTRAST_UI_MIN;
        return TRUE;
    }
    else
    {
        *pGetValue = _wtoi(INI_KEY_VIDEO_CONTRAST_DEFAULT);
        return FALSE;
    }
}

BOOL CSetVideoParams::getHue(UINT32* pGetValue)
{
    UINT32 u4RegValue;
    if(!GGetHueFromHW(&u4RegValue))
    {
        *pGetValue = (u4RegValue - VIDEO_HUE_MIN) / ((VIDEO_HUE_MAX-VIDEO_HUE_MIN) / (VIDEO_HUE_UI_MAX - VIDEO_HUE_UI_MIN)) + VIDEO_HUE_UI_MIN;
        return TRUE;
    }
    else
    {
        *pGetValue = _wtoi(INI_KEY_VIDEO_HUE_DEFAULT);
        return FALSE;
    }
}
BOOL CSetVideoParams::getSaturation(UINT32* pGetValue)
{
    UINT32 u4RegValue;
    if(!GGetSaturationFromHW(&u4RegValue))
    {
        *pGetValue = (u4RegValue - VIDEO_SATUTATION_MIN) / ((VIDEO_SATUTATION_MAX-VIDEO_SATUTATION_MIN) / (VIDEO_SATUTATION_UI_MAX - VIDEO_SATUTATION_UI_MIN)) + VIDEO_SATUTATION_UI_MIN;
        return TRUE;
    }
    else
    {
        *pGetValue = _wtoi(INI_KEY_VIDEO_SATUTATION_DEFAULT);
        return FALSE;
    }
}
BOOL CSetVideoParams::getBacklight(UINT32* pGetValue)
{
    UINT32 u4RegValue;
#if 1
    if(GGetBackLightLevel(&u4RegValue))
    {
        *pGetValue = (u4RegValue - VIDEO_BACKLIGHT_MIN) / ((VIDEO_BACKLIGHT_MAX-VIDEO_BACKLIGHT_MIN) / (VIDEO_BACKLIGHT_UI_MAX - VIDEO_BACKLIGHT_UI_MIN)) + VIDEO_BACKLIGHT_UI_MIN;
        return TRUE;
    }
    else
#endif
    {
        *pGetValue = _wtoi(INI_KEY_VIDEO_BACKLIGHT_DEFAULT);
        return FALSE;
    }

}
