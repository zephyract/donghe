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
#define ID_TIMER_RESIZE_IMAGE  102	// 全屏切换时，需调整缓冲的图片的大小
#define ID_TIMER_SHOWINVILIDICON	 103

struct IMAGE_INFO 
{
	// 生成的IImage接口
	// 如果解码出来的image太大，将重新设置其大小，最大不超过屏幕的二倍
	GHANDLE pimage;
	GHANDLE pbitmap;

	// bmp格式的图片，与显示区域一样大，加快显示速度
	CWceUiGenericBitmap *pbmpImage;
	// 当图片放大后，将画在该位图上进行输出操作，该位图的大小将随着放大缩小操作而变化，
	// 输出时不需进行拉伸，这样做是为了提高拖动效果
	CWceUiGenericBitmap *pbmpImageLarge;
	// 刚进行放大操作时，得到这张最大的放大图，这张图有可能比原始图片要小，因为受限于最大放大倍数。
	// 其它放大倍数的图将由这张图生成。
	// 这么做是因为发现IImage draw速度比较慢，连续放大时，不如这种方式性能好，缺点是多耗了些内存
	CWceUiGenericBitmap *pbmpImageLargest;
	// 根据图片大小(image的大小，如图片太大有可能是调整后的尺寸)及输出屏幕的大小
	// 得到的最小比率，缩小时不能小过此比率. 如果图片比屏幕小，这个值应为1，如果较大
	// 按此值输出时，图片刚好落在屏幕内
	double minRatio;
	// 这个值为minRatio的二倍，放大倍数太大没意义，如果图片比较大时，可能无法完成该操作.
	double maxRatio;
	// 当前的放大比例
	double curRatio;
	// 当前显示部分的中心点坐标(在位图中所处的位置)，拖动时该值会变化
	POINT ptCenter;
	// 当前的输出大小，放大缩小操作会引起该值变化，初始值应为刚好输出到
	// 屏幕大小，并保持了纵横比
	// SIZE szDst;
	// 图片的全路径文件名
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
	// 目录变化了，会调用此方法
	void OnFileListChange();

	void SetFullScreen(BOOL bFullScreen);
	BOOL GetFullScreen() {return m_bFullScreen;}
	void PlaySlide();
	void StopPlaySlide();
	BOOL IsPlayingSlide();
	void Rotate();
	void ZoomIn();
	void ZoomOut();
	
	// 提供给遥控器的接口,单按ZOOM键可以循环放大和缩小
	// 返回值表示的是当前执行的操作, TRUE ~ 放大， FALSE ~ 缩小
	BOOL Zoom();
	BOOL m_bZoom;	// 用来记录上一次放大或缩小的动作, TRUE ~ 放大， FALSE ~ 缩小

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

	// 控件自己记录的当前播放的索引
	// 因为控件可以自己滑动到下一页，此时与父窗口的索引不一致，通过处理后才会保持一致
	// 所以在某一时刻他们的值可能是不一样的
	int m_nIndexPlaying;

	// 如果可能系统会缓存5个解码的图片，加快显示速度，这里记录了
	// 解码后的image及对应的文件名
	// 总是显示中间的那张，缓存前二张及后二张，如没有就不缓存

	IMAGE_INFO m_image[MAX_IMAGE_BUFFERED];

	// 加载还没加载的需缓存的图片，一次只加载一张，该函数需调用多次才能将所有需加载的加载完毕，他自己决定优先秩序
	// 如果当前的图片换了,需要重新构建image缓存,
	void ReBufferImage();

	// 加载还没加载的需缓存的图片，一次只加载一张，该函数需调用多次才能将所有需加载的加载完毕，他自己决定优先秩序
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
	// 如果加载失败， 显示加载失败的信息
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

	// 创建这个位图，输出图像时，先输出到该位图，OK后，再输出到primary surface
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
