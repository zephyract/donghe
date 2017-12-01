 
/*******************************************************************
* Model   Name�� 
* Function    �� 
* Include File�� 
* Author      �� Zhouxingjie
* EMail       �� shenzhenit@163.com
* Date Created�� 2007��12��3�� ������
* Remark      �� 
* 
*******************************************************************/


#include "stdafx.h"
#include "GameBoxMan.h"
#include <Mmsystem.h>   //��Ч  Winmm.lib


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//����״̬
#define SOUND_STATE_START		0  //��Ϸ��ʼ
#define SOUND_STATE_MOVE		1  //���������ƶ�
#define SOUND_STATE_PUSH		2  //���߲��ƶ�����
#define SOUND_STATE_VICTORY		3  //ʤ��
#define  MY_VK_JUMP 2000

//�ٺ궨��
int		MAX_MISSION_NUM  = 1;
int		iBlockWidth = 20;
int		iBlockHeight = 20;


MovStep::MovStep()
: m_manFromPt(-1,-1),
	m_BoxPtFrom(-1,-1)
{
	m_MovDirection =0;		// VK_LEFT:VK_UP :VK_RIGHT :VK_DOWN :
	m_ManFromType = m_ManToType = m_BoxFromType = m_BoxToType = MAP_BACKGROUP;

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameManBoxMan::CGameManBoxMan(  )
{
	memset( m_cMap,'0',sizeof(m_cMap) );
	m_nMapType = MAPNOR_INRES;

    m_iMissionNum = 1;

	m_pDCBack = NULL;					//��Ϸ����
	m_ptOffset = CPoint(0,0);			//��Ϸ��������λ��

	m_pDCMan = NULL;
	m_szMan = CSize(0,0);
	m_clManTrans = CLR_INVALID;
	m_direction = 0;

	m_nIDRESMapSmall = m_nIDRESMapBig = 0;

}

void  CGameManBoxMan::SetResInfo( LPCTSTR pStrImagePath,UINT nIDMapSmall,UINT nIDMapBig)
{
	m_StrImagePath = pStrImagePath;
	m_nIDRESMapSmall = nIDMapSmall;
	m_nIDRESMapBig = nIDMapBig;

	::MAX_MISSION_NUM = LoadMaxMissionNum();
	LoadMap(m_iMissionNum);
	m_ptManPosition = GetManPosition();


}

CGameManBoxMan::~CGameManBoxMan()
{

}

void CGameManBoxMan::SetManDC( CDC *pDCMan,CSize szman,COLORREF clManTrans )
{
	m_pDCMan = pDCMan;						//��Ϸ�е���
	m_szMan = szman;						//����Ĵ�С
	m_clManTrans = clManTrans;
}

void CGameManBoxMan::SetBackDC( CDC *pDC,CPoint ptOffset )
{
	m_pDCBack = pDC;				//��Ϸ����
	m_ptOffset = ptOffset;			//��Ϸ��������λ��
}

void CGameManBoxMan::SetMapType( UINT nIDType )
{
	if ( nIDType>=MAP_TOTAL )
	{
		 m_nMapType = MAPNOR_INRES; 
	}
	else
		m_nMapType = nIDType;

	memset( m_cMap,'0',sizeof(m_cMap) );
	for ( int i=0;i<M_TAB_HEIGHT;i++)
	{	
		for ( int j=0;j<M_TAB_WIDTH;j++ )
		{
			m_cMap[i][j] = '0';
		}
	}

	m_iMissionNum = 1;
	::MAX_MISSION_NUM = LoadMaxMissionNum();
	LoadMap(m_iMissionNum);
	m_ptManPosition = GetManPosition();

}

int CGameManBoxMan::LoadMaxMissionNumInRes(void)
{
	int iMissionNum = 1;
	HRSRC hRes = NULL;

	if ( m_nMapType==MAPNOR_INRES )
		hRes = FindResource( NULL,  MAKEINTRESOURCE(m_nIDRESMapBig), L"MAP" );
	else
		hRes =  FindResource( NULL,  MAKEINTRESOURCE(m_nIDRESMapSmall), L"MAP" );


	HGLOBAL hResData;
	if (!hRes || !(hResData = LoadResource(NULL,hRes)))
	{
		AfxMessageBox(_T("Load map info error!!! \r\n"));
		return -1;
	}

	// ��Դ���ݵĴ�С
	DWORD dwSize = SizeofResource(NULL,hRes);
	PBYTE pData = (PBYTE)malloc( dwSize );
	memset( pData,0,dwSize );

	if(pData == NULL)
		return -1;

	// ��Դ����
	PBYTE pSrc = (PBYTE)LockResource(hResData);
	if (!pSrc)
	{
		FreeResource(hResData);
		free(pData);
		return -1;
	};
	CopyMemory(pData,pSrc,dwSize);
	PBYTE pMemData = pData;
	BOOL bFindLevel = FALSE;
	//Find level [number]
	while(   pMemData<pData+dwSize-3 )
	{
		if ( *pMemData =='[' && pMemData[1] ==iMissionNum+'0' && pMemData[2] ==']' ) 
		{
			++iMissionNum;
			pMemData += 3;
		}
		else if ( *pMemData =='[' && pMemData[3] ==']' )
		{
			int index = (pMemData[1]-'0')*10+ (pMemData[2]-'0');
			iMissionNum = (index == iMissionNum)?iMissionNum+1:iMissionNum;
			pMemData += 4;
		}
		else
		{
			++pMemData;
		}
	}

	UnlockResource(hResData);
	// �ͷ���Դ
	FreeResource(hResData);
	free(pData);
	return iMissionNum-1;

}

LONG  CGameManBoxMan::LoadMapInRes(int iMissionNum)
{
	HRSRC hRes = NULL;
	if ( m_nMapType==MAPNOR_INRES )
		hRes = FindResource( NULL,  MAKEINTRESOURCE(m_nIDRESMapBig), L"MAP" );
	else
		hRes = FindResource(NULL,  MAKEINTRESOURCE(m_nIDRESMapSmall), L"MAP" );

	HGLOBAL hResData;
	if (!hRes || !(hResData = LoadResource(NULL,hRes)))
	{
		AfxMessageBox(_T("Load map info error!!! \r\n"));
		return -1;
	}

	// ��Դ���ݵĴ�С
	DWORD dwSize = SizeofResource(NULL,hRes);
	PBYTE pData = (PBYTE)malloc( dwSize );

	if(pData == NULL)
		return -1;

	memset( pData,0,dwSize );

	// ��Դ����
	PBYTE pSrc = (PBYTE)LockResource(hResData);
	if (!pSrc)
	{
		FreeResource(hResData);
		free(pData);
		return -1;
	};
	// ��������
	CopyMemory(pData,pSrc,dwSize);

	PBYTE pMemData = pData;
	BOOL bFindLevel = FALSE;
	//Find level [number]
	while(   pMemData<pData+dwSize-2 )
	{
		if ( *pMemData =='[' && pMemData[1] ==iMissionNum+'0' && pMemData[2] ==']' ) 
		{
			bFindLevel = TRUE;
			while(	 pMemData<pData+dwSize-2&& *pMemData !='\r' && *(pMemData+1) !='\n' )
				++pMemData;
			pMemData+=2;
			break;
		}
		else if ( *pMemData =='[' && pMemData[3] ==']' )
		{
			int index = (pMemData[1]-'0')*10+ (pMemData[2]-'0');
			pMemData+=4;
			if (  index==iMissionNum )
			{
				bFindLevel = TRUE;
				while(	 pMemData<pData+dwSize-2&& *pMemData !='\r' && *(pMemData+1) !='\n' )
					++pMemData;
				pMemData+=2;
				break;
			}
		}
		else
		{
			++pMemData;
		}
	}

	if ( !bFindLevel)
		return -1;

	//Get Level map info
	int nX = 0;
	for (int i = 0; i < M_TAB_HEIGHT; i++)
	{
		nX = 0;
		while( nX<M_TAB_WIDTH && *pMemData!='\r' && pMemData<pData+dwSize )
		{
			m_cMap[i][nX] = *pMemData;
			++nX;
			++pMemData;
		}

		while(	 pMemData<pData+dwSize-2&& *pMemData !='\r' && *(pMemData+1) !='\n' )
			++pMemData;
		pMemData+=2;
	}

	UnlockResource(hResData);
	// �ͷ���Դ
	FreeResource(hResData);
	free(pData);
	return iMissionNum;

}

CString CGameManBoxMan::GetProgromRunFolder()
{
	CString strFilePath;
	CString strFolderPath;

	TCHAR lpFileName[MAX_PATH+1];
	GetModuleFileName(NULL, lpFileName, MAX_PATH);
	strFilePath = lpFileName;

	int nLastIndex = strFilePath.ReverseFind('\\');
	if (nLastIndex!=-1) 
	{
		strFolderPath = strFilePath.Left(nLastIndex+1);
	} 
	else 
	{
		strFolderPath = _T("\\");
	}
	return strFolderPath; 
}


void CGameManBoxMan::LoadMap(int iMissionNum)
{
	if ( m_nMapType!=MAP_EXTERN )
	{
		LoadMapInRes( iMissionNum );
		return;
	}


    CString str;
    str.Format(_T("[%d]"), iMissionNum);

	CString strInfoPath = GetProgromRunFolder() + m_StrImagePath + _T("map.info");


	FILE *pFile = _tfopen( strInfoPath, _T("r") );
	if (pFile == NULL)
	{
        AfxMessageBox(_T("�����ͼ�ļ�ʧ��"));
        return;
	}

    TCHAR cTmp[M_TAB_WIDTH*2];
	
	CString strTmp;
    _fgetts( strTmp.GetBuffer( M_TAB_WIDTH*2 ), M_TAB_WIDTH*2, pFile );
	strTmp.ReleaseBuffer();
	strTmp.TrimLeft();
	strTmp.TrimRight();
    while ( strTmp != str )
    {
        _fgetts( strTmp.GetBuffer( M_TAB_WIDTH*2 ), M_TAB_WIDTH*2, pFile );
		strTmp.ReleaseBuffer();
		strTmp.TrimLeft();
		strTmp.TrimRight();
    }

    for (int i = 0; i < M_TAB_HEIGHT; i++)
        _fgetts(m_cMap[i],M_TAB_WIDTH*2, pFile);

    fclose(pFile);
}

int CGameManBoxMan::LoadMaxMissionNum(void)
{
	if ( m_nMapType!=MAP_EXTERN )
		return LoadMaxMissionNumInRes();

	int iMissionNum = 1;
    CString str;
    str.Format(_T("[%d]"), iMissionNum);
	
	CString strInfoPath = GetProgromRunFolder() + m_StrImagePath + _T("map.info");


	FILE *pFile = _tfopen( strInfoPath, _T("r") );
	if (pFile == NULL)
	{
        MessageBox(NULL,_T("�����ͼ�ļ�ʧ��"),NULL,MB_OK);
        return -1;
	}
	
    TCHAR cTmp[M_TAB_WIDTH*2];
	CString strTmp;
	while( !feof(pFile) )//not end of file
	{
		if( _fgetts( strTmp.GetBuffer( M_TAB_WIDTH*2 ), M_TAB_WIDTH*2, pFile ) )
		{
			strTmp.ReleaseBuffer();
			strTmp.TrimLeft();strTmp.TrimRight();
			
			if( strTmp == str )
			{
				str.Format(_T("[%d]"), ++iMissionNum);
			}
		}
		else
		{
			strTmp.ReleaseBuffer();
		}
	}
    fclose(pFile);

	return iMissionNum-1;
}

CPoint CGameManBoxMan::GetManPosition()
{
    CPoint manPosition(0, 0);
    for (int i = 0; i < M_TAB_HEIGHT; i++)
    {
        for (int j = 0; j < M_TAB_WIDTH; j++)
        {
            if (m_cMap[i][j]==MAP_MANWALL || m_cMap[i][j]==MAP_MANBALL)
            {
                manPosition.x = j;
                manPosition.y = i;
				return manPosition;
            }
        }
    }
    return manPosition;
}

void CGameManBoxMan::DrawGameArea(CDC* pDC)
{
    int i, j, x, y;

	//����������ͼ��Ϸ����
    for (i = 0; i < M_TAB_HEIGHT; i++)
    {
        for (j = 0; j < M_TAB_WIDTH; j++)
        {
            x = j * BLOCK_WIDTH;
            y = i * BLOCK_HEIGHT;

            switch (m_cMap[i][j])
            {
            case MAP_BACKGROUP://����
                DrawBackGroup(x, y, pDC);
                break;
            case MAP_WHITEWALL://ǽ
                DrawWhiteWall(x, y, pDC);
                break;
            case MAP_BLUEWALL://ͨ��
                DrawBlueWall(x, y, pDC);
                break;
            case MAP_BALL://Ŀ�ĵ�
                DrawBall(x, y, pDC);
                break;
            case MAP_YELLOWBOX://δ���źõ�����
                DrawYellowBox(x, y, pDC);
                break;
            case MAP_REDBOX://���źõ�����
                DrawRedBox(x, y, pDC);
                break;
            case MAP_MANWALL://����ͨ������
                DrawManWall(x, y, pDC);
                break;
            case MAP_MANBALL://����Ŀ�ĵ�����
                DrawManBall(x, y, pDC);
                break;
            }
        }
    }

	//��ȡ�ɵ��ı�����
    COLORREF crOldText = pDC->GetTextColor();
    COLORREF crOldBack = pDC->GetBkColor();
	//���ĵ�ǰ���ı�����
    pDC->SetTextColor(RGB(0, 0, 102));
	pDC->SetBkColor(RGB(0, 0, 0));
	//����ı�
    CString sTmp;
    sTmp.Format(_T("��ǰ���� : %d "), m_iMissionNum);
//    pDC->TextOut(50, 270, sTmp); // pDC->DrawText(50, 270, sTmp);
	//�ָ�ԭ���ı�����
    pDC->SetTextColor(crOldText);
    pDC->SetBkColor(crOldBack);
}

//
//  ���Ʊ���
//
void CGameManBoxMan::DrawBackGroup(int x, int y, CDC* pDC)
{
#ifdef _USE_GAME_BK_TRANSPARENT 
	CPoint ptSrc( m_ptOffset.x+x,m_ptOffset.y+y );
	pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCBack,ptSrc.x,ptSrc.y,SRCCOPY );

#else
	COLORREF clr = RGB(0, 0, 0);
	pDC->FillSolidRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, clr);

#endif  
}

//
//  ����ǽ
//
void CGameManBoxMan::DrawWhiteWall(int x, int y, CDC* pDC)
{
    COLORREF clr1 = RGB(255, 255, 255);
    COLORREF clr2 = RGB(48, 48, 48);
    COLORREF clr3 = RGB(192, 192, 192);

	//�ص�������3DЧ��,����3�㽥��
    pDC->FillSolidRect(x, y, 19, 19, clr1);
    pDC->FillSolidRect(x + 1, y + 1, 19, 19, clr2);
    pDC->FillSolidRect(x + 1, y + 1, 18, 18, clr3);

	//����ǽ�ķ�϶
    pDC->MoveTo(x, y + 10);
    pDC->LineTo(x + 20, y + 10);
    pDC->MoveTo(x + 10, y + 10);
    pDC->LineTo(x + 10, y + 20);
}

//
//  ����ͨ��
//
void CGameManBoxMan::DrawBlueWall(int x, int y, CDC* pDC)
{
    COLORREF clr = RGB(0, 0, 255);
    pDC->FillSolidRect(x, y, 20, 20, clr);
    pDC->MoveTo(x, y + 10);
    pDC->LineTo(x + 20, y + 10);
    pDC->MoveTo(x + 10, y + 10);
    pDC->LineTo(x + 10, y + 20);
}

//
//  ����Ŀ�ĵ�
//
void CGameManBoxMan::DrawBall(int x, int y, CDC* pDC)
{
    COLORREF clr = RGB(0, 0, 255);
    pDC->FillSolidRect(x, y, 20, 20, clr);
    pDC->MoveTo(x, y + 10);
    pDC->LineTo(x + 20, y + 10);
    pDC->MoveTo(x + 10, y + 10);
    pDC->LineTo(x + 10, y + 20);
    pDC->Ellipse(x, y, x + 20, y + 20);
    pDC->Ellipse(x + 5, y + 5, x + 15, y + 15);
}

//
//  ����δ�źõ�����
//
void CGameManBoxMan::DrawYellowBox(int x, int y, CDC* pDC)
{
    COLORREF clr = RGB(255, 255, 0);
    pDC->FillSolidRect(x, y, 20, 20, clr);
    COLORREF clr2 = RGB(255, 192, 0);
    pDC->FillSolidRect(x + 2, y + 2, 16, 16, clr2);
    COLORREF clr3 = RGB(0, 0, 0);
    pDC->SetPixel(x + 3, y + 3, clr3);
    pDC->SetPixel(x + 17, y + 3, clr3);
    pDC->SetPixel(x + 3, y + 17, clr3);
    pDC->SetPixel(x + 17, y + 17, clr3);
}

//
//  ���ư��źõ�����
//
void CGameManBoxMan::DrawRedBox(int x, int y, CDC* pDC)
{
    COLORREF clr = RGB(255, 255, 0);
    pDC->FillSolidRect(x, y, 20, 20, clr);
    COLORREF clr2 = RGB(255, 0, 0);
    pDC->FillSolidRect(x + 2, y + 2, 16, 16, clr2);
    COLORREF clr3 = RGB(0, 0, 0);
    pDC->SetPixel(x + 3, y + 3, clr3);
    pDC->SetPixel(x + 17, y + 3, clr3);
    pDC->SetPixel(x + 3, y + 17, clr3);
    pDC->SetPixel(x + 17, y + 17, clr3);
}

//
//  ��������ͨ��
//
void CGameManBoxMan::DrawManWall(int x, int y, CDC* pDC)
{
	if( NULL !=m_pDCMan )
	{
		if ( CLR_INVALID == m_clManTrans )
		{
			pDC->BitBlt( x,y,20,20,m_pDCMan,0,0,SRCCOPY );
		}
		else
		{
			COLORREF clr = RGB(0, 0, 255);                   //��ɫǽ
			pDC->FillSolidRect(x, y, 20, 20, clr);
			pDC->MoveTo(x, y + 10);
			pDC->LineTo(x + 20, y + 10);
			pDC->MoveTo(x + 10, y + 10);
			pDC->LineTo(x + 10, y + 20);

			TransparentBlt(pDC->m_hDC,x,y,20,20,m_pDCMan->m_hDC,0,0,m_szMan.cx,m_szMan.cy,m_clManTrans );
		}
		return;
	}

	COLORREF clr = RGB(0, 0, 255);                   //��ɫǽ
    pDC->FillSolidRect(x, y, 20, 20, clr);
    pDC->MoveTo(x, y + 10);
    pDC->LineTo(x + 20, y + 10);
    pDC->MoveTo(x + 10, y + 10);
    pDC->LineTo(x + 10, y + 20);

	//�˵Ļ���
    pDC->Ellipse(x + 6, y + 2, x + 14, y + 10);      //��ͷ
    pDC->MoveTo(x + 2, y + 11);                      //����
    pDC->LineTo(x + 18, y + 11);
    pDC->MoveTo(x + 10, y + 10);                     //������
    pDC->LineTo(x + 10, y + 12);

    pDC->MoveTo(x + 2, y + 20);                      //�˽�
    pDC->LineTo(x + 10, y + 12);
    pDC->LineTo(x + 18, y +20);
}

//
//  ��������Ŀ�ĵ�
//
void CGameManBoxMan::DrawManBall(int x, int y, CDC* pDC)
{
    COLORREF clr = RGB(0, 0, 255);                   //��
    pDC->FillSolidRect(x, y, 20, 20, clr);
    pDC->MoveTo(x, y + 10);
    pDC->LineTo(x + 20, y + 10);
    pDC->MoveTo(x + 10, y + 10);
    pDC->LineTo(x + 10, y + 20);
    pDC->Ellipse(x, y, x + 20, y + 20);
    pDC->Ellipse(x + 5, y + 5, x + 15, y + 15);

//#ifdef	_USE_GAME_BK_TRANSPARENT	
	if( NULL !=m_pDCMan )
	{	
		if ( CLR_INVALID != m_clManTrans )
		{
			TransparentBlt(pDC->m_hDC,x,y,20,20,m_pDCMan->m_hDC,0,0,m_szMan.cx,m_szMan.cy,m_clManTrans );
		}
		else
		{
			m_bfSelect.BlendOp = AC_SRC_OVER;
			m_bfSelect.BlendFlags = 0;
			m_bfSelect.AlphaFormat = 0;
			m_bfSelect.SourceConstantAlpha = 220;				//͸����
//			BOOL result = ::AlphaBlend( pDC->m_hDC, x, y,20,20,m_pDCMan->m_hDC,0,0,m_szMan.cx,m_szMan.cy,m_bfSelect);
		}
		return;
	}
//#endif


	//�˵Ļ���
    pDC->Ellipse(x + 6, y + 2, x + 14, y + 10);      //��ͷ

    pDC->MoveTo(x + 2, y + 11);                      //����
    pDC->LineTo(x + 18, y + 11);

    pDC->MoveTo(x + 10, y + 10);                     //������
    pDC->LineTo(x + 10, y + 12);

    pDC->MoveTo(x + 2, y + 20);                      //�˽�
    pDC->LineTo(x + 10, y + 12);
    pDC->LineTo(x + 18, y +20);
}

void CGameManBoxMan::KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	DispatchMsg(nChar);

    if (IsFinish())
    {
		m_soundState = SOUND_STATE_VICTORY;
        m_iMissionNum = m_iMissionNum +1;
        if (m_iMissionNum > MAX_MISSION_NUM)
            m_iMissionNum = 1;
        LoadMap(m_iMissionNum);
        m_ptManPosition = GetManPosition();
	
		NewGameInit();

    }

	BoxManPlaySound();
}

void CGameManBoxMan::DispatchMsg(UINT nChar)
{
    int x1, y1, x2, y2, x3, y3;

	//��ȡ���˵�ǰλ��
    x1 = m_ptManPosition.x;
    y1 = m_ptManPosition.y;
	m_direction = nChar;

	//����������Ϣ
	switch (nChar)
    {
	//����
    case VK_UP:
        x2 = x1;
        y2 = y1 - 1;
        x3 = x1;
        y3 = y1 - 2;
		//������λ���������жϲ�����ͼ����
        UpdateMap(x1, y1, x2, y2, x3, y3);
        break;
	//����
    case VK_DOWN:
        x2 = x1;
        y2 = y1 + 1;
        x3 = x1;
        y3 = y1 + 2;
        UpdateMap(x1, y1, x2, y2, x3, y3);
        break;
	//����
    case VK_LEFT:
        x2 = x1 - 1;
        y2 = y1;
        x3 = x1 - 2;
        y3 = y1;
        UpdateMap(x1, y1, x2, y2, x3, y3);
        break;
	//����
    case VK_RIGHT:
        x2 = x1 + 1;
        y2 = y1;
        x3 = x1 + 2;
        y3 = y1;
        UpdateMap(x1, y1, x2, y2, x3, y3);
        break;

  //ѡ�ع��ܼ�
    case VK_BOXMAN_RESTART:  //R  �����浱ǰ��
    case 114: //r  
        LoadMap(m_iMissionNum);
        m_ptManPosition = GetManPosition();
        break;
    case VK_BOXMAN_NEXT_BARRIER:  //F2  ������һ��
        m_iMissionNum = m_iMissionNum + 1;
        if (m_iMissionNum > MAX_MISSION_NUM)
            m_iMissionNum = 1;
        LoadMap(m_iMissionNum);
        m_ptManPosition = GetManPosition();
        break;
    case VK_BOXMAN_FORER_BARRIER:  //F1  ����ǰһ��
        m_iMissionNum = m_iMissionNum - 1;
        if (m_iMissionNum < 1)
            m_iMissionNum = MAX_MISSION_NUM;
        LoadMap(m_iMissionNum);
        m_ptManPosition = GetManPosition();
    }
}

void CGameManBoxMan::NewGameInit()
{
	while( m_stackRedoMove.size()>0 )
		m_stackRedoMove.pop();
}
/*******************************************************************
* Function Name �� 
* Remark        �� 
* Update Record �� 
*               �� 
* Params        �� 
* Return Value  �� 
* Author        �� Zhxj
* Emaile        �� shenzhenit@163.com
* Date Created  �� 2009��12��3�� ������
*******************************************************************/
BOOL CGameManBoxMan::UndoMove()
{
	if ( m_stackRedoMove.size() ==0 )
		return FALSE;

	MovStep mstep = m_stackRedoMove.top( );
	
	CPoint ManToPt = mstep.m_manFromPt,BoxToPt = mstep.m_BoxPtFrom;
	//����������Ϣ
	switch ( mstep.m_MovDirection )
	{
		//����
	case VK_UP:
		ManToPt.Offset(0,-1);
		BoxToPt.Offset(0,-1);
		break;
		//����
	case VK_DOWN:
		ManToPt.Offset(0,1);
		BoxToPt.Offset(0,1);
		break;
		//����
	case VK_LEFT:
		ManToPt.Offset(-1,0);
		BoxToPt.Offset(-1,0);
		break;
		//����
	case VK_RIGHT:
		ManToPt.Offset(1,0);
		BoxToPt.Offset(1,0);	
		break;
    case MY_VK_JUMP:
        ManToPt.Offset(GetManPosition().x - mstep.m_manFromPt.x, GetManPosition().y - mstep.m_manFromPt.y);
        OutputDebugString(TEXT("UNDO_MY_VK_JUMP"));
        break;
	}

	//Test Code error , couldn't occur,
	if ( ManToPt.x<0 || ManToPt.y<0 || ManToPt.x>=BLOCK_WIDTH || ManToPt.y>=BLOCK_HEIGHT )
	{
		AfxMessageBox( TEXT("Boxman Undo Error!!!") );
		return  FALSE;
	}

	m_cMap[ ManToPt.y ][ManToPt.x] = mstep.m_ManToType;
	m_cMap[ mstep.m_manFromPt.y ][ mstep.m_manFromPt.x ] = mstep.m_ManFromType;
	if ( mstep.m_BoxPtFrom.x!=-1 && mstep.m_BoxPtFrom.y!=-1 )
	{
		m_cMap[ BoxToPt.y ][ BoxToPt.x ] = mstep.m_BoxToType;
	}
	m_ptManPosition = mstep.m_manFromPt;

	m_stackRedoMove.pop();

	return TRUE;
}

void CGameManBoxMan::UpdateMap(int x1, int y1, int x2, int y2, int x3, int y3)
{
	MovStep mstep;
	BOOL	bCouldMov = FALSE;
	//���浱ǰ�ƶ�λ�ã����㷵��
	switch (m_cMap[y2][x2])
	{	
	case MAP_BLUEWALL:             //ͨ������������   
	case MAP_BALL:				  //Ŀ�ĵأ���������
		mstep.m_manFromPt.SetPoint( x1,y1 );	
		mstep.m_MovDirection = m_direction;
		mstep.m_ManFromType = m_cMap[y1][x1];
		mstep.m_ManToType = m_cMap[y2][x2];
		
		bCouldMov = TRUE;
		//if (   m_cMap[y2][x2] ==  MAP_BLUEWALL )mstep.m_ManToType = MAP_MANWALL;
		//else mstep.m_ManToType = MAP_MANBALL;
		break;

	case MAP_YELLOWBOX:          //����
		if (m_cMap[y3][x3] == MAP_BALL)   //Ŀ�ĵؿ�������
		{
			mstep.m_manFromPt.SetPoint( x1,y1 );	
			mstep.m_MovDirection = m_direction;
			mstep.m_ManFromType = m_cMap[y1][x1];
			mstep.m_ManToType = m_cMap[y2][x2];;

			mstep.m_BoxPtFrom.SetPoint( x2,y2);			
			mstep.m_BoxFromType = m_cMap[y2][x2];
			mstep.m_BoxToType = m_cMap[y3][x3];

			bCouldMov = TRUE;
		}
		else if (m_cMap[y3][x3] == MAP_BLUEWALL) //ͨ������������
		{
			mstep.m_manFromPt.SetPoint( x1,y1 );	
			mstep.m_MovDirection = m_direction;
			mstep.m_ManFromType = m_cMap[y1][x1];
			mstep.m_ManToType = m_cMap[y2][x2];;

			mstep.m_BoxPtFrom.SetPoint( x2,y2);			
			mstep.m_BoxFromType = m_cMap[y2][x2];
			mstep.m_BoxToType = m_cMap[y3][x3];

			bCouldMov = TRUE;
		}
		break;

	case MAP_REDBOX:             //���źõ�����
		if (m_cMap[y3][x3] == MAP_BALL)   //Ŀ�ĵؿ�������
		{
			mstep.m_manFromPt.SetPoint( x1,y1 );	
			mstep.m_MovDirection = m_direction;
			mstep.m_ManFromType = m_cMap[y1][x1];
			mstep.m_ManToType = m_cMap[y2][x2];;

			mstep.m_BoxPtFrom.SetPoint( x2,y2);			
			mstep.m_BoxFromType = m_cMap[y2][x2];
			mstep.m_BoxToType = m_cMap[y3][x3];
			bCouldMov = TRUE;
		}
		else if (m_cMap[y3][x3] == MAP_BLUEWALL)  //ͨ������������
		{
			mstep.m_manFromPt.SetPoint( x1,y1 );	
			mstep.m_MovDirection = m_direction;
			mstep.m_ManFromType = m_cMap[y1][x1];
			mstep.m_ManToType = m_cMap[y2][x2];;

			mstep.m_BoxPtFrom.SetPoint( x2,y2);			
			mstep.m_BoxFromType = m_cMap[y2][x2];
			mstep.m_BoxToType = m_cMap[y3][x3];
			bCouldMov = TRUE;
		}        
		break;
	}
	if ( bCouldMov )
	{
		m_stackRedoMove.push( mstep );
	}
	


    switch (m_cMap[y2][x2])
    {
    case MAP_BACKGROUP:           //��ͼ��ƴ���,��Ӧ�ó���
        AfxMessageBox(_T("wrong map"));
        break;
    case MAP_WHITEWALL:          //����ǽ�������κ�����
        
        break;
    case MAP_BLUEWALL:           //ͨ������������
        m_cMap[y2][x2] = MAP_MANWALL;
        if (m_cMap[y1][x1] == MAP_MANWALL)
            m_cMap[y1][x1] = MAP_BLUEWALL;
        else if (m_cMap[y1][x1] == MAP_MANBALL)
            m_cMap[y1][x1] = MAP_BALL;
        m_ptManPosition.x = x2;
        m_ptManPosition.y = y2;
		m_soundState = SOUND_STATE_MOVE;
        break;

    case MAP_BALL:               //Ŀ�ĵأ���������
        m_cMap[y2][x2] = MAP_MANBALL;
        if (m_cMap[y1][x1] == MAP_MANWALL)
            m_cMap[y1][x1] = MAP_BLUEWALL;
        else if (m_cMap[y1][x1] == MAP_MANBALL)
            m_cMap[y1][x1] = MAP_BALL;        
        m_ptManPosition.x = x2;
        m_ptManPosition.y = y2;
		m_soundState = SOUND_STATE_MOVE;
        break;

    case MAP_YELLOWBOX:          //����
        if (m_cMap[y3][x3] == MAP_BALL)   //Ŀ�ĵؿ�������
        {
            m_cMap[y3][x3] = MAP_REDBOX;
            m_cMap[y2][x2] = MAP_MANWALL;
            if (m_cMap[y1][x1] == MAP_MANWALL)
                m_cMap[y1][x1] = MAP_BLUEWALL;
            else if (m_cMap[y1][x1] == MAP_MANBALL)
                m_cMap[y1][x1] = MAP_BALL;
            m_ptManPosition.x = x2;
            m_ptManPosition.y = y2;
		m_soundState = SOUND_STATE_PUSH;
        }
        else if (m_cMap[y3][x3] == MAP_BLUEWALL) //ͨ������������
        {
            m_cMap[y3][x3] = MAP_YELLOWBOX;
            m_cMap[y2][x2] = MAP_MANWALL;
            if (m_cMap[y1][x1] == MAP_MANWALL)
                m_cMap[y1][x1] = MAP_BLUEWALL;
            else if (m_cMap[y1][x1] == MAP_MANBALL)
                m_cMap[y1][x1] = MAP_BALL;
            m_ptManPosition.x = x2;
            m_ptManPosition.y = y2;
		m_soundState = SOUND_STATE_PUSH;
        }
        break;

    case MAP_REDBOX:             //���źõ�����
        if (m_cMap[y3][x3] == MAP_BALL)   //Ŀ�ĵؿ�������
        {
            m_cMap[y3][x3] = MAP_REDBOX;
            m_cMap[y2][x2] = MAP_MANBALL;
            if (m_cMap[y1][x1] == MAP_MANWALL)
                m_cMap[y1][x1] = MAP_BLUEWALL;
            else if (m_cMap[y1][x1] == MAP_MANBALL)
                m_cMap[y1][x1] = MAP_BALL;
            m_ptManPosition.x = x2;
            m_ptManPosition.y = y2;
			m_soundState = SOUND_STATE_PUSH;
        }
        else if (m_cMap[y3][x3] == MAP_BLUEWALL)  //ͨ������������
        {
            m_cMap[y3][x3] = MAP_YELLOWBOX;
            m_cMap[y2][x2] = MAP_MANBALL;
            if (m_cMap[y1][x1] == MAP_MANWALL)
                m_cMap[y1][x1] = MAP_BLUEWALL;
            else if (m_cMap[y1][x1] == MAP_MANBALL)
                m_cMap[y1][x1] = MAP_BALL;
            m_ptManPosition.x = x2;
            m_ptManPosition.y = y2;
			m_soundState = SOUND_STATE_PUSH;
        }        
        break;

    case MAP_MANWALL:            //��ͼ��ƴ���,��Ӧ�ó���
        AfxMessageBox(_T("wrong map"));
        break;

    case MAP_MANBALL:            //��ͼ��ƴ���,��Ӧ�ó���
        AfxMessageBox(_T("wrong map"));
        break;
    }
}

bool CGameManBoxMan::IsFinish()
{
    bool bFinish = true;
    for (int i = 0; i < M_TAB_HEIGHT; i++)
    {
        for (int j = 0; j < M_TAB_WIDTH; j++)
        {
            if (m_cMap[i][j] == MAP_BALL || m_cMap[i][j] == MAP_MANBALL)
                bFinish = false;
        }
    }
    return bFinish;
}

BOOL CGameManBoxMan::ChangeMissionNum(int iNum)
{
	if((iNum>MAX_MISSION_NUM)||(iNum<1))
	{
		return FALSE;
	}
	else
	{
        m_iMissionNum = iNum;
        LoadMap(m_iMissionNum);
        m_ptManPosition = GetManPosition();
		return TRUE;
	}
}

int  CGameManBoxMan::GetNowMissionNum(void)
{
	return m_iMissionNum;
}

void CGameManBoxMan::BoxManPlaySound(void)
{
	return;
	if(m_bSound)
	{
		TCHAR strFileName[30];
		
		switch (m_soundState)
		{
		case SOUND_STATE_START :
			wcscpy(strFileName,_T("start.wav"));
			break;
			
		case SOUND_STATE_MOVE :
			wcscpy(strFileName,_T("move.wav"));
			break;
			
		case SOUND_STATE_PUSH :
			wcscpy(strFileName,_T("push.wav"));
			break;
			
		case SOUND_STATE_VICTORY :
			wcscpy(strFileName,_T("victory.wav"));
			break;
			
		default:
			return;
		}

//		PlaySound(strFileName, NULL, SND_ASYNC | SND_FILENAME);	
	}
}

void CGameManBoxMan::IsSound(BOOL bUse)
{
	m_bSound = bUse;
}

void CGameManBoxMan::ReturnCurManPos( CPoint *pPos )
{
	*pPos = m_ptManPosition;
}

TCHAR * CGameManBoxMan::GetMap()
{
   TCHAR strtmp[MAX_PATH] ={0};
#ifdef _DEBUG
    
    _stprintf(strtmp,TEXT("%s\r\n"),m_cMap);
    OutputDebugString(strtmp);
#endif
    return (TCHAR *)m_cMap;
}

//�������ֻ�е�����Man���ƶ������ƶ�Box�����Բ���Ҫ����BOX��ѡ��
void CGameManBoxMan::ManJump( int xFrom,int yFrom,int x2,int y2 )
{
    MovStep mstep;
    mstep.m_manFromPt.SetPoint( xFrom,yFrom );	
    mstep.m_ManFromType = m_cMap[yFrom][xFrom];
    mstep.m_MovDirection = MY_VK_JUMP;
    mstep.m_ManToType = m_cMap[y2][x2];;


    //mstep.m_BoxPtFrom.SetPoint( x2,y2 );			
    //mstep.m_BoxFromType = 0;
    //mstep.m_BoxToType = 0;


    //�޸ĵ�ͼ��Ϣ
    switch (m_cMap[y2][x2])
    {
    case MAP_BLUEWALL:           //ͨ������������
        m_cMap[y2][x2] = MAP_MANWALL;
        if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
            m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
        else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
            m_cMap[yFrom][xFrom] = MAP_BALL;
        m_ptManPosition.x = x2;
        m_ptManPosition.y = y2;
        break;

    case MAP_BALL:               //Ŀ�ĵأ���������
        m_cMap[y2][x2] = MAP_MANBALL;
        if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
            m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
        else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
            m_cMap[yFrom][xFrom] = MAP_BALL;        
        m_ptManPosition.x = x2;
        m_ptManPosition.y = y2;
        break;
    default:           //��ͼ��ƴ���,��Ӧ�ó���
        AfxMessageBox(_T("wrong !!!"));
        break;

    }
    m_stackRedoMove.push( mstep );
}

BOOL	CGameManBoxMan::IsPositionValid( int xEnd,int yEnd )
{
    if ( xEnd<0 || xEnd<0 || xEnd>=M_TAB_WIDTH || xEnd>=M_TAB_HEIGHT )
        return FALSE;
    if ( ASSET_CANNOTMOV( m_cMap[yEnd][xEnd] ) )
        return FALSE;
    return TRUE;
}

BOOL	CGameManBoxMan::FindNearPos( int xStart,int yStart,int xEnd,int yEnd,TCHAR &KeyDirect )
{
    BOOL br = FALSE;
    int xSub,ySub;
    xSub = abs(xStart-xEnd);
    ySub = abs(yStart-yEnd);
    if ( ySub>1 || xSub>1 )
        return FALSE;
    if ( ySub==1 && xSub==1 )
        return FALSE;

    if ( xStart+1 == xEnd )
    {
        br = TRUE;
        KeyDirect = VK_RIGHT;
    }	
    else if ( xStart-1 == xEnd )
    {
        br = TRUE;
        KeyDirect = VK_LEFT;
    }	
    if ( yStart+1 == yEnd )
    {
        br = TRUE;
        KeyDirect = VK_DOWN;
    }	
    if ( yStart-1 == yEnd )
    {
        br = TRUE;
        KeyDirect = VK_UP ;
    }	
    return br;

}

//dir ��ʾ��������
bool CGameManBoxMan::search( int xStart,int yStart,int xEnd,int yEnd )
{
    SearchStep step = { xStart,yStart,TRUE,TRUE,TRUE,TRUE };
    stack<SearchStep> SearchQueue;
    SearchQueue.push( step );

    //BOOL bRight; BOOL bLeft; BOOL bUp; BOOL	bDown;

    memset( m_bSerChMap,0,sizeof(m_bSerChMap) );

    SearchStep SCurStep;
    int xCur,yCur;
    while( !SearchQueue.empty() )
    {
        SCurStep = SearchQueue.top( );
        SearchQueue.pop();

       // RETAILMSG( 1,(L"UnStep=%d,  (%d,%d)\r\n",SearchQueue.size(),SCurStep.x,SCurStep.y ) );

        if ( SCurStep.bRight )
        {
            xCur = SCurStep.x+1;
            yCur = SCurStep.y;
            m_bSerChMap[SCurStep.y][SCurStep.x] = TRUE;

            if ( xCur==xEnd && yCur==yEnd )
            {
                while(!SearchQueue.empty()) 
                    SearchQueue.pop();
                return TRUE;
            }
            if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
            {
                if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] )
                {
                    SearchStep step = { xCur,yCur,TRUE,FALSE,TRUE,TRUE };	
                    SearchQueue.push( step );
                }
            }
        }

        if ( SCurStep.bLeft )
        {
            xCur = SCurStep.x-1;
            yCur = SCurStep.y;
            if ( xCur==xEnd  && yCur==yEnd )
            {
                while(!SearchQueue.empty()) 
                    SearchQueue.pop();
                return TRUE;
            }

            if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
            {
                if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
                    SearchStep step = { xCur,yCur,FALSE,TRUE,TRUE,TRUE };	
                    SearchQueue.push( step );
                }
            }
        }

        if ( SCurStep.bUp )
        {
            xCur = SCurStep.x;
            yCur = SCurStep.y-1;
            if ( xCur==xEnd  && yCur==yEnd )
            {
                while(!SearchQueue.empty()) 
                    SearchQueue.pop();
                return TRUE;
            }

            if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
            {
                if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
                    SearchStep step = { xCur,yCur,TRUE,TRUE,TRUE,FALSE };	
                    SearchQueue.push( step );
                }
            }
        }

        if ( SCurStep.bDown )
        {
            xCur = SCurStep.x;
            yCur = SCurStep.y+1;
            if ( xCur==xEnd  && yCur==yEnd )
            {
                while(!SearchQueue.empty()) 
                    SearchQueue.pop();
                return TRUE;
            }


            if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
            {
                if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
                    SearchStep step = { xCur,yCur,TRUE,TRUE,FALSE,TRUE };	
                    SearchQueue.push( step );
                }
            }
        }
    }

    while(!SearchQueue.empty()) 
        SearchQueue.pop();

    return FALSE;
}