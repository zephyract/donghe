// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiLayerCtrl.h"
#include "WceUiGenericBitmap.h"

class CWceUiDlg;	typedef CWceUiDlg*		PCWceUiDlg;
class CWceUiLayer;	typedef CWceUiLayer*    PCWceUiLayer;
class CWceUiRealWnd;

namespace wceui
{
	class CWceUiLayerCtrlData;
}

class CWceUiLayerCtrl
{
	friend class CWceUiRealWnd;
	friend class CWceUiLayer;
	friend class CWceUiDlg;
public:
	CWceUiLayerCtrl();
	~CWceUiLayerCtrl();

public:
	PCWceUiDlg		CreateDlg(LPCTSTR pDlgPath);

	PCWceUiLayer	CreateLayer(PCWceUiLayer pParent, LPCTSTR pType, LPCTSTR pName = NULL);
	PCWceUiLayer	CreateLayer(PCWceUiLayer pParent, PCWceUiLayer pLeftBrother, LPCTSTR pType, LPCTSTR pName = NULL);

	PCWceUiLayer	CreateLayerByXmlStr(PCWceUiLayer pParent, LPCTSTR pXmlStr);

	void			DeleteLayer(PCWceUiLayer pNode);

	PCWceUiLayer	FindLayerById(WORD dwLayerID);

	CWceUiRealWnd* GetWindow();

	void   UpdateLayersInfo();

	void    ReBuildDrawList();
	void	ReBuildInputList();
	void    ReBuildNeedDrawList(const RECT& rcNeedDraw);

	CWceUiLayer* GetDrawHead() const;
	CWceUiLayer* GetDrawNext(CWceUiLayer* pCurLayer) const;
	CWceUiLayer* GetDrawTail() const;
	CWceUiLayer* GetDrawPre(CWceUiLayer* pCurLayer) const;

	CWceUiLayer* GetNeedDrawHead() const;
	CWceUiLayer* GetNeedDrawNext(CWceUiLayer* pCurLayer) const;

	CWceUiLayer* GetInputHead() const;
	CWceUiLayer* GetInputNext(CWceUiLayer* pCurLayer) const;

	CWceUiLayer* RootLayer() const;

	// 不管该layer可不可见，调用该接口将把layer及其子控件重绘到指定的位图上
	void DrawLayerToBmp(CWceUiLayer* pDrawLayer, CWceUiGenericBitmap* pWndBmp, RECT rcNeedDraw);

	//for window
private:
	void SetWindow(CWceUiRealWnd* pWindow);
	void Draw(CWceUiGenericBitmap* pWndBmp, RECT rcNeedDraw);

	void FreeBmpRes();

	//for myself
private:
	void InitLayerInfo(CWceUiLayer* pNode, LPCTSTR pLayerName);

	void InitBody();
	void UnInitBody();
	void impDeleteElement(CWceUiLayer* pNode);

private:
	PCWceUiLayer impCreateLayer(PCWceUiLayer pParent, LPCTSTR pType);

private:
	wceui::CWceUiLayerCtrlData* GetData() const;
	wceui::CWceUiLayerCtrlData* m_pData;

private:
	CWceUiLayerCtrl (const CWceUiLayerCtrl &);
	CWceUiLayerCtrl & operator=(const CWceUiLayerCtrl &);
};