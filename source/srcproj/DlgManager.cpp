#include "StdAfx.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"
#include "BaseDlg.h"
#include "sysUtil.h"
#include "interface.h"
#include "RadioRDSDlg.h"
#include "phonelink.h"

extern DLG_INFO g_dlg_info[];
int ui_get_dlg_info_count();

vector<DLG_INFO> CDlgManager::m_dlg_info;

extern int g_count;
CDlgManager::CDlgManager(void)
: m_cur_dlginfo(NULL),
  m_last_dlginfo(NULL)
{
	for (int i=0; i<ui_get_dlg_info_count(); i++)
	{
		m_dlg_info.push_back(g_dlg_info[i]);
	}
}

CDlgManager::~CDlgManager(void)
{
}

CDlgManager* CDlgManager::GetInstance()
{
	static CDlgManager dlg_manager;
	return &dlg_manager;
}

void CDlgManager::SetToDeleteFlag(DLG_ID did, SOURCE_ID sid, BOOL bToDelete)
{
	DLG_INFO* pdlg = GetDlgInfo(did, sid);
	if (pdlg)
	{
		pdlg->to_delete = bToDelete;
	}
}

void CDlgManager::ClearDlgResource(DLG_INFO* pdlgShow, DLG_INFO* pdlgToShow)
{
	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ( (*iter).to_delete && (*iter).is_created)
		{
			if (&(*iter) != pdlgShow && &(*iter) != pdlgToShow)	// �������Ĳ������������еĵ�ַ,��������ֱ�ӿ��ԱȽϵ�ַ
			{
				if (!IsExternalApp(&(*iter)))
				{
					DeleteDlg((*iter).dlg_id, (*iter).source_id);
				}
			}
		}
	}
}

void CDlgManager::NotifyDlgChanged(DLG_INFO* pdi_show, DLG_INFO* pdi_hide)
{
	DLG_ID did_show = pdi_show ? pdi_show->dlg_id : INVALID_DLG_ID;
	DLG_ID did_hide = pdi_hide ? pdi_hide->dlg_id : INVALID_DLG_ID;
	CMainWnd::GetInstance()->PostMessage(UI_MSG_DLG_CHANGE, 0, (did_hide<<16) | did_show);
}



BOOL CDlgManager::ShowDlg(DLG_ID did, SOURCE_ID sid)
{
	if (!IsDlgCreated(did, sid))
	{
		CreateDlg(did, sid);
	}
	DLG_INFO *pdi = GetDlgInfo(did, sid);

	// �����ǰ������topmostģʽ,Ҫ��ʾ�Ĵ��ڲ���topmost,���ں�̨������Ӧ�Ĵ���
	if (IsInTopMostMode() && !IsTopMostDlg(pdi))
	{
		return ShowDlgInBackground(did, sid);
	}

	if (pdi != NULL && pdi != m_cur_dlginfo)	// ����Ѿ��ǵ�ǰ�����ˣ������κβ���
	{
		NotifyDlgChanged(pdi, m_cur_dlginfo);

		ANIM_MODE am_show = ANIM_MODE_NONE;
		ANIM_MODE am_hide = ANIM_MODE_NONE;
		GetAnimMode(pdi, m_cur_dlginfo, &am_show, &am_hide);
		DWORD data_show = 0;
		DWORD data_hide = 0;
		GetAnimData(pdi, m_cur_dlginfo, &data_show, &data_hide);

		// ����ǰ��ʾ�Ĵ�������
		if (m_cur_dlginfo != NULL && m_cur_dlginfo->is_show)
		{
			if(m_cur_dlginfo->pdlg)
			{
				if (IsExternalApp(m_cur_dlginfo))
				{
					// ������ⲿ����Ҫ����,��Ҫ��ʾ�Ĵ��������ڲ�����,�ȰѴ����Ƶ�ǰ̨����ֹ����һ�µ���
					if (!IsExternalApp(pdi) 
						&& !sysutil::nss_get_instance()->sys_gps_foreground)
					{
						SetForegroundWindow(CMainWnd::GetInstance()->m_hWnd);
					}


					ExternalApp_Show_Fun show_fun = (ExternalApp_Show_Fun)m_cur_dlginfo->pdlg;
					show_fun(FALSE);
				}
				else
				{
					//m_cur_dlginfo->pdlg->AnimShowWindow(FALSE, am_hide, data_hide);
					m_cur_dlginfo->pdlg->ShowLayer(FALSE);
					m_cur_dlginfo->pdlg->OnDlgShow(FALSE);	// notify dlg show
				}
			}
			else
			{
				RETAILMSG(1, (L"[nvai_car]: CDlgManager::ShowDlg(), dlg info error.\n", did));
			}
			m_cur_dlginfo->is_show = FALSE;
		}

		// ��¼��֮ǰ����ʾ����
		// ��������������������ʱ��������,����¼��last dlg
		if (!IsTopMostDlg(m_cur_dlginfo))
		{
			m_last_dlginfo = m_cur_dlginfo;
		}

		// ����ָ����ǰ����
		m_cur_dlginfo = pdi;

		if (IsExternalApp(pdi))
		{
			if (!sysutil::nss_get_instance()->sys_gps_foreground)
			{
				CMainWnd::GetInstance()->RemoveTopmost();
				ExternalApp_Show_Fun show_fun = (ExternalApp_Show_Fun)m_cur_dlginfo->pdlg;
				show_fun(TRUE);
			}
			pdi->is_show = TRUE;
			SetActiveDlg(did, sid);
		}
		else
		{
			if (!sysutil::nss_get_instance()->sys_gps_foreground)
			{
				SetForegroundWindow(CMainWnd::GetInstance()->m_hWnd);
			}

			// ��ʾָ���Ĵ��ڲ�ָ��Ϊ�����
			//pdi->pdlg->AnimShowWindow(TRUE, am_show, data_show);
			pdi->pdlg->ShowLayer(TRUE);
			pdi->pdlg->OnDlgShow(TRUE);	// notify dlg show
			pdi->is_show = TRUE;
//			pdi->to_delete = FALSE;		// ������ʾ������,�����Լ�ɾ��,���Ǵ����������ô˱�־
			SetActiveDlg(did, sid);
		}

	
		// ����ʹ�õ�dlg����ɾ��
		ClearDlgResource(m_last_dlginfo, m_cur_dlginfo);

	}

	

	return TRUE;
}


BOOL CDlgManager::ShowDlgInBackground(DLG_ID did, SOURCE_ID sid)
{
	if (!IsDlgCreated(did, sid))
	{
		CreateDlg(did, sid);
	}

	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi != NULL && pdi != m_cur_dlginfo)	// ����Ѿ��ǵ�ǰ�����ˣ������κβ���
	{
		// ��̨�������µĴ���,����last dlgΪ��ǰҪ�����Ĵ���
		if (!IsTopMostDlg(pdi))
		{
			m_last_dlginfo = pdi;
		}

		// ��ʾָ���Ĵ��ڲ�ָ��Ϊ�����
		pdi->is_show = FALSE;
		pdi->to_delete = FALSE;		// ������ʾ������,�����Լ�ɾ��,���Ǵ����������ô˱�־
		SetActiveDlg(did, sid);
	}
	return TRUE;
}

BOOL CDlgManager::IsInTopMostMode()
{
	if ( sysutil::nss_get_instance()->sys_camera_status == 1
		|| ui_is_on_bt_call() 
		|| sysutil::nss_get_instance()->sys_ecar_status
		|| sysutil::nss_get_instance()->sys_rds_status)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgManager::IsTopMostDlg(DLG_INFO* pdi)
{
	if (pdi && 
		(pdi->dlg_id == CBTCallDlg_ID
		|| pdi->dlg_id == CCameraDlg_ID
		|| pdi->dlg_id == CEcarDlg_ID
		||(pdi->dlg_id == CRadioRDSDlg_ID && CRadioRDSDlg::IsTopMostDlg())))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDlgManager::GotoLastDlg()
{
	// ��lastdlg��Դ������ʱ,��������ʾ��Ӧ�Ĵ���
	if (m_last_dlginfo != NULL )
	{
		if( m_last_dlginfo->source_id < SOURCE_MAX_ID	// �Ǹ�������Դ, ����SOURCE_MAIN��SOURCE_SETTING��Щ
			&& sysutil::nss_get_instance()->is_source_available(m_last_dlginfo->source_id))
		{
			if(m_last_dlginfo->dlg_id != PhoneLink_ID || CPhoneLink::GetInstance()->IsConnected())
			{
				ShowDlg(m_last_dlginfo->dlg_id, m_last_dlginfo->source_id);
			}
		}
		else if (m_last_dlginfo->source_id > SOURCE_MAX_ID)
		{
			ShowDlg(m_last_dlginfo->dlg_id, m_last_dlginfo->source_id);
		}
	}
}

DLG_ID CDlgManager::GetLastDlgID()
{
	if (m_last_dlginfo != NULL)
	{
		return m_last_dlginfo->dlg_id;
	}
	else
	{
		return INVALID_DLG_ID;
	}
}

DLG_ID CDlgManager::GetCurDlgID()
{
	if (m_cur_dlginfo != NULL)
	{
		return m_cur_dlginfo->dlg_id;
	}
	else
	{
		return INVALID_DLG_ID;
	}
}

DLG_INFO* CDlgManager::GetLastDlgInfo()
{
	return m_last_dlginfo;
}

void CDlgManager::SetLastDlgInfo(DLG_INFO* pdi)
{
	if (pdi)
	{
		m_last_dlginfo = pdi;
	}
}

DLG_INFO* CDlgManager::GetCurDlgInfo()
{
	return m_cur_dlginfo;
}


BOOL CDlgManager::GotoSource(SOURCE_ID sid)
{
	DLG_INFO* pdi = GetActiveDlgInfo(sid);
	if (pdi)
	{
		return ShowDlg(pdi->dlg_id, sid);
	}

	return FALSE;
}

void CDlgManager::CloseSource(SOURCE_ID sid)
{
	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).source_id == sid)
		{
			// ��ʱ��ɾ������
			if ((*iter).is_created)
			{
				DeleteDlg((*iter).dlg_id, sid);
			}

			// ȥ��active����
			(*iter).is_active = FALSE;
		}
	}

}

DLG_INFO* CDlgManager::GetActiveDlgInfo(SOURCE_ID sid)
{
	DLG_INFO* first_pdi = NULL;
	DLG_INFO* active_pdi = NULL;

	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).source_id == sid)	// ��������ͬ����ACTIVE����
		{
			if (first_pdi == NULL)
			{
				first_pdi = &(*iter);
			}

			if ((*iter).is_active)
			{
				active_pdi = &(*iter);
			}
		}
	}

	// ���û�ҵ�active dlg������ƥ�䵽�ĵ�һ��
	return (active_pdi != NULL) ? active_pdi : first_pdi;
}

void CDlgManager::SetActiveDlg(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO* pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		vector<DLG_INFO>::iterator iter;
		for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
		{
			if ((*iter).source_id == pdi->source_id)	// ��������ͬ����ACTIVE����
			{
				(*iter).is_active = FALSE;
			}
		}

		if (!IsDlgCreated(did, sid))
		{
			CreateDlg(did, sid);
		}
		pdi->is_active = TRUE;	// ����ǰdlg��Ϊactive
	}
}


DLG_INFO* CDlgManager::GetDlgInfo(DLG_ID did, SOURCE_ID sid)
{
	// �Ż�������, �ظ���ѯʱ���ñ���
	static DLG_INFO *pdi = NULL;
	if (pdi!=NULL && pdi->dlg_id == did && pdi->source_id == sid)
	{
		return pdi;
	}

	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).dlg_id == did && (*iter).source_id == sid)
		{
			pdi= &(*iter);
			return &(*iter);
		}
	}

//	RETAILMSG(1, (L"[nvai_car]: CDlgManager::GetDlgInfo(), Could not find dialog(id=%d) in dlg_info table.\n", did));
	return NULL;
}


DLG_INFO* CDlgManager::GetDlgInfo(CBaseDlg* pdlg)
{
	if (pdlg == NULL)
	{
		return NULL;
	}

	// �Ż�������, �ظ���ѯʱ���ñ���
	static DLG_INFO *pdi = NULL;
	if (pdi!=NULL && pdi->pdlg == pdlg)
	{
		return pdi;
	}

	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).pdlg == pdlg)
		{
			pdi= &(*iter);
			return &(*iter);
		}
	}

	return NULL;
}

BOOL CDlgManager::IsDlgCreated(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		return pdi->is_created;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgManager::IsDlgShow(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		return pdi->is_show;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgManager::IsDlgActive(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		return pdi->is_active;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgManager::IsAnimating()
{
	if (m_cur_dlginfo && m_cur_dlginfo->pdlg && !IsExternalApp(m_cur_dlginfo))
	{
		return m_cur_dlginfo->pdlg->IsAnimating();
	}

	return FALSE;
}

CBaseDlg* CDlgManager::CreateDlg(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi && !IsExternalApp(pdi))
	{
		if (pdi->is_created)
		{
			RETAILMSG(1, (L"[nvai_car]: CDlgManager::CreateDlg(), warning, dialog(id=%d) already created.\n", did));
		}
		else
		{
			pdi->pdlg = (CBaseDlg*)CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(pdi->dlg_path.c_str());
			if (pdi->pdlg)
			{
				pdi->is_created = TRUE;
				pdi->pdlg->RunModeless();
			}
			else
			{
				RETAILMSG(1, (L"[nvai_car]: CDlgManager::CreateDlg(), warning, dialog(path=%s) create fail.\n", pdi->dlg_path.c_str()));
			}
		}

		return pdi->pdlg;
	}
	else
	{
		return NULL;
	}
}


BOOL CDlgManager::DeleteSrcDlg(SOURCE_ID sid)
{
	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).source_id == sid && (*iter).is_created)
		{
			DeleteDlg((*iter).dlg_id, sid);
		}
	}

	return TRUE;
}


BOOL CDlgManager::DeleteAllDlg()
{
	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).is_created)
		{
			DeleteDlg((*iter).dlg_id, (*iter).source_id);
		}
	}

	return TRUE;
}

BOOL CDlgManager::DeleteDlg(DLG_ID did, SOURCE_ID sid)
{
	if (IsExternalApp(did, sid))	// ������ⲿ����,ֱ�ӷ���TRUE
	{
		ResetDlgInfo(did, sid);
		return TRUE;
	}

	DLG_INFO *pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		if (!pdi->is_created)
		{
			RETAILMSG(1, (L"[nvai_car]: CDlgManager::DeleteDlg(), warning, dialog(id=%d) not created.\n", did));
		}
		else
		{
			if (pdi->pdlg)
			{
				pdi->pdlg->EndModeless();
				CMainWnd::GetInstance()->GetLayerCtrl()->DeleteLayer(pdi->pdlg);
				ResetDlgInfo(did, sid);
				RETAILMSG(1, (L"[nvai_car]: Dialog deleted, id=%d, sid=%d.\n", did, sid));
				return TRUE;
			}
			else
			{
				RETAILMSG(1, (L"[nvai_car]: CDlgManager::DeleteDlg(), dlg info error.\n"));
			}
		}
	}

	return FALSE;
}

void CDlgManager::ResetDlgInfo(DLG_ID did, SOURCE_ID sid)
{
	DLG_INFO* pdi = GetDlgInfo(did, sid);
	if (pdi)
	{
		if (IsExternalApp(pdi))
		{
			pdi->is_show = FALSE;
			pdi->is_active = FALSE;
		}
		else
		{
			pdi->is_show = FALSE;
			pdi->is_created = FALSE;
			pdi->pdlg = NULL;
			pdi->is_active = FALSE;
		}
	}
}

BOOL CDlgManager::GetAnimMode(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, 
				 OUT ANIM_MODE* pam_show, OUT ANIM_MODE* pam_hide)
{
	if (pdi_show == NULL || pam_show == NULL || pam_hide == NULL)
	{
		RETAILMSG(1, (L"[nvai_car]: CDlgManager::GetAnimMode(), invalid parameters.\n"));
		return FALSE;
	}
	// �ⲿ�����޶���
	if (IsExternalApp(pdi_show) || IsExternalApp(pdi_hide))
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case0: ���������ǰ̨����,����ʾ����
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}
	// �������ڲ�Ҫ����
	if (pdi_show->dlg_id == CCameraDlg_ID 
		|| (pdi_hide && pdi_hide->dlg_id == CCameraDlg_ID))	// pdi_hide����Ϊ��
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case1: ϵͳ��һ�������������״̬������ʾ״̬
	if (pdi_hide == NULL)
	{
		if (pdi_show->ui_level == 0)	// ����������ڶ�����ʾ���������ڲ�������
		{
			*pam_show = ANIM_MODE_MAIN;
		} 
		else
		{
			*pam_show = ANIM_MODE_NONE;
		}
		return TRUE;
	}

	// �������Ϊ�ڲ��������ڼ��л�


	// ָ��Ϊͬһ���ڣ���������
	if (pdi_show->dlg_id == pdi_hide->dlg_id)
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case2: Դ�ڲ����ڵ��л�
	if (pdi_show->source_id == pdi_hide->source_id)
	{
		// ���¼����ڼ��л�
		if (abs(pdi_show->ui_level - pdi_hide->ui_level) >= 1	// �п������༶
			&& pdi_show->dlg_id != CDVDStartDlg_ID && pdi_hide->dlg_id != CDVDStartDlg_ID)	// dvd���ش�����dvd���������ڲ�������
		{
			*pam_show = *pam_hide = ANIM_MODE_UPLEVEL;
		}
		else	// �����¼�
		{
			*pam_show = *pam_hide = ANIM_MODE_OTHER;
		}
		return TRUE;
	}

	// case3: ��ͬԴ���ڵ��л�
	// �л��漰��������
	if (pdi_show->ui_level == 0 || pdi_hide->ui_level == 0)
	{
		*pam_show = *pam_hide = ANIM_MODE_MAIN;
	}
	else	// ���漰������
	{
		*pam_show = *pam_hide = /*ANIM_MODE_OTHER*/ANIM_MODE_NONE;
	}

	return TRUE;

}


BOOL CDlgManager::GetAnimData(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, OUT DWORD* pdata_show, OUT DWORD* pdata_hide)
{
	if (pdi_show == NULL || pdata_show == NULL || pdata_hide == NULL)
	{
		RETAILMSG(1, (L"[nvai_car]: CDlgManager::GetAnimData(), invalid parameters.\n"));
		return FALSE;
	}

	// �ⲿ�����޶���
	if (IsExternalApp(pdi_show) || IsExternalApp(pdi_hide))
	{
		*pdata_show = *pdata_hide = 0;
		return TRUE;
	}


	// case1: ϵͳ��һ�������������״̬������ʾ״̬
	if (pdi_hide == NULL)
	{
		if (pdi_show->ui_level == 0)	// ����������ڶ�����ʾ���������ڲ�������
		{
			*pdata_show = CMainDlg_ID;
		} 
		else
		{
			*pdata_show = 0;
		}
		return TRUE;
	}

	// �������Ϊ�ڲ��������ڼ��л�

	// ָ��Ϊͬһ���ڣ���������
	if (pdi_show->dlg_id == pdi_hide->dlg_id && pdi_show->source_id == pdi_hide->source_id)
	{
		*pdata_show = *pdata_hide = 0;
		return TRUE;
	}

	// case2: Դ�ڲ����ڵ��л�
	if (pdi_show->source_id == pdi_hide->source_id)
	{
		// 0 ~ ��ʾ���¼���ϵ���ϼ�
		// 1 ~ ��ʾ���¼���ϵ���¼�
		// 2 ~ ��ʾƽ��
		if (pdi_show->ui_level == pdi_hide->ui_level)
		{
			*pdata_show = *pdata_hide = ANIM_LEVEL_BROTHER;
		}
		else if (pdi_show->ui_level > pdi_hide->ui_level)
		{
			*pdata_show = ANIM_LEVEL_CHILD;
			*pdata_hide = ANIM_LEVEL_PARENT;
		}
		else
		{
			*pdata_show = ANIM_LEVEL_PARENT;
			*pdata_hide = ANIM_LEVEL_CHILD;
		}
		
		return TRUE;
	}

	// case3: ��ͬԴ���ڵ��л�
	// �л��漰��������
	if (pdi_show->ui_level == 0 || pdi_hide->ui_level == 0)
	{
		// ȡ�������ڵ�source id
		SOURCE_ID sid = (pdi_show->ui_level == 0) ? (pdi_hide->source_id) : pdi_show->source_id;
		*pdata_show = *pdata_hide = GetTopDlgInSource(sid);
	}
	else	// ���漰������
	{
		*pdata_show = *pdata_hide = 0;
	}

	return TRUE;

}

DLG_ID CDlgManager::GetTopDlgInSource(SOURCE_ID sid)
{
	int level = 0xFFFF;		// large enough
	DLG_ID did = INVALID_DLG_ID;

	vector<DLG_INFO>::iterator iter;
	for (iter=m_dlg_info.begin(); iter!=m_dlg_info.end(); iter++)
	{
		if ((*iter).source_id == sid)
		{
			if (level > (*iter).ui_level)
			{
				level = (*iter).ui_level;
				did = (*iter).dlg_id;
			}
		}
	}

	return did;
}

inline BOOL CDlgManager::IsExternalApp(DLG_ID did, SOURCE_ID sid)
{
	return (did > ExternalApp_ID_MIN);
}

inline BOOL CDlgManager::IsExternalApp(const DLG_INFO* pdi)
{
	if (pdi && IsExternalApp(pdi->dlg_id, pdi->source_id))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}