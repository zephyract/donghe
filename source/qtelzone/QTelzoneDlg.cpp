#include "stdafx.h"
#include "QTelzoneDlg.h"
#include "WceUiEditBox.h"
#include "QTelzone.h"

CQTelzoneDlg::CQTelzoneDlg(void)
{

}

CQTelzoneDlg::~CQTelzoneDlg(void)
{

}

void CQTelzoneDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayers();
}


void CQTelzoneDlg::InitLayers()
{

	m_pLayerFrameKeypad = GetChildByName(L"keypad_frame");
	ASSERT(m_pLayerFrameKeypad);

	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"keypad_editbox");
	if (pEditbox)
	{
		pEditbox->SetReadOnly(TRUE);
		pEditbox->OnFocus();
	}

	CWceUiButton* pButton = (CWceUiButton*)GetLayerByName(L"key_backspace");
	ASSERT(pButton);
	pButton->EnableMultiCmd(TRUE);
}


void CQTelzoneDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_REFRESH_ZONE)
	{
		ShowTelZone();
		KillTimer(TIMER_ID_REFRESH_ZONE);
	}

	__super::OnTimer(wIDEvent);
}

void CQTelzoneDlg::OnBnClick(CWceUiButton* pButton)
{
	if (OnBnKeypadClick(pButton))
	{
		SetTimer(TIMER_ID_REFRESH_ZONE, 10);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CQTelzoneDlg::ShowTelZone()
{
	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"keypad_editbox");
	CWceUiLayer* pResult = GetChildByName(L"result");
	if (pEditbox && pResult)
	{
		pResult->SetText(CQTelzone::GetInstance()->GetTelZone(pEditbox->GetData()));
	}
}


BOOL CQTelzoneDlg::OnBnKeypadClick(CWceUiButton* pButton)
{
	const int KEY_COUNT = 11;
	LPCTSTR key_layers[] = {L"key_0", L"key_1", L"key_2", L"key_3", L"key_4", L"key_5", L"key_6", 
		L"key_7", L"key_8", L"key_9", L"key_backspace"};
	TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'\b'};


	int i = 0;
	for (i=0; i<KEY_COUNT; i++)
	{
		if (pButton->IsEqualName(key_layers[i]))
		{
			break;
		}
	}

	if (i<KEY_COUNT)	// 匹配到了
	{
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[i], 0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
