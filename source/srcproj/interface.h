// ���ﶨ�����ⲿ�Ľӿ�

#pragma once

#define UI_MAIN_WINDOW_NAME		L"Appolo_Main_Window"

// LPARAM ~ ��ʶlanguage ID
// 0 ~ english, 1 ~ chinesegb, 2 ~ chinesebig5
const UINT NAVI_MSG_LAN_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_LAN_CHANGE");

// LPARAM ~ ��ʶ��������
const UINT NAVI_MSG_BK_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_BK_CHANGE");


// ACC OFFʱ��㲥����Ϣ, �㲥��Ϣ��MCU����ʱһ��ʱ�����(ȱʡ�����ԼΪ4��)
const UINT NAVI_MSG_ACC_OFF		= ::RegisterWindowMessage(L"NAVI_MSG_ACC_OFF");

// ACC OFF��ARM��û����ʱ��ACC���ϵ��ˣ�ϵͳ��㲥����Ϣ֪ͨ���¼�
// ����״̬���ϵ磬�ǲ���㲥����Ϣ�ģ��������MCUҲû�취֪ͨAPP��ϵͳ��û����
const UINT NAVI_MSG_ACC_ON	= ::RegisterWindowMessage(L"NAVI_MSG_ACC_ON");

// �յ�����Ϣ������˳�, ��������APPʱ���ܻ��õ�����Ϣ
const UINT NAVI_MSG_CLOSE_PROCESS = ::RegisterWindowMessage(L"NAVI_MSG_CLOSE_PROCESS");

// ʱ����ʾ��ʽ�仯, LPARAM ~ 1 ��ʾ24Сʱ�ƣ� 0��ʾ12Сʱ��
const UINT NAVI_MSG_24HOURTIME_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_24HOURTIME_CHANGE");

// GPS��λ״̬��δ��λ״̬�仯ʱ���ᴥ������Ϣ
// LPARAM : 0 ~ δ��λ�� 1 ~ ����λ
const UINT NAVI_MSG_GPS_FIXED = ::RegisterWindowMessage(L"NAVI_MSG_GPS_FIXED");

// ���������˳�ʱ��㲥����Ϣ
// LPARAM : 0 ~ δ��⵽���������� 1 ~ ��⵽�����������˳�ʱ��������Ϣ
const UINT NAVI_MSG_FASTCAMERA_EXIT = ::RegisterWindowMessage(L"NAVI_MSG_FASTCAMERA_EXIT");

// �յ�����ϢAPP���˳�
const UINT NAVI_MSG_APP_EXIT = ::RegisterWindowMessage(L"NAVI_MSG_APP_EXIT");
// ��������Ҫ����APPʱ���ȷ������Ϣ����,�յ�����Ϣ������˳�,�������ߺø����ļ�����
const UINT NAVI_MSG_APP_UPDATE_START = ::RegisterWindowMessage(L"GPSAPP_UPDATE_START");


/***************************************************************************
*                     �ڲ���Ϣ���� (WM_USER ~ WM_USER+100)                 *
***************************************************************************/
// GPS���ݸ��º󣬻ᴥ������Ϣ
#define UI_MSG_GPS_STATUS_UPDATE		(WM_USER + 1)
#define UI_MSG_GPS_FIRMWARE_VERSION		(WM_USER + 2)

// ʱ���ı��ˣ�Ҳ������ʱ��仯
#define UI_MSG_TIMEZONE_CHANGE			(WM_USER + 3)

// mainwnd�յ�����Ϣ��������GPSʱ������ϵͳʱ�䣬���GPSû��λ�������ã�GPS��λ���ٴ�������ʱ��
#define UI_MSG_GPS_SETTIME				(WM_USER + 4)

// EQ���ͱ仯��,���͸���Ϣ
// lParam: EQ����
#define UI_MSG_EQ_CHANGE				(WM_USER + 5)

// ֪ͨAUX����,��Ƶ�����б仯
#define UI_MSG_AUX_VIDEO_PARAMS_CHANGE	(WM_USER + 6)

// �ָ��������ú�,�㲥����Ϣ,��Щ������ˢ��UI
#define UI_MSG_SYSTEM_RESET					(WM_USER + 7)

//  �����ڹ���ģ�鵯���µĴ���ʱ����㲥����Ϣ
//  LPARAM : High wordΪ֮ǰ��dlg id��Low wordΪҪ��ʾ��dlg id
#define UI_MSG_DLG_CHANGE					(WM_USER + 8)

// ���������б仯,��ЩUI��״̬������Ҫ����
#define UI_MSG_FACTORY_SET_CHANGE			(WM_USER + 9)

/***************************************************************************
*                               MCU              	                       *
***************************************************************************/
// MCU������������Ϣ
// wParam : byte0 ΪЭ���ж��尴����𣬼�����ֵkey_id, byte1Ϊ����״̬
// lParam : ����
#define UI_MSG_MCU_KEY_COMMAND			(WM_USER + 10)

// �յ�MCU��radio������
// lParam: ��Ϣ������(�ο�Э��0xB4) 0 ~ radio status, 1 ~ preset list ...
#define UI_MSG_MCU_RADIO_DATA			(WM_USER + 11)

// MCU���豸״̬�仯��
// lParam: MCU_STATUSָ��,��������,���,ɲ��״̬��
#define UI_MSG_MCU_STATUS_CHANGE		(WM_USER + 12)

// MCU����SWC��
// lParam: MCU_SWC_TABLEָ��,
#define UI_MSG_MCU_SWC_TABLE			(WM_USER + 13)

// MCU����������
// lParam: LOWORD ��ʾ�Ƿ񱣴�(��ӦЭ��byte0), HIWORD ��ʾkeyID(��ӦЭ��byte1)
#define UI_MSG_MCU_SWC_KEY_RESULT		(WM_USER + 14)

// CAN BUS�����и���
// lParam: ��Ϣ����
#define UI_MSG_MCU_CANBUS_DATA			(WM_USER + 15)

// MCU��ϵͳ�����仯��
// lParam: ��������,���ĸ������仯��
#define UI_MSG_MCU_SYSPARAMS_CHANGE		(WM_USER + 16)

// �û�����ң�������ּ����յ������Ϣ
// lParam: ��Ϻ����������
#define UI_MSG_MCU_DIGITAL_KEY			(WM_USER + 17)


/***************************************************************************
*                               SOURCE���         	                       *
***************************************************************************/
// ����һ��Դʱ֪ͨ����Ϣ
// wParam : DVP_SINK ָ��sink����
// lParam : SOURCE_ID Դid��ָ���ĸ�ԴҪֹͣ
#define UI_MSG_LOAD_SOURCE_NOTIFY		(WM_USER + 18)

// ֹͣһ��Դʱ֪ͨ����Ϣ
// lParam : SOURCE_ID Դid��ָ���ĸ�ԴҪֹͣ
#define UI_MSG_STOP_SOURCE_NOTIFY		(WM_USER + 19)

// ��һ��Դ���ڿ���(��ο�,����,����,��Ϊ��ʱ),��Դ��������Ҫ���˳�Դ,�ô���Ϣ֪ͨ������
// ��������ȥ������һ������Դ
// lParam : SOURCE_ID Դid��ָ���ĸ�ԴҪ�˳�
#define UI_MSG_SOURCE_EXIT				(WM_USER + 20)

// DVD��������Ϣ֪ͨ
// wParam : 0 ~ �н���������������Ҫ��Դ�� 1 ~ �н�������������������Դ
// lParam : 0 ~ ������ 1 ~ ����
#define UI_MSG_DVD_EJECT_NOTIFY				(WM_USER + 21)

// ר�ŷ���DVD���ػ����,��ִ�м��ض���
#define UI_MSG_DVD_ANIM_LOADING				(WM_USER + 22)

// ������ֹ��(��Ϣ��HOOK�ػ���),����е��,���������ڷ��͸���Ϣ֪ͨ
// ������HOOK�������Ĳ���һ��
// ����尴��Ҳ���ܴ�������Ϣ,������������Ϊ0
#define UI_MSG_TP_CLICK						(WM_USER + 23)

// ֻҪ��걻�����,�ͻᷢ�͸���Ϣ
#define UI_MSG_LBUTTON_DOWN				(WM_USER + 24)

// ֻҪ��걻�����,�ͻᷢ�͸���Ϣ
#define UI_MSG_LBUTTON_UP				(WM_USER + 25)

/***************************************************************************
*                     MSDK�ļ�����ʱ���¼�֪ͨ��Ϣ	                       *
***************************************************************************/
#define UI_MSG_MSDK_MEDIAGRAPH_FILE		(WM_USER + 50)
// MSDK AVINʱ���¼�֪ͨ��Ϣ
#define UI_MSG_MSDK_MEDIAGRAPH_AVIN		(WM_USER + 51)
// DVD����ʱʱ���¼�֪ͨ��Ϣ
#define UI_MSG_DVP_AGENT				(WM_USER + 52)
// wma
#define UI_MSG_MSDK_MEDIAGRAPH_WMA		(WM_USER + 53)

// ֪ͨ�и��ļ�Ҫ������
#define UI_MSG_MSDK_PLAY_FILE			(WM_USER + 54)

/***************************************************************************
*                                  BT 	                                   *
***************************************************************************/
// ����״̬�仯ʱ����֪ͨ����
#define UI_MSG_BT_CONNECT_STATUS_CHANGE		(WM_USER + 60)
// �����¼�֪ͨ
// lParam: ~ �¼�������
#define UI_MSG_BT_CALL_EVENT				(WM_USER + 61)

// ͬ������ʱ�¼�֪ͨ
// wParam: ~ ��WORDΪ�����ؼ�¼������,��WORDΪ�������ݵĸ���E_BT_PBAP_SYNC_PATH_T(�绰����δ�������)
// lParam: ~ �¼�������
#define UI_MSG_BT_PBMGR_SYNC_EVENT			(WM_USER + 62)

// �����¼�֪ͨ
// lParam: ~ �¼�������
#define UI_MSG_BT_MUSIC_EVENT				(WM_USER + 63)

// ����ͨ����¼�仯֪ͨ
// lParam: ~ ��¼������E_BT_PBAP_SYNC_PATH_T, ���Ϊ¼������Ϊ0xFF(RECENTS_TYPE_RECORD)
#define UI_MSG_BT_LOCAL_CALLINFO_CHANGE			(WM_USER + 64)

// �����绰�����������仯ʱ���͸���Ϣ
#define UI_MSG_BT_PB_QUERY_CHANGE			(WM_USER + 65)

/***************************************************************************
*                               USB IPOD	                               *
***************************************************************************/
// ipod��֤�ɹ��������Ϻ��γ�IPODʱ,���͸���Ϣ
// lParam: 0 ~ �Ͽ�����, 1 ~ ���ӳɹ�
#define UI_MSG_USB_IPOD_CONNECT				(WM_USER + 70)

// ipod Playback ״̬��Ϣ
// lParam ~ ��ǰ����λ����Ϣ
#define UI_MSG_USB_IPOD_PB_POS_CHANGED		(WM_USER + 71)

// lParam ~ ��ǰ������Ŀ����
#define UI_MSG_USB_IPOD_PB_TRACK_CHANGED	(WM_USER + 72)

// lParam ~ MUSB_IPOD_PLAY_ST_EX_E����,������ǰ����״̬(����,��ͣ��)
#define UI_MSG_USB_IPOD_PB_STATUS_CHANGED		(WM_USER + 73)


/***************************************************************************
*                               Storage card                               *
***************************************************************************/

// wParam : UI_MASS_STORAGE_TYPE���������ĸ��������
// lParam : UI_MASS_STORAGE_PLUG�������ǲ��뻹�ǰγ�
#define UI_MSG_STORAGE_CARD_PLUG				(WM_USER + 80)


/***************************************************************************
*         �ڲ��ؼ������붨�� (WCEUI_CMD_USER ~ WCEUI_CMD_USER+100)         *
***************************************************************************/

/*
* CPhotoShowLayer ����ǰ�����ļ��仯ʱ�����͸�����
* lParam: ��ǰ�����ļ�����
*/
#define UI_CMD_PHOTOSHOW_SELCHANGE		(WCEUI_CMD_USER + 1)

/*
* CPhotoShowLayer ȫ��״̬�л�ʱ�����͸�����
* lParam: 0 ~ ��ȫ���� 1 ~ ȫ��
*/
#define UI_CMD_PHOTOSHOW_FULLSCREEN		(WCEUI_CMD_USER + 2)

/*
* CPhotoShowLayer ����״̬�л�ʱ�����͸�����
* lParam: 0 ~ ֹͣ���ţ� 1 ~ ����
*/
#define UI_CMD_PHOTOSHOW_PLAY_STATUS	(WCEUI_CMD_USER + 4)

/*
* CBalanceSlider ȫ��״̬�л�ʱ�����͸�����
* lParam: hight wordΪ��ǰ����״̬��low wordΪ��ǰ��λ��
*/
#define UI_CMD_BSLIDER		(WCEUI_CMD_USER + 3)

/***************************************************************************
*                           ������Ϣ����                              *
***************************************************************************/

// ֪ͨ�������洰���˳�
#define UI_MSG_NAVI_WARNGINDLG_EXIT				(WM_USER + 0x800)