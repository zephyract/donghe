// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiLayer.h"

class CWceUiLayerCtrl;
class CWceUiWndAnimate;

namespace wceui
{
	class CWceUiDlgData;
}

class CWceUiDlg : public CWceUiLayer
{
	friend class CWceUiLayerCtrl;
	WCEUI_DYNCREATE_BEGIN(CWceUiDlg, CWceUiLayer)
public:
	CWceUiDlg();
	~CWceUiDlg();

	//
	//virtual functions
	//
	virtual void OnInitDialog();
	virtual void OnLMouseDownOuside(UINT nFlags, POINT point);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);

	virtual BOOL OnApply(BOOL bIn);
	virtual BOOL OnPlay(BOOL bIn);

	//modeless
	BOOL RunModeless();
	BOOL EndModeless();

	//modal
	virtual INT_PTR DoModal();
	BOOL EndDialog(int nResult);

	void SetWndRect(LPCRECT lpRect);
	void GetWndRect(LPRECT lpRect) const;

	WCEUI_DLG_RUN_WAY GetDlgRunWay() const;

public:
	CWceUiXmlDoc* GetWndDoc();
	CWceUiXmlDoc* GetStyleDoc();

	BOOL GetStyleData(LPCTSTR pStyleName, CWceUiStyle& styleData);
	CWceUiWndAnimate* GetWndAnimate(LPCTSTR pName);

protected:
	virtual void OnDrawBegin();
	virtual void OnDrawEnd();

private:
	BOOL LoadStyleFile(LPCTSTR pStyleFile);
	BOOL DoesHaveAnimate(LPCTSTR pName);

private:
	BOOL Init(LPCTSTR pDlgFileFullPath, LPCTSTR pStyleFileFullPath);
	void UnInit();

	void SendOnLoad();
	void SendOnInitLayer();

private:
	wceui::CWceUiDlgData* GetData() const;
	wceui::CWceUiDlgData* m_pData;

private:
	CWceUiDlg (const CWceUiDlg &);
	CWceUiDlg & operator=(const CWceUiDlg &);
};
WCEUI_DYNCREATE_END(CWceUiDlg, CWceUiLayer)