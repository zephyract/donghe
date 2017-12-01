// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"

#define WCEUI_CLASS_NAME(class_name) (_T(#class_name))

class CWceUiObject;
CWceUiObject*  WceUiCreateObject(IN LPCTSTR pClassName);

CWceUiObject*  WceUiDynamicWithoutCreateObject(void);

typedef CWceUiObject* (*FuncWceUiCreateObj)(void);


struct WCEUI_INIT_DYN_INFO 
{
	WCEUI_INIT_DYN_INFO(IN FuncWceUiCreateObj pFunc, IN LPCTSTR pClassName, IN LPCTSTR pBaseClassName);
};

#define WCEUI_DYNAMIC_BEGIN(class_name, base_class_name) \
	public:\
		void class_name##init_dynamic(){\
			SetClassName(WCEUI_CLASS_NAME(class_name));\
		}\
		struct class_name##WCEUI_INIT_DYNAMIC{\
			class_name##WCEUI_INIT_DYNAMIC(){\
				WCEUI_METHOD_PROLOGUE(class_name,class_name##WCEUI_INIT_DYNAMIC)\
				pThis->class_name##init_dynamic();\
			}\
		}m_x##class_name##WCEUI_INIT_DYNAMIC;\
	private:

#define WCEUI_DYNCREATE_BEGIN(class_name, base_class_name) \
	WCEUI_DYNAMIC_BEGIN(class_name, base_class_name)

#define WCEUI_DYNAMIC_END(class_name, base_class_name) \
	static WCEUI_INIT_DYN_INFO init_dyn_info##class_name(\
		WceUiDynamicWithoutCreateObject, \
		WCEUI_CLASS_NAME(class_name),\
		WCEUI_CLASS_NAME(base_class_name)\
	);


#define WCEUI_DYNCREATE_END(class_name, base_class_name) \
	inline CWceUiObject* FuncWceUiCreateObj##class_name(){\
		return new class_name;\
	}\
	static WCEUI_INIT_DYN_INFO init_dyn_info##class_name(\
		FuncWceUiCreateObj##class_name, \
		WCEUI_CLASS_NAME(class_name),\
		WCEUI_CLASS_NAME(base_class_name)\
	);

//////////////////////////////////////////////////////////////////////////
/////////////////////     class CWceUiObject     /////////////////////////
//////////////////////////////////////////////////////////////////////////
class CWceUiObject
{
	WCEUI_DYNCREATE_BEGIN(CWceUiObject, NULL)
public:
	CWceUiObject(void);
	virtual ~CWceUiObject(void);

public:
	BOOL	IsKindOf(IN LPCTSTR pBaseClassName) const;
	LPCTSTR GetClassName() const;

protected:
	void	SetClassName(IN LPCTSTR pClassName);

private:
	LPCTSTR m_xlpszWceUiClassName;

private:
	CWceUiObject (const CWceUiObject &);
	CWceUiObject & operator=(const CWceUiObject &);
};

WCEUI_DYNCREATE_END(CWceUiObject, NULL)

