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
	// ���ݵ绰������ص㼰���ȶԿؼ����ַ�����ʽ������
	void GetFormatedString(CString& strFormated);
};
WCEUI_DYNCREATE_END(CBTDialEditBox, CWceUiSEditBox);
