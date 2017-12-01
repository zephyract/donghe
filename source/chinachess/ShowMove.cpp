#include "StdAfx.h"
#include "ShowMove.h"
#include "WceUiRealWnd.h"
CShowMove::CShowMove(void)
{
    m_iListCount = 6;
    m_clTxtDown = 0x0;
    m_clTxtNor =0x0;

}

CShowMove::~CShowMove(void)
{
}

void CShowMove::SetProperties( PCWceUiXmlElem pCreateElem )
{

    ASSERT(pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    LPCTSTR pstrCount  =  pCreateElem->GetAttr(TEXT("rowcount"));
    LPCTSTR pstrNorCL  =  pCreateElem->GetAttr(TEXT("norcl"));
    LPCTSTR pstrDownCL  =  pCreateElem->GetAttr(TEXT("downcl"));

	if (NULL  != pstrCount)
    {
        m_iListCount  = _ttoi(pstrCount);
       // SetListCount(nCount);
    }
    if (NULL != pstrNorCL)
    {
        DWORD nNorCL = _tcstol(pstrNorCL,NULL,16);
        m_clTxtNor  =RGB(nNorCL>>16,(nNorCL - (nNorCL>>16<<16))>>8,nNorCL%256);
    }
    if (NULL != pstrDownCL)
    {
        DWORD nDownCL = _tcstol(pstrDownCL,NULL,16);
        m_clTxtDown  =RGB(nDownCL>>16,(nDownCL - (nDownCL>>16<<16))>>8,nDownCL%256);
    }
     
}

void CShowMove::OnDraw(  )
{
    //by lzy//CWceUiLayer::OnDraw();
    CRect rcDstShow;
    GetWindowRect(rcDstShow);

    HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
    ASSERT(NULL != hfont);

    int ilineht = rcDstShow.Height()/m_iListCount;	
    CRect rcline( 0,rcDstShow.top,rcDstShow.Width(),ilineht );
    int iStrIndex = m_StrAyList.GetCount()-1;
    CString strinfo;
    for ( int i=0;i<m_iListCount&&iStrIndex>=0;i++ )
    {
        if ( i<m_iTopFocusLine )
        {	
            strinfo = m_StrAyList.GetAt( iStrIndex );
			//by lzy//wceui::wceglDrawText(strinfo, rcline, DT_LEFT|DT_VCENTER, m_clTxtDown, hfont);
        }
        else
        {
            strinfo = m_StrAyList.GetAt( iStrIndex );
			//by lzy//wceui::wceglDrawText(strinfo, rcline, DT_LEFT|DT_VCENTER, m_clTxtNor, hfont);
        }
        rcline.OffsetRect( 0,ilineht );
        --iStrIndex;
    }
}

void	CShowMove::ClearList()
{
    m_StrAyList.RemoveAll();
    m_StrAyList.FreeExtra();
    InvalidateRect(NULL);
    GetWindow()->UpdateWindow();
}

int CShowMove::AddString( CString Str ,int iFocusCount )
{
    m_iTopFocusLine = iFocusCount;
    
    DWORD nReturn =   m_StrAyList.Add( Str );
    InvalidateRect(NULL);
      GetWindow()->UpdateWindow();
    return  nReturn;
}

void	CShowMove::SetListCount(  int iCount )
{
    m_iListCount = iCount;
}

int CShowMove::DeleteString( int index )
{
    if ( index>=m_StrAyList.GetCount() )
        return LB_ERR;
    m_StrAyList.RemoveAt( index );
   DWORD nReturn = m_StrAyList.GetCount();
    InvalidateRect(NULL);
    GetWindow()->UpdateWindow();
    return  nReturn;
}

void CShowMove::SetTxtColor( COLORREF clNor,COLORREF clDown )
{
    m_clTxtNor = clNor;
    m_clTxtDown = clDown;
}

void CShowMove::OnInitLayer()
{
    CWceUiLayer::OnInitLayer();
}