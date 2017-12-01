#pragma once
#include "wceuilayer.h"
class CShowMove;
typedef  CShowMove* PCShowMove;
class CShowMove :    public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CShowMove, CWceUiLayer);
public:
    CShowMove(void);
    ~CShowMove(void);
    virtual void OnInitLayer();
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnDraw();
    void	ClearList();
    int		AddString( CString Str,int iFocusCount =2 );
    void	SetListCount( int iCount );
    void	SetTxtColor( COLORREF clNor,COLORREF clDown );
    int		DeleteString( int index );
    int		GetCount()		{	 return m_StrAyList.GetCount();	}

protected:
    CStringArray		m_StrAyList;		//�ı�
    BOOL				m_bDrawFront;		//�Ƿ���ʼ����
    short				m_iListCount;		//����
    int					m_iTopFocusLine;	//������������

    COLORREF			m_clTxtNor;
    COLORREF			m_clTxtDown;
    BOOL				m_bShowWindow;


};
WCEUI_DYNCREATE_END(CShowMove, CWceUiLayer);