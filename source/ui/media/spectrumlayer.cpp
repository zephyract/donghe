#include "stdafx.h"
#include "LyricEditBox.h"
#include "SpectrumLayer.h"
#include "DVP_Agent.h"
#include "sysUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSpectrumLayer::CSpectrumLayer()
: m_hAudioInfo(NULL)
, m_nMode(SRC_MODE_MSK)
{
}

CSpectrumLayer::~CSpectrumLayer()
{

}


void CSpectrumLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	CRect rc_client;
	GetWindowRect(&rc_client);

	long fft_data[64] = {0,};

	GUINT8 u1Short[EQ_MAX_FREQ_CNT] = {0,}; //length of Short Term
	GUINT8 u1Long[EQ_MAX_FREQ_CNT] = {0,}; //length of Long Term

	if ((m_nMode==SRC_MODE_MSK && m_hAudioInfo))
	{
  		AudioInfo_GetSpectrums(m_hAudioInfo, u1Short, u1Long);
		for (int i=0; i<EQ_MAX_FREQ_CNT; i++)
		{
// 			fft_data[i*2] = u1Short[i]-40;
// 			fft_data[i*2+1] = u1Long[i]-40;

			fft_data[i] = (u1Short[i] + u1Long[i]) / 2 - 40;
		}
	}
	else if ((m_nMode==SRC_MODE_MSK_WMA && m_hAudioInfo))
	{
		AudioInfo_Wma_GetSpectrums(m_hAudioInfo, u1Short, u1Long);
		for (int i=0; i<EQ_MAX_FREQ_CNT; i++)
		{
			fft_data[i*2] = u1Short[i]-40;
			fft_data[i*2+1] = u1Long[i]-40;
		}
	}
	else if (m_nMode == SRC_MODE_DVD && 
		!sysutil::nss_get_instance()->audio_is_media_front_mute 
		&& sysutil::nss_get_instance()->audio_media_front_volume>0)	// 静音时关掉频谱
	{

		DVP_AudioInfo_GetSpectrum(u1Short, u1Long);
		for (int i=0; i<EQ_MAX_FREQ_CNT; i++)
		{
			fft_data[i*2] = u1Short[i]-60;
			fft_data[i*2+1] = u1Long[i]-60;
		}
	}
	else
	{
		memset(fft_data, 0, sizeof(fft_data));
	}

	// 只用前面的32个数据
	static long mybuff[64] = {0,};
	for (int i=0; i<16; i++)
	{
		mybuff[i] -= 4;
		if (mybuff[i] < 0)
		{
			mybuff[i] = 0;
		}

		if (mybuff[i] < fft_data[i])
		{
			mybuff[i] = fft_data[i];
		}
		// 调整为3的倍数
		mybuff[i] = mybuff[i] - mybuff[i]%3;
	}

	static long mybuff_top[64] = {0,};
	for (int i=0; i<16; i++)
	{
		mybuff_top[i] -= 2;
		if (mybuff[i] <= 0)		// 没有柱子了
		{
			mybuff_top[i] = max(0, mybuff_top[i]);
		}
		else
		{
			mybuff_top[i] = max(mybuff[i]+2, mybuff_top[i]);
		}
	}

	RECT rc;

	CWceUiLoadBitmap redTexture;
	CWceUiLoadBitmap yellowTexture;
	if (WceUiGetBitsPixel() == 16)
	{
		redTexture.SetBitmapFromRes(L"media\\red_line.565a.conf");
		yellowTexture.SetBitmapFromRes(L"media\\yellow_line.565a.conf");
	}
	else
	{
		redTexture.SetBitmapFromRes(L"media\\red_line.32a.conf");
		yellowTexture.SetBitmapFromRes(L"media\\yellow_line.32a.conf");
	}
	const int SPEC_WIDTH = 13;
	for (int i=0; i<16; i++)
	{
		for (int j=0; j<=mybuff[i]; j+=3)
		{
			::SetRect(&rc, rc_client.left + 5 + i*SPEC_WIDTH, rc_client.bottom-1 - j,
				rc_client.left + 5 + i*SPEC_WIDTH + 10, rc_client.bottom-1 - j+1);

			DrawBmp(pWndBitmap,rc.left,rc.top,yellowTexture.GetBitmap());
		}

		::SetRect(&rc, rc_client.left + i*SPEC_WIDTH + 5, rc_client.bottom-1 - mybuff_top[i],
			rc_client.left + i*SPEC_WIDTH + 5 + 10, rc_client.bottom-1 - mybuff_top[i]+1);

		DrawBmp(pWndBitmap,rc.left,rc.top,redTexture.GetBitmap());
	}
}

void CSpectrumLayer::DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap)
{	
	if (pSrcBitmap)
	{
		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		CRect rc(x, y, x + pSrcBitmap->GetWidth(), y + pSrcBitmap->GetHeight());
		pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),pSrcBitmap,0,0,bf);
	}
}

