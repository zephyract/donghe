#include "stdafx.h"
#include "MusicPlayerDlg.h"
#include "wceuipagelayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "msdkFileManager.h"
#include "sysUtil.h"
#include "config.h"
#include "tzUtil.h"
#include "WceUi32Bitmap.h"
#include "WceUiLoadString.h"
#include "interface.h"
#include "OverlayDlg.h"

#include <imaging.h> 

CMusicPlayerDlg::CMusicPlayerDlg(void)
{
	CMsdkRender::GetInstance()->RegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

CMusicPlayerDlg::~CMusicPlayerDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

void CMusicPlayerDlg::OnInitDialog()
{
	RegisterMsg(NAVI_MSG_LAN_CHANGE);

	__super::OnInitDialog();

	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
// 	pLayer->SetExtraScroll(30);
// 	pLayer->SetScrollRange(0, 800);

	// lyric
	m_pLayerLyric = (CLyricEditBox*)GetChildByName(L"lyric");
	ASSERT(m_pLayerLyric);
	m_pLayerLyric->SetExtraScroll(130);

	// spectrum
	m_pLayerSpectrum = (CSpectrumLayer*)GetChildByName(L"spectrum");
	m_pLayerSpectrum->ShowLayer(FALSE);
	SetTimer(TIMER_ID_SPECTRUM, 150);
// 
// 	CWceUiLayer* player = GetChildByName(L"artist_image");
// 	if (player)
// 	{
// 		player->SetZCoord(Z_AXE, Z_AXE, Z_AXE-100, Z_AXE-100);
// 	}

	// 第一次进来时,文件可能已经开始播放了,收不到EVT_MSDK_GRAPH_BUILT,这里主动调用一次
	UpdateMediaInfo();
	SetSpectrumInfo();

	ShowPlayMode();
}

void CMusicPlayerDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SPECTRUM)
	{
		m_pLayerSpectrum->InvalidateRect(NULL);
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CMusicPlayerDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		ShowFileTip();
		ShowTitle();
	}
}

#include "msdkFileManager.h"
void CMusicPlayerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMMFileListDlg_ID, ui_msdk_get_cursrc());
	}
	else if (pButton->IsEqualName(L"next"))
	{
		PlayNext();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		PlayPrev();
	}
	else if (pButton->IsEqualName(L"play"))
	{
		CMsdkRender::GetInstance()->FPlay();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CMsdkRender::GetInstance()->FPause();
	}
	else if (pButton->IsEqualName(L"random"))
	{
		OnRandom();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		OnRepeat();
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CMusicPlayerDlg::OnRandom()
{
	WORD random = CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_MUSIC);
	random++;
	random %= 2;	// 取值0, 1,
	CMsdkFileManager::GetInstance()->SetPlayRandomMode(random, MSDK_FILE_MUSIC);
	ShowPlayMode();


	// tip
	CWceUiLoadString strRandom;
	strRandom.SetTextResID((random == MSDK_RANDOM_ON) ? L"MIS_PLAY_RANDOM_ON" : L"MIS_PLAY_RANDOM_OFF");
	COverlayDlg::GetInstance()->MessageTip_ShowUI(strRandom.GetString(), FALSE);
}

void CMusicPlayerDlg::OnRepeat()
{
	WORD repeat = CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_MUSIC);
	repeat++;
	repeat %= 4;	// 取值0, 1, 2, 3
	CMsdkFileManager::GetInstance()->SetPlayRepeateMode(repeat, MSDK_FILE_MUSIC);
	ShowPlayMode();

	// tip
	CWceUiLoadString strRepeat;
	switch (repeat)
	{
	case MSDK_REPEATE_ALL:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_ALL");
		break;
	case MSDK_REPEATE_FOLDER:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_FOLDER");
		break;
	case MSDK_REPEATE_ONE:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_SINGLE");
		break;
	case MSDK_REPEATE_OFF:
	default:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_OFF");
		break;
	}
	COverlayDlg::GetInstance()->MessageTip_ShowUI(strRepeat.GetString(), FALSE);
}


void CMusicPlayerDlg::PlayNext()
{
	int index = CMsdkFileManager::GetInstance()->PlayFileNext(MSDK_FILE_MUSIC);
	if (index<0)	//没有成功播放下一曲
	{
		index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	}
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CMusicPlayerDlg::PlayPrev()
{
	int index = CMsdkFileManager::GetInstance()->PlayFilePrev(MSDK_FILE_MUSIC);
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CMusicPlayerDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CMusicPlayerDlg* pthis = (CMusicPlayerDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

void CMusicPlayerDlg::UpdateMediaInfo()
{
	int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	CString lrcFilepath = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);

	if (m_strPlayFilePath == lrcFilepath)	// 重复调用时，不重复更新数据
	{
		return;
	}
	m_strPlayFilePath = lrcFilepath;

	ShowMp3Tag();
	ShowMp3Image();

	int pos = lrcFilepath.ReverseFind(L'.');
	lrcFilepath = lrcFilepath.Mid(0, pos+1);
	lrcFilepath += L"lrc";
	m_lyric.Initialize(lrcFilepath);
	AddLyricInfo();

	ShowFileTip();
	ShowTitle();

	// 进度条也置为初始位置
	m_pProgressSlider->SetPos(0);
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
	ShowMediaTimeInfo(0, outDuration);
}

void CMusicPlayerDlg::OnGraphBuilt()
{
	UpdateMediaInfo();
}

void CMusicPlayerDlg::ShowLRC(UINT curDuration)
{
	int nIndex = m_lyric.GetLyricIndex(curDuration);
	int nMin = -1;
	int nMax = -1;

	if (nIndex < 0)
		return;

	// 根据该行LRC及下行的LRC在editbox中的位置，确定该行LRC在editbox一共占了几行
	nMin = m_pLayerLyric->GetLineIndex(nIndex);
	nMax = m_pLayerLyric->GetLineIndex(nIndex+1);
	if (nMax == -1)		// error index
		nMax = nMin;
	else
		nMax--;

	m_pLayerLyric->SetHighLightLines(nMin, nMax);
	if (nMin != -1)
	{
		int pos = m_pLayerLyric->GetScrollPos();
		int cur_pos = (nMin-m_pLayerLyric->LYRIC_EMPTY_LINES)*m_pLayerLyric->GetRowHeight();

		m_pLayerLyric->SetScrollFrame(10);
		m_pLayerLyric->AnimateScrollWindow(cur_pos - pos);
	}
}

void CMusicPlayerDlg::AddLyricInfo()
{
	CPtrArray *pLyricArray = m_lyric.GetLyricData();
	ASSERT(pLyricArray);
	CString strData;
	for (int i=0; i<m_pLayerLyric->LYRIC_EMPTY_LINES; i++)
	{
		strData += L"\n";
	}

	if (pLyricArray->GetCount() <= 0)
	{
		CWceUiLoadString str;
		str.SetTextResID(L"MUSIC_NO_LYRIC");
		strData += str.GetString();
	}
	else
	{
		for (int i=0; i<pLyricArray->GetCount(); i++)
		{
			LYRIC* plyric = (LYRIC*)pLyricArray->GetAt(i);
			strData += plyric->szLyric;
			strData += L"\n";
		}
	}


	m_pLayerLyric->SetData(strData);
	// adjust the scroll range, editbox's rule does not fit into this situation.
	// for edit box, assure the cursor in the last line is visible.
	int minPos = 0;
	int maxPos = 0;
	CRect rc;
	m_pLayerLyric->GetScrollRange(&minPos, &maxPos);
	m_pLayerLyric->GetClientRect(rc);
	maxPos -= rc.Height()/2;
	maxPos = max(minPos, maxPos);
	m_pLayerLyric->SetScrollRange(minPos, maxPos);

	m_pLayerLyric->ResetLineIndex();
	m_pLayerLyric->SetScrollPos(0);
	m_pLayerLyric->SetHighLightLines(0,0);
	m_pLayerLyric->InvalidateRect();
}


void CMusicPlayerDlg::ShowMp3Tag()
{
	CWceUiLayer *pLayerSong = GetChildByName(L"song_text");
	CWceUiLayer *pLayerAlbum = GetChildByName(L"album_text");
	CWceUiLayer *pLayerArtist = GetChildByName(L"artist_text");



	UINT nCodePage = 936; //ChineseGB
	int lan_id = sysutil::nss_get_instance()->ui_lan_id;
	LANGUAGE_TABLE_INFO* info = config::get_language_info(lan_id);
	if (info)
	{
		nCodePage = info->code_page;
	}

	int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);

	// display the MP3 file tag information
	tzutil::MP3_TAG tag;
	if (CMsdkRender::GetInstance()->FIsWmaPlaying())
	{
		tzutil::GetWMATag(file_path, &tag);
	}
	else
	{
		tzutil::GetMP3Tag(file_path, &tag, nCodePage);
	}
	if (tag._title[0] != L'\0')
	{
		pLayerSong->SetText(tag._title);
	}
	else
	{
		pLayerSong->SetTextResID(L"MUSIC_UNKNOWN_TITLE");
	}

	if (tag._album[0] != L'\0')
	{
		pLayerAlbum->SetText(tag._album);		
	}
	else
	{
		pLayerAlbum->SetTextResID(L"MUSIC_UNKNOWN_ALBUM");
	}

	if (tag._artist[0] != L'\0')
	{
		pLayerArtist->SetText(tag._artist);
	}
	else
	{
		pLayerArtist->SetTextResID(L"MUSIC_UNKNOWN_ARTIST");
	}

// 	_tcscpy(m_dvp_play_info._title,tag._title);
// 	_tcscpy(m_dvp_play_info._album,tag._album);
// 	_tcscpy(m_dvp_play_info._artist,tag._artist);	
//	MediaInfo2Shell(m_dvp_play_info);
}


void CMusicPlayerDlg::ShowMp3Image()
{
	LPCTSTR artist_image_resid = NULL;
	if (WceUiGetBitsPixel() == 16)
	{
		artist_image_resid = L"music\\artist_image.565a.conf";
	}
	else
	{
		ASSERT(WceUiGetBitsPixel() == 32);
		artist_image_resid = L"music\\artist_image.32a.conf";
	}

	// get mp3 image data
	BOOL bRet = FALSE;
	CWceUiLayer *pLayerImage = GetChildByName(L"artist_image");
	ASSERT(pLayerImage);

	int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);

	BYTE *pImageBuffer = NULL;
	int nImageSize;
	if (!tzutil::GetMp3Image(file_path, &pImageBuffer, &nImageSize))
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromRes(artist_image_resid);
		pLayerImage->InvalidateRect(NULL);
		return;
	}

	// create CWceUiGenericBitmap
	CRect rc;
	pLayerImage->GetClientRect(rc);
	CWceUiGenericBitmap *pbmp = new CWceUi32Bitmap();
	pbmp->CreateDIB(rc.Width(), rc.Height());
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	LPBYTE pbb;
	HBITMAP oldbmp = (HBITMAP)::SelectObject(dc.GetSafeHdc(), pbmp->GetHBitmap(pbb));

	IImage *pImage = NULL;
	IImagingFactory *pImageFactory = NULL;

	// Create IPicture and render 
	LPVOID pvData = NULL;
	IStream *pStream = NULL; 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nImageSize); 
	if (hGlobal == NULL)
	{
		goto EXIT;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)
	{
		goto EXIT;
	}

	memcpy(pvData, pImageBuffer, nImageSize);
	GlobalUnlock(hGlobal);


	if (!SUCCEEDED(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)))
	{
		goto EXIT;
	}

	if( !SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (LPVOID*)&pImageFactory)))
	{
		goto EXIT;
	}

	if (!SUCCEEDED(pImageFactory->CreateImageFromStream(pStream, &pImage)))
	{
		goto EXIT;
	}

	if (!SUCCEEDED(pImage->Draw(dc.GetSafeHdc(), rc, NULL)))
	{
		goto EXIT;
	}
	::SelectObject(dc.GetSafeHdc(), oldbmp);

	bRet = TRUE;

EXIT:
	if (bRet)
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromApp(pbmp);
		pLayerImage->InvalidateRect(NULL);
	}
	else
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromRes(artist_image_resid);
		pLayerImage->InvalidateRect(NULL);
	}

	if (pbmp)
	{
		dc.SelectObject(oldbmp);
		delete pbmp;
		pbmp = NULL;
	}

	// release resources
	if (pImageBuffer)
	{
		free(pImageBuffer);
		pImageBuffer = NULL;
	}

	if(pImage)
	{
		pImage->Release();
		pImage = NULL;
	}
	if (pImageFactory)
	{
		pImageFactory->Release();
		pImageFactory = NULL;
	}

	if (pStream)
	{
		pStream->Release();
		pStream = NULL;
	}

	if (hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}

	return;
}


void CMusicPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuilt();
		break;
	case EVT_MSDK_FINISHED:	
		break;
	case EVT_MSDK_ERROR_OCCURED:
		break;
	case EVT_MSDK_GRAPH_CHANGED:
		break;
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
		OnPosChange(param1, param2);
		break;
	case EVT_MSDK_PLAYED:
		SetSpectrumInfo();
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case EVT_MSDK_PAUSED:
	case EVT_MSDK_STOPED:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		break;
	default:
		break;
	}
}

void CMusicPlayerDlg::SetSpectrumInfo()
{
	HAUDIOINFO hAudioInfo = CMsdkRender::GetInstance()->FGetAudioInfoHandle();
	if (NULL != hAudioInfo)
	{
		m_pLayerSpectrum->SetAudioInfo(hAudioInfo);
		if (CMsdkRender::GetInstance()->FIsWmaPlaying())
		{
			m_pLayerSpectrum->SetSrcMode(SRC_MODE_MSK_WMA);
		}
		else
		{
			m_pLayerSpectrum->SetSrcMode(SRC_MODE_MSK);
		}
	}
}

void CMusicPlayerDlg::OnPosChange(UINT param1, UINT param2)
{
	if (param1 == 0)
	{
		return;
	}

	UINT curDuration = param1 / 1000;
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();

	// 进度条
	UpdataProgress(curDuration, outDuration);
	// 播放时间
	ShowMediaTimeInfo(curDuration, outDuration);
	// 歌词刷新
	ShowLRC(curDuration);
}

void CMusicPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CMsdkRender::GetInstance()->FSeek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}


void CMusicPlayerDlg::ShowFileTip()
{
	int cur = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	int total = CMsdkFileManager::GetInstance()->GetPlayingFilesCount(MSDK_FILE_MUSIC);
	if (total<0)
	{
		total = 0;	// 如果返回数据不合法,显示0
	}

	TCHAR buffer[10] = L"";
	if (total>0)
	{
		_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	}
	else
	{
		_stprintf(buffer, TEXT("_/_")); 
	}
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}

void CMusicPlayerDlg::ShowTitle()
{
	// title显示当前播放的目录
	CWceUiLayer* ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		CRect rc;
		TCHAR file_path[MAX_PATH];
		const CStringArray& folder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_MUSIC);
		int index = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(MSDK_FILE_MUSIC);
		if (index >= 0 && folder.GetCount()>0)
		{
			ptitle->GetWindowRect(rc);
			ui_format_file_path(ptitle->GetStyle()->GetFontCtrl()->GetFont(), rc, folder.GetAt(index), file_path);
			ptitle->SetText(file_path);
		}
	}
}

void CMusicPlayerDlg::ShowPlayMode()
{
	WORD random = CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_MUSIC);
	WORD repeat = CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_MUSIC);
	CWceUiLayer* pRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pRepeat = GetChildByName(L"repeat_tip");
	if (pRandom)
	{
		pRandom->SetStyle(random == 0 ? L"random_tip_off" : L"random_tip_on");
	}

	if (pRepeat)
	{
		LPCTSTR style[] = {L"repeat_tip_off", L"repeat_tip_single", L"repeat_tip_folder", L"repeat_tip_all"};
		if (repeat>=0 && repeat<4)
		{
			pRepeat->SetStyle(style[repeat]);
		}
	}

	ui_led_set_playmode(random, repeat);
}


LRESULT CMusicPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowMp3Tag();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CMusicPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		PlayPrev();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		PlayNext();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			CMsdkRender::GetInstance()->FPlay();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CMsdkRender::GetInstance()->FPause();
		}
		else
		{
			if( MSDK_MGSTATE_PLAYING == CMsdkRender::GetInstance()->FGetState())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CMsdkRender::GetInstance()->FPause();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				CMsdkRender::GetInstance()->FPlay();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		CMsdkRender::GetInstance()->FStop();
		break;
	case T_IR_GOTO:
		if (CMsdkFileManager::GetInstance()->PlayFileGoto(param-1, MSDK_FILE_MUSIC))	//成功GOTO指定曲目
		{
			LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(param-1, MSDK_FILE_MUSIC);
			CMsdkRender::GetInstance()->FSetSourceFile(file_path);
		}
		break;
	case T_REPEAT:
		OnRepeat();
		break;
	case T_SHUFFLE:
		OnRandom();
		break;
	}
}