// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
#pragma once
#include "WceUiCommon.h"

#define WCEUI_TYPE

#ifdef WCEUI_TYPE
	#ifndef _AFX
		#ifndef ASSERT
			#include <assert.h>
			#define ASSERT assert
		#endif
		#include "WceUiTypes.h"
	#endif
#endif