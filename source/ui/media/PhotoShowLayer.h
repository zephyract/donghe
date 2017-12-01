#pragma once
#include "WceUiLayer.h"
#include "WceUiScrollLayer.h"
#include "WceUiLoadBitmap.h"
#include "WceUi565Bitmap.h"
#include "WceUi32Bitmap.h"
#include <afxmt.h>
#include "WceUiPageLayer.h"

#ifdef _WIN32_WCE
#include "filefind.h"
#include <imaging.h> 
#endif

#include "GDef.h"

const int MAX_IMAGE_BUFFERED = 3;
#define ID_TIMER_REBUILD_IMAGE 100
#define ID_TIMER_SLIDE		   101
#define ID_TIMER_RESIZE_IMAGE  102	// ȫ���л�ʱ������������ͼƬ�Ĵ�С
#define ID_TIMER_SHOWINVILIDICON	 103

struct IMAGE_INFO 
{
	// ���ɵ�IImage�ӿ�
	// ������������image̫�󣬽������������С����󲻳�����Ļ�Ķ���
	GHANDLE pimage;
	GHANDLE pbitmap;

	// bmp��ʽ��ͼƬ������ʾ����һ���󣬼ӿ���ʾ�ٶ�
	CWceUiGenericBitmap *pbmpImage;
	// ��ͼƬ�Ŵ�󣬽����ڸ�λͼ�Ͻ��������������λͼ�Ĵ�С�����ŷŴ���С�������仯��
	// ���ʱ����������죬��������Ϊ������϶�Ч��
	CWceUiGenericBitmap *pbmpImageLarge;
	// �ս��зŴ����ʱ���õ��������ķŴ�ͼ������ͼ�п��ܱ�ԭʼͼƬҪС����Ϊ���������Ŵ�����
	// �����Ŵ�����ͼ��������ͼ���ɡ�
	// ��ô������Ϊ����IImage draw�ٶȱȽ����������Ŵ�ʱ���������ַ�ʽ���ܺã�ȱ���Ƕ����Щ�ڴ�
	CWceUiGenericBitmap *pbmpImageLargest;
	// ����ͼƬ��С(image�Ĵ�С����ͼƬ̫���п����ǵ�����ĳߴ�)�������Ļ�Ĵ�С
	// �õ�����С���ʣ���Сʱ����С���˱���. ���ͼƬ����ĻС�����ֵӦΪ1������ϴ�
	// ����ֵ���ʱ��ͼƬ�պ�������Ļ��
	double minRatio;
	// ���ֵΪminRatio�Ķ������Ŵ���̫��û���壬���ͼƬ�Ƚϴ�ʱ�������޷���ɸò���.
	double maxRatio;
	// ��ǰ�ķŴ����
	double curRatio;
	// ��ǰ��ʾ���ֵ����ĵ�����(��λͼ��������λ��)���϶�ʱ��ֵ��仯
	POINT ptCenter;
	// ��ǰ�������С���Ŵ���С�����������ֵ�仯����ʼֵӦΪ�պ������
	// ��Ļ��С�����������ݺ��
	// SIZE szDst;
	// ͼƬ��ȫ·���ļ���
	TCHAR strFileName[MAX_PATH];
};


class CPhotoShowLayer : public CWceUiPageLayer
{
	WCEUI_DYNCREATE_BEGIN(CPhotoShowLayer, CWceUiPageLayer);
public:
	CPhotoShowLayer(void);
	virtual ~CPhotoShowLayer(void);

	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	//	virtual void OnDrawText(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnInitLayer();
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags, POINT point);
	virtual void OnMouseMove(UINT nFlags, POINT point);

	void InitFileList(CStringArray *paFile, int nIndexPlaying);
	void OnSelectChange();
	// Ŀ¼�仯�ˣ�����ô˷���
	void OnFileListChange();

	void SetFullScreen(BOOL bFullScreen);
	BOOL GetFullScreen() {return m_bFullScreen;}
	void PlaySlide();
	void StopPlaySlide();
	BOOL IsPlayingSlide();
	void Rotate();
	void ZoomIn();
	void ZoomOut();
	
	// �ṩ��ң�����Ľӿ�,����ZOOM������ѭ���Ŵ����С
	// ����ֵ��ʾ���ǵ�ǰִ�еĲ���, TRUE ~ �Ŵ� FALSE ~ ��С
	BOOL Zoom();
	BOOL m_bZoom;	// ������¼��һ�ηŴ����С�Ķ���, TRUE ~ �Ŵ� FALSE ~ ��С

	void Next();
	void Prev();
	void PlayItem(int nIndex);

	GHANDLE GetCurGBitmap();

protected:
	virtual void OnScrollWindowEnd();
	virtual void OnMouseClicked();
	virtual void OnMouseScrolled();

protected:

	// play file list, get it from parent dialog.
	// it's a pointer to parent's member, initialized by parent
	CStringArray* m_pPlayFileList;

	// �ؼ��Լ���¼�ĵ�ǰ���ŵ�����
	// ��Ϊ�ؼ������Լ���������һҳ����ʱ�븸���ڵ�������һ�£�ͨ�������Żᱣ��һ��
	// ������ĳһʱ�����ǵ�ֵ�����ǲ�һ����
	int m_nIndexPlaying;

	// �������ϵͳ�Ỻ��5�������ͼƬ���ӿ���ʾ�ٶȣ������¼��
	// ������image����Ӧ���ļ���
	// ������ʾ�м�����ţ�����ǰ���ż�����ţ���û�оͲ�����

	IMAGE_INFO m_image[MAX_IMAGE_BUFFERED];

	// ���ػ�û���ص��軺���ͼƬ��һ��ֻ����һ�ţ��ú�������ö�β��ܽ���������صļ�����ϣ����Լ�������������
	// �����ǰ��ͼƬ����,��Ҫ���¹���image����,
	void ReBufferImage();

	// ���ػ�û���ص��軺���ͼƬ��һ��ֻ����һ�ţ��ú�������ö�β��ܽ���������صļ�����ϣ����Լ�������������
	BOOL RebuildImage();
	static UINT RebuildImageProc(LPVOID lpData);
	CEvent m_eventRebuildImage;
	CSingleLock m_singleLock;
	BOOL m_bExitRebuildImageProc;
	CWinThread *m_pThreadRebuildImage;
	CMutex m_mutexRebuildImage;
	void CreateRebuildImageThread();
	void DeleteRebuildImageThread();

	void ResizeImage();
	BOOL LoadImageFromFile(LPCTSTR lpszFilePath, OUT GHANDLE* phImage, OUT GHANDLE* phBitmap);

	BOOL LimitImageSize(IN OUT IImage** ppimage);
	BOOL RotateImage(IN IImage* pimageIn, OUT IImage** ppimageOut);
	SIZE GetImageSize(GHANDLE hImage);
	BOOL DrawImage(CWceUiGenericBitmap* pbmp, CRect rcDst, GHANDLE hImage);
	// �������ʧ�ܣ� ��ʾ����ʧ�ܵ���Ϣ
	void DrawErrorInfo(CWceUiGenericBitmap* pbmp, CRect rcDst);

	// call it to recreate the buffered bmp and snap bmp when changed between
	// full screen mode and normal mode.
	void OnLayerSizeChanged();


	int GetCount();
	int GetVisibleItems(OUT int *pnIndexStart, IN CRect &rcClient, IN CRect& rcPaint);
	// image item width.
	int m_cyItemWidth;
	// the layer's position.
	CRect m_rcLayer;

	// �������λͼ�����ͼ��ʱ�����������λͼ��OK���������primary surface
	//	CWceUi565Bitmap m_bmpSnap;

	void ResetScrollRange();
	BOOL m_bFullScreen;

	int m_nSlideTime;
	BOOL m_bIsSliding;		// set to TRUE if it is playing slider.


	double GetZoomRate(int cxSrc, int cySrc, int cxDst, int cyDst);

	BOOL m_bIsNeedMoving;	// set to TRUE if picture is zoomed in and it is larger than display area.
	BOOL m_bPictureMoved;	// set to TRUE if picture is moved.
	POINT m_ptPrevious;
	void MovePicture(int cx, int cy);
public:
	void OnImageEvent(GHANDLE hImage, UINT uEvent, UINT uResult); 
};
WCEUI_DYNCREATE_END(CPhotoShowLayer, CWceUiPageLayer);
