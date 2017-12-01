#pragma once
#include "WceUiSEditBox.h"

class CBTDialEditBox : public CWceUiSEditBox
{
	WCEUI_DYNCREATE_BEGIN(CBTDialEditBox, CWceUiSEditBox)
public:
	CBTDialEditBox(void);
	~CBTDialEditBox(void);	

	virtual void OnDrawText(CWceUiGenericBitmap* pWndBitmap, BOOL bLayerBitmap);

protected:
	// 根据电话号码的特点及长度对控件的字符串格式化处理
	void GetFormatedString(CString& strFormated);
};
WCEUI_DYNCREATE_END(CBTDialEditBox, CWceUiSEditBox);
