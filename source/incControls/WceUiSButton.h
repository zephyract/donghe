#pragma once
#include "wceuiscrolllayer.h"
#include "WceUiLoadBitmap.h"

class CWceUiSButton :	public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiSButton, CWceUiScrollLayer);
public:
	CWceUiSButton(void);
	virtual ~CWceUiSButton(void);

	// wceui override
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);

	// CWceuiScrollLayer override
	virtual int GetScrollDistance(DWORD tick, POINT point);
	virtual void OnScrollWindowEnd();
	virtual void OnMouseScrolled();
	virtual void OnMouseClicked();


	// interface
	int GetStatus();
	void SetStatus(int nStatus, BOOL bRedraw=TRUE);


protected:
	// mask����������ʾ����region,�������ܲ��Ǿ���
	CWceUiLoadBitmap m_bmpMask;
	HRGN m_rgnLayer;
	int m_rgnOffsetX;
	int m_rgnOffsetY;
	// ����ı���ָ����������, �Ȼ��ɻ�����λͼ�ٻ�������
	CWceUiLoadBitmap m_bmpBackground;
	// ����״̬�µĿɻ�����λͼ
	CWceUiLoadBitmap m_bmpNormal;
	// ��ѹ״̬�µĿɻ�����λͼ
	CWceUiLoadBitmap m_bmpDown;

	// �������SButton��λͼ�������ɺ󣬲Ž���λͼ�򵽿��ṩ�Ĵ���λͼ��
	CWceUiGenericBitmap* m_pbmpSButton;


	int m_nMaxScrollPos;
	int m_nStatus;	// 0 ~ off, 1 ~ on
	BOOL m_bMouseScrolled; // ������ʶ�Ƿ����������¼�����������ˣ���������ʱ���ж�״̬�Ƿ��Ѿ��ı�

};
WCEUI_DYNCREATE_END(CWceUiSButton, CWceUiScrollLayer);