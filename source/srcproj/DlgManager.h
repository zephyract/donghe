#pragma once
#include <vector>
#include "BaseDlg.h"
class CBaseDlg;

typedef void (*ExternalApp_Show_Fun)(BOOL bShow);


class CDlgManager
{
public:
	CDlgManager(void);
	~CDlgManager(void);

public:
	// ��ʾָ���ĶԻ���
	BOOL ShowDlg(DLG_ID did, SOURCE_ID sid);
	
	// �л���ָ����Դ��Ӧ�Ļ����,���û�л���ڣ���ʾ��һ��ƥ�䵽��
	// ����һ��Դ���ܶ�Ӧ������ڣ���ֻ��һ������ڣ����������м��书��
	// �´ν�����������ϴ��˳�ʱ�Ĵ���
	BOOL GotoSource(SOURCE_ID sid);

	// ���رա�Դ�����д��ڣ���һ�������٣�Ҳ�����ǽ�����Ϊ��ʼ��״̬��
	// �������Ϊȱʡ��(��Դ��֮ǰ�ļ�����Ч��)��������ڲ����٣�������Ҫ
	// ���³�ʼ�����ڵ����ݣ�������Ҫͳһ�Ľӿڣ���
	void CloseSource(SOURCE_ID sid);

	// ������һ����ʾ�Ľ��棬���û�м�¼����һ�εĽ���(����������������ڣ�������Ӧ�ò�����øýӿ�)��
	// ���������л�������ͣ���ڵ�ǰ����
	// ֻ��ĳЩ������ԴӲ�ͬ�ط�����ʱ���ŵ��øýӿڷ��أ����ܶ��������Ľ��滥����ʾ���γ���ѭ���ˣ�
	// ��Ŀǰ��֪��û�������������������ע��
	void GotoLastDlg();
	// ������һ�����ڵ�ID
	DLG_ID GetLastDlgID();
	DLG_ID GetCurDlgID();
	DLG_INFO* GetLastDlgInfo();
	DLG_INFO* GetCurDlgInfo();
	// �ⲿ���ÿ�����������last dlg����Ϣ, ע����������Ǵӱ����ȡ����Ч��Ϣ(ָ�����ĳ������),�����Ǹ���ʱָ��
	void SetLastDlgInfo(DLG_INFO* pdi);	

	BOOL IsDlgCreated(DLG_ID did, SOURCE_ID sid);
	BOOL IsDlgShow(DLG_ID did, SOURCE_ID sid);
	BOOL IsDlgActive(DLG_ID did, SOURCE_ID sid);
	BOOL IsAnimating();	// ��ǰ�����Ƿ��ڶ���,����ʱ��Ҫ���������Ϣ,(�����Ƿ�ҲҪ�����£�)

	static CDlgManager* CDlgManager::GetInstance();

	static vector<DLG_INFO> m_dlg_info;


	// ɾ��һ������
	BOOL DeleteDlg(DLG_ID did, SOURCE_ID sid);

	// ɾ��һ��Դ��ص����д���
	BOOL DeleteSrcDlg(SOURCE_ID sid);

	// ɾ�������Ѵ����Ĵ���,APP�˳�ʱ�ɵ��øýӿ�ɾ��dlg
	BOOL DeleteAllDlg();

	// ����һ������
	CBaseDlg* CreateDlg(DLG_ID did, SOURCE_ID sid);

	// ͨ��dlg id�ҵ���Ϣ���е���, �Ҳ�������NULL
	DLG_INFO* GetDlgInfo(DLG_ID did, SOURCE_ID sid);

	// ͨ��dlg�ҵ���Ϣ���е���, �Ҳ�������NULL
	DLG_INFO* GetDlgInfo(CBaseDlg* pdlg);


	// ��ָ����dlg��Ϊ�״̬����ʱ���ͬ��������dlg��Ϊ�ǻ״̬
	// ����Ի���û�д��������ȴ����Ի���
	void SetActiveDlg(DLG_ID did, SOURCE_ID sid);

	// ���˱�־��dlg��ܻ��ں���ʱ���ͷ���Դ
	void SetToDeleteFlag(DLG_ID did, SOURCE_ID sid, BOOL bToDelete);

	// ��dlg info�в���ָ��Դ�Ļ���ڣ��Ҳ�������NULL
	// ���û��ָ������ڣ�����ƥ�䵽�ĵ�һ����
	DLG_INFO* GetActiveDlgInfo(SOURCE_ID sid);

	// �ж��Ƿ�Ϊ�ⲿAPP�Ĵ���
	inline BOOL IsExternalApp(DLG_ID did, SOURCE_ID sid);
	inline BOOL IsExternalApp(const DLG_INFO* pdi);


protected:

	// ��һ��dialog���ٺ������Ϣ��������
	void ResetDlgInfo(DLG_ID did, SOURCE_ID sid);


	// ��ȡ�����Ի�����л�ʱ�Ķ���Ч��
	BOOL GetAnimMode(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, 
		OUT ANIM_MODE* pam_show, OUT ANIM_MODE* pam_hide);

	// ��ȡ�����Ի�����л�ʱ�Ķ���Ч���ĸ�������
	// Ŀǰ�Ƿ�����������ͼ���Ӧ��dlg id(��Ҫʱ)
	BOOL GetAnimData(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, 
		OUT DWORD* pdata_show, OUT DWORD* pdata_hide);

	// ��ȡָ��Դ�����ϲ㴰��ID(ui_levelֵ��С)
	// ���ж�����ϲ㴰��ʱ(��SD����������ƵͼƬ����),���ص�һ��
	DLG_ID GetTopDlgInSource(SOURCE_ID sid);

	// ������ʹ�õ�dlg��Դ�ͷŵ�
	// ���������ֱ�ָ��������ʾ���뽫Ҫ��ʾ��,�����dlg����ɾ��
	void ClearDlgResource(DLG_INFO* pdlgShow, DLG_INFO* pdlgToShow);

	// �����л�ʱ,�㲥�����л���Ϣ
	void NotifyDlgChanged(DLG_INFO* pdi_show, DLG_INFO* pdi_hide);

	// BT��������ECar������Ϊ�Ƕ��㴰�ڣ�
	BOOL IsTopMostDlg(DLG_INFO* pdi);
	// ��������topmostģʽ(����BTͨ��,����,ECar)ʱ���������������Ҫ��ʾ��ֻ���ں���ʾ
	BOOL IsInTopMostMode();
	// ��̨�����Ի���
	BOOL ShowDlgInBackground(DLG_ID did, SOURCE_ID sid);

	DLG_INFO* m_cur_dlginfo;	// ��ǰ������Ϣ

	// ��һ����ʾ�Ĵ�����Ϣ,��Щ�����˳�ʱ�߼�Ϊ����������������ȥ
	DLG_INFO* m_last_dlginfo;

};
