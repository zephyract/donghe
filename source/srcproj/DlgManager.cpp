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
			if (&(*iter) != pdlgShow && &(*iter) != pdlgToShow)	// 传进来的参数就是数组中的地址,所以这里直接可以比较地址
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

	// 如果当前工作在topmost模式,要显示的窗口不是topmost,则在后台启动相应的窗口
	if (IsInTopMostMode() && !IsTopMostDlg(pdi))
	{
		return ShowDlgInBackground(did, sid);
	}

	if (pdi != NULL && pdi != m_cur_dlginfo)	// 如果已经是当前窗口了，不作任何操作
	{
		NotifyDlgChanged(pdi, m_cur_dlginfo);

		ANIM_MODE am_show = ANIM_MODE_NONE;
		ANIM_MODE am_hide = ANIM_MODE_NONE;
		GetAnimMode(pdi, m_cur_dlginfo, &am_show, &am_hide);
		DWORD data_show = 0;
		DWORD data_hide = 0;
		GetAnimData(pdi, m_cur_dlginfo, &data_show, &data_hide);

		// 将当前显示的窗口隐藏
		if (m_cur_dlginfo != NULL && m_cur_dlginfo->is_show)
		{
			if(m_cur_dlginfo->pdlg)
			{
				if (IsExternalApp(m_cur_dlginfo))
				{
					// 如果是外部窗口要隐藏,将要显示的窗口又是内部窗口,先把窗口推到前台，防止会闪一下导航
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

		// 记录下之前的显示窗口
		// 蓝牙，倒车属于特殊临时弹出窗口,不记录到last dlg
		if (!IsTopMostDlg(m_cur_dlginfo))
		{
			m_last_dlginfo = m_cur_dlginfo;
		}

		// 重新指定当前窗口
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

			// 显示指定的窗口并指定为活动窗口
			//pdi->pdlg->AnimShowWindow(TRUE, am_show, data_show);
			pdi->pdlg->ShowLayer(TRUE);
			pdi->pdlg->OnDlgShow(TRUE);	// notify dlg show
			pdi->is_show = TRUE;
//			pdi->to_delete = FALSE;		// 窗口显示出来后,不会自己删除,除非代码另外设置此标志
			SetActiveDlg(did, sid);
		}

	
		// 不在使用的dlg可以删除
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
	if (pdi != NULL && pdi != m_cur_dlginfo)	// 如果已经是当前窗口了，不作任何操作
	{
		// 后台启动了新的窗口,更新last dlg为当前要启动的窗口
		if (!IsTopMostDlg(pdi))
		{
			m_last_dlginfo = pdi;
		}

		// 显示指定的窗口并指定为活动窗口
		pdi->is_show = FALSE;
		pdi->to_delete = FALSE;		// 窗口显示出来后,不会自己删除,除非代码另外设置此标志
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
	// 当lastdlg的源不存在时,不允许显示对应的窗口
	if (m_last_dlginfo != NULL )
	{
		if( m_last_dlginfo->source_id < SOURCE_MAX_ID	// 是个真正的源, 不是SOURCE_MAIN或SOURCE_SETTING这些
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
			// 暂时作删除处理
			if ((*iter).is_created)
			{
				DeleteDlg((*iter).dlg_id, sid);
			}

			// 去掉active属性
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
		if ((*iter).source_id == sid)	// 重置所有同类别的ACTIVE属性
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

	// 如果没找到active dlg，返回匹配到的第一个
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
			if ((*iter).source_id == pdi->source_id)	// 重置所有同类别的ACTIVE属性
			{
				(*iter).is_active = FALSE;
			}
		}

		if (!IsDlgCreated(did, sid))
		{
			CreateDlg(did, sid);
		}
		pdi->is_active = TRUE;	// 将当前dlg置为active
	}
}


DLG_INFO* CDlgManager::GetDlgInfo(DLG_ID did, SOURCE_ID sid)
{
	// 优化下性能, 重复查询时不用遍历
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

	// 优化下性能, 重复查询时不用遍历
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
	if (IsExternalApp(did, sid))	// 如果是外部窗口,直接返回TRUE
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
	// 外部窗口无动画
	if (IsExternalApp(pdi_show) || IsExternalApp(pdi_hide))
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case0: 如果导航在前台运行,不显示动画
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}
	// 倒车窗口不要动画
	if (pdi_show->dlg_id == CCameraDlg_ID 
		|| (pdi_hide && pdi_hide->dlg_id == CCameraDlg_ID))	// pdi_hide可以为空
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case1: 系统第一次启动或从隐藏状态进入显示状态
	if (pdi_hide == NULL)
	{
		if (pdi_show->ui_level == 0)	// 如果是主窗口动画显示，其它窗口不作动画
		{
			*pam_show = ANIM_MODE_MAIN;
		} 
		else
		{
			*pam_show = ANIM_MODE_NONE;
		}
		return TRUE;
	}

	// 以下情况为内部二个窗口间切换


	// 指定为同一窗口，不做动画
	if (pdi_show->dlg_id == pdi_hide->dlg_id)
	{
		*pam_show = *pam_hide = ANIM_MODE_NONE;
		return TRUE;
	}

	// case2: 源内部窗口的切换
	if (pdi_show->source_id == pdi_hide->source_id)
	{
		// 上下级窗口间切换
		if (abs(pdi_show->ui_level - pdi_hide->ui_level) >= 1	// 有可能相差多级
			&& pdi_show->dlg_id != CDVDStartDlg_ID && pdi_hide->dlg_id != CDVDStartDlg_ID)	// dvd加载窗口与dvd的其它窗口不作动画
		{
			*pam_show = *pam_hide = ANIM_MODE_UPLEVEL;
		}
		else	// 非上下级
		{
			*pam_show = *pam_hide = ANIM_MODE_OTHER;
		}
		return TRUE;
	}

	// case3: 不同源窗口的切换
	// 切换涉及到主窗口
	if (pdi_show->ui_level == 0 || pdi_hide->ui_level == 0)
	{
		*pam_show = *pam_hide = ANIM_MODE_MAIN;
	}
	else	// 不涉及主窗口
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

	// 外部窗口无动画
	if (IsExternalApp(pdi_show) || IsExternalApp(pdi_hide))
	{
		*pdata_show = *pdata_hide = 0;
		return TRUE;
	}


	// case1: 系统第一次启动或从隐藏状态进入显示状态
	if (pdi_hide == NULL)
	{
		if (pdi_show->ui_level == 0)	// 如果是主窗口动画显示，其它窗口不作动画
		{
			*pdata_show = CMainDlg_ID;
		} 
		else
		{
			*pdata_show = 0;
		}
		return TRUE;
	}

	// 以下情况为内部二个窗口间切换

	// 指定为同一窗口，不做动画
	if (pdi_show->dlg_id == pdi_hide->dlg_id && pdi_show->source_id == pdi_hide->source_id)
	{
		*pdata_show = *pdata_hide = 0;
		return TRUE;
	}

	// case2: 源内部窗口的切换
	if (pdi_show->source_id == pdi_hide->source_id)
	{
		// 0 ~ 表示上下级关系的上级
		// 1 ~ 表示上下级关系的下级
		// 2 ~ 表示平级
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

	// case3: 不同源窗口的切换
	// 切换涉及到主窗口
	if (pdi_show->ui_level == 0 || pdi_hide->ui_level == 0)
	{
		// 取非主窗口的source id
		SOURCE_ID sid = (pdi_show->ui_level == 0) ? (pdi_hide->source_id) : pdi_show->source_id;
		*pdata_show = *pdata_hide = GetTopDlgInSource(sid);
	}
	else	// 不涉及主窗口
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