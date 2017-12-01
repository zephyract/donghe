// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// Coded by Hui Xu
//
#pragma once
#include <windows.h>

EXTERN_C
{
	int   WceUiZcompress2 (BYTE *dest, unsigned long *destLen, const BYTE *source, unsigned long sourceLen, int level);
	int   WceUiZuncompress(BYTE *dest, unsigned long *destLen, const BYTE *source, unsigned long sourceLen);
};

inline unsigned long FhZcompressBound (unsigned long sourceLen)
{
	return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11;
}