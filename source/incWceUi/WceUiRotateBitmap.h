// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUi32Bitmap.h"

class TPixels32;
class CWceUiRotateBitmap
{
public:
	CWceUiRotateBitmap(void);
	~CWceUiRotateBitmap(void);

	BOOL LoadFile(LPCTSTR lpszFileName);
	BOOL Rotate(float nAngle, float rotateX, float rotateY, float zoomX, float zoomY);
	CWceUiGenericBitmap* GetBitmap();

	int GetWidth();
	int GetHeight();

protected:
	BOOL Write2Bitmap(const TPixels32* src);
	TPixels32* m_srcPic;
	TPixels32* m_dstPic;
	CWceUi32Bitmap  m_Bitmap;

	float m_fCurAngle;
	float m_fCurRotateX;
	float m_fCurRotateY;
	float m_fCurZoomX;
	float m_fCurZoomY;
};

