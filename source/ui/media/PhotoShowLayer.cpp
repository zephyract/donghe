#include "stdafx.h"
#include "PhotoShowLayer.h"
#include "PhotoPlayerDlg.h"
#include <ddraw.h>
#include "mainwnd.h"
#include "GImaging.h"
#include "interface.h"
//#include "yccommon.h"

#define SAFE_RELEASE(p) \
	if(p) {(p)->Release();(p)=NULL;}
#define SAFE_DELETE(p) \
	if(p) {delete (p);(p)=NULL;}

static GVOID JPGImageDecodeAsyncProc(GHANDLE hImage, GUINT32 u4Evt,
									 GUINT32 u4CustomData, GUINT32 u4Result)
{
	CPhotoShowLayer *pView = NULL;

	pView = (CPhotoShowLayer *)u4CustomData;
	if (pView)
	{
		pView->OnImageEvent(hImage, u4Evt, u4Result); 
	}
}

CPhotoShowLayer::CPhotoShowLayer(void)
: m_pPlayFileList(NULL)
, m_nIndexPlaying(-1)
, m_bFullScreen(FALSE)
, m_nSlideTime(2500)
, m_bIsSliding(FALSE)
, m_bIsNeedMoving(FALSE)
, m_bPictureMoved(FALSE)
, m_bExitRebuildImageProc(FALSE)
, m_eventRebuildImage()
, m_singleLock(&m_eventRebuildImage)
, m_bZoom(TRUE)
{
	memset(m_image, 0, sizeof(m_image));
	SetExtraScroll(50);
	SetScrollFrame(15);
	//SetScrollFrameTime(50);

	CreateRebuildImageThread();

}


CPhotoShowLayer::~CPhotoShowLayer(void)
{
	DeleteRebuildImageThread();

	for (int i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		// release resource
		if (m_image[i].pbitmap)
		{
			GDestroyBitmap(m_image[i].pbitmap);
			m_image[i].pbitmap = NULL;
		}

		if (m_image[i].pimage)
		{
			GDestroyImageEx(m_image[i].pimage);
			m_image[i].pimage = NULL;
		}

		SAFE_DELETE(m_image[i].pbmpImage);
		SAFE_DELETE(m_image[i].pbmpImageLarge);
		SAFE_DELETE(m_image[i].pbmpImageLargest);
	}

	// clear primary surface
// 	if (::IsWindowVisible(GetWindow()->m_hWnd))
// 	{
// 		HDC hdcPrimary = NULL;
// 		RECT rc = {0,0,WceUiGetScreenWidth(),WceUiGetScreenHeight()};
// 		LPDIRECTDRAWSURFACE lpDDSPrimary = ((CMyWceUiRealWnd*)GetWindow())->m_lpDDSPrimary;
// 		lpDDSPrimary->GetDC(&hdcPrimary);
// 		::FillRect(hdcPrimary, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
// 		lpDDSPrimary->ReleaseDC(hdcPrimary);
// 	}

}

void CPhotoShowLayer::CreateRebuildImageThread()
{
	m_bExitRebuildImageProc = FALSE;
	m_pThreadRebuildImage = AfxBeginThread(CPhotoShowLayer::RebuildImageProc, this, THREAD_PRIORITY_BELOW_NORMAL);
	ASSERT(m_pThreadRebuildImage);
	m_pThreadRebuildImage->m_bAutoDelete = FALSE;
}

void CPhotoShowLayer::DeleteRebuildImageThread()
{
	m_bExitRebuildImageProc = TRUE;
	m_eventRebuildImage.SetEvent();
	WaitForSingleObject(m_pThreadRebuildImage->m_hThread, INFINITE);
	delete m_pThreadRebuildImage;
	m_pThreadRebuildImage = NULL;
}


void CPhotoShowLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{

	__super::SetProperties(pCreateElem);
}

void CPhotoShowLayer::OnInitLayer()
{
	GetStyle()->GetPosition(m_rcLayer);

	OnLayerSizeChanged();

	__super::OnInitLayer();
}

void CPhotoShowLayer::OnLayerSizeChanged()
{
	CRect rc;
	GetClientRect(rc);

	m_cyItemWidth = rc.Width();

	// it allows call CreateDIB() repeatedly, it will release allocated resources automatically.
	//m_bmpSnap.CreateDIB(rc.Width(), rc.Height());

	for (int i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		if (m_image[i].pbmpImage == NULL)
		{
			if (WceUiGetBitsPixel() == 16)
			{
				m_image[i].pbmpImage = new CWceUi565Bitmap();
			}
			else
			{
				m_image[i].pbmpImage = new CWceUi32Bitmap();
			}
		}
		m_image[i].pbmpImage->CreateDIB(rc.Width(), rc.Height());

		// 为了使逻辑简单，只保留当前的图片信息，其它已经缓存了的全部释放掉，重新ReBufferImage()
		// 大小变化后要重新draw(为了保证性能不能一次全部画好), 此时用户有可能切换到其它的图片，
		// 此时IImage的管理变得太复杂了
		if (i == MAX_IMAGE_BUFFERED/2)
		{
			// StretchBlt() is faster than draw, so it has higher priority.
			if (m_image[i].pbmpImageLargest!=NULL && m_image[i].pbmpImageLargest->IsLoaded())
			{
				m_image[i].pbmpImage->StretchBlt(0,0,m_image[i].pbmpImage->GetWidth(),m_image[i].pbmpImage->GetHeight(),
					m_image[i].pbmpImageLargest,
					0,0,m_image[i].pbmpImageLargest->GetWidth(),m_image[i].pbmpImageLargest->GetHeight(),SRCCOPY);
			}
			else
			{
				m_mutexRebuildImage.Lock();
				DrawImage(m_image[i].pbmpImage, rc, m_image[i].pbitmap);
				m_mutexRebuildImage.Unlock();
			}
		}
		else
		{
			// RebuildImageProc()也在操作该变量，保证互不影响
			m_mutexRebuildImage.Lock();
			if (m_image[i].pbitmap)
			{
				GDestroyBitmap(m_image[i].pbitmap);
				m_image[i].pbitmap = NULL;
			}

			if (m_image[i].pimage)
			{
				GDestroyImageEx(m_image[i].pimage);
				m_image[i].pimage = NULL;
			}
			m_mutexRebuildImage.Unlock();
		}
	}

	// 重新缓冲, 缓冲前将当前图片的相关资料保存起来，防止缓冲时会清除这些资料
	CWceUiGenericBitmap* pbmpLarge = m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLarge;
	CWceUiGenericBitmap* pbmpLargest = m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLargest;

	m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLarge = NULL;
	m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLargest = NULL;

	ReBufferImage();

	m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLarge = pbmpLarge;
	m_image[MAX_IMAGE_BUFFERED/2].pbmpImageLargest = pbmpLargest;
}


void CPhotoShowLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	CRect rc_client(0,0,WceUiGetScreenWidth(),WceUiGetScreenHeight());

	CRect rcPaint(rc_client);
	CRect rc_items(rc_client);
	rc_items.left -= GetScrollPos();
	rc_items.right = rc_items.top + m_cyItemWidth*GetCount();

	// draw items
	int first_visible_items = 0;
	int total_visible_items = 0;

	total_visible_items = GetVisibleItems(&first_visible_items, rc_client, rcPaint);
	for (int i=first_visible_items; i<first_visible_items+total_visible_items; i++)
	{
		CRect rc_item(rc_items);
		rc_item.left = rc_items.left+i*m_cyItemWidth;
		rc_item.right = rc_item.left + m_cyItemWidth;
		CRect rc_need_draw;
		rc_need_draw.IntersectRect(rc_item, rcPaint);

		CWceUiGenericBitmap* pImage = NULL;
		POINT ptCenterImage;
		for (int j=0; j<MAX_IMAGE_BUFFERED; j++)
		{
			if (m_pPlayFileList->GetAt(i) == m_image[j].strFileName)
			{
				if (m_image[j].pbmpImageLarge != NULL && m_image[j].pbmpImageLarge->IsLoaded())
				{
					pImage = m_image[j].pbmpImageLarge;
					ptCenterImage = m_image[j].ptCenter;
				}
				else
				{
					pImage = m_image[j].pbmpImage;
					ptCenterImage.x = pImage->GetWidth()/2;
					ptCenterImage.y = pImage->GetHeight()/2;
				}
				break;
			}
		}

		if (pImage)
		{
			CRect rc_src(0,0,pImage->GetWidth(),pImage->GetHeight());
			CRect rc_dst(rc_need_draw);

			// 图片的矩形区rc_src必须与rc_item处于同一个坐标体系，而且要根据ptCenter的位置计算其落在rc_item中的位置，
			// 如果没有放大，应该居中显示
			// 算法：将二个矩形区的中心点重合就满足要求了，坐标体系以rc_item的为准
			POINT ptCenterItem;
			ptCenterItem.x = (rc_item.left+rc_item.right)/2;
			ptCenterItem.y = (rc_item.top+rc_item.bottom)/2;
			rc_src.OffsetRect(ptCenterItem.x-ptCenterImage.x, ptCenterItem.y-ptCenterImage.y);

			// 此时rc_src与rc_dst处于同一坐标体系，他们相交的部分即为需要输出的部分
			rc_dst.IntersectRect(rc_dst, rc_src);

			pWndBitmap->BitBlt(rc_dst.left, rc_dst.top, rc_dst.Width(), rc_dst.Height(), pImage,
				rc_dst.left-rc_src.left,	// 将需要画的部分的坐标调整为图片自己的坐标体系
				rc_dst.top-rc_src.top, SRCCOPY);
// 			pImage->BltToDc(hdcVideo, rc_dst.left, rc_dst.top, rc_dst.Width(), rc_dst.Height(),
// 				rc_dst.left-rc_src.left,	// 将需要画的部分的坐标调整为图片自己的坐标体系
// 				rc_dst.top-rc_src.top, SRCCOPY);
		} 
		else
		{
			TRACE(L"pimage is null. \n");
		}
	}

	__super::OnDraw(pWndBitmap, rcNeedDraw);
}

int CPhotoShowLayer::GetCount()
{
	return m_pPlayFileList->GetCount();
}

int CPhotoShowLayer::GetVisibleItems(OUT int *pnIndexStart, IN CRect &rcClient, IN CRect& rcPaint)
{
	CRect rc_items(rcClient);
	rc_items.left -= GetScrollPos();
	rc_items.right = rc_items.left + GetCount()*m_cyItemWidth;
	CRect rc_visble;
	rc_visble.IntersectRect(rcPaint, rc_items);

	if (rc_visble.IsRectEmpty())
	{
		return 0;
	}

	int first, last;
	first = (rc_visble.left-rc_items.left) / m_cyItemWidth;
	last = (rc_visble.right-rc_items.left-1) / m_cyItemWidth;
	if (pnIndexStart)
		*pnIndexStart = first;

	return (last-first+1);
}

void CPhotoShowLayer::OnFileListChange()
{
	m_nIndexPlaying = -1;
	ResetScrollRange();
}

void CPhotoShowLayer::ResetScrollRange()
{
	CRect rc;
	GetClientRect(rc);
	int max_range = 0;
	if (m_pPlayFileList)
	{
		max_range = (m_pPlayFileList->GetCount()-1)*rc.Width();
	}
	SetScrollRange(0, max(0, max_range), FALSE);
}

void CPhotoShowLayer::ReBufferImage()
{
	if(m_nIndexPlaying < 0 )
	{
		return;		// 还没指定播放哪一张，没必要缓冲
	}

	m_mutexRebuildImage.Lock();

	// reuse the images that have already been buffered.
	void* pbitmap_temp[MAX_IMAGE_BUFFERED];
	memset(pbitmap_temp, 0, sizeof(pbitmap_temp));

	void* pimage_temp[MAX_IMAGE_BUFFERED];
	memset(pimage_temp, 0, sizeof(pbitmap_temp));

	CWceUiGenericBitmap *pbmpImage[MAX_IMAGE_BUFFERED];
	memset(pbmpImage, 0, sizeof(pbmpImage));

	int index_min = m_nIndexPlaying-MAX_IMAGE_BUFFERED/2;
	int index_max = m_nIndexPlaying+MAX_IMAGE_BUFFERED/2;

	int i = 0;
	int j = 0;
	for (i=index_min; i<=index_max; i++)
	{
		// it must be a valid index.
		if (i>=0 && i<m_pPlayFileList->GetCount())
		{
			// find the image in buffered image list
			for (j=0; j<MAX_IMAGE_BUFFERED; j++)
			{
				if (m_pPlayFileList->GetAt(i) == m_image[j].strFileName)
				{
					// move buffered image to temp list.
					pbitmap_temp[i-index_min] = m_image[j].pbitmap;
					m_image[j].pbitmap = NULL;

					pimage_temp[i-index_min] = m_image[j].pimage;
					m_image[j].pimage = NULL;

					pbmpImage[i-index_min] = m_image[j].pbmpImage;
					break;
				}
			}
		}
	}

	// delete the buffered obsolete image
	for (i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		// release resource
		if (m_image[i].pbitmap != NULL)
		{
			GDestroyBitmap(m_image[i].pbitmap);
			m_image[i].pbitmap = NULL;
		}

		if (m_image[i].pimage != NULL)
		{
			GDestroyImageEx(m_image[i].pimage);
			m_image[i].pimage = NULL;
		}

		// 只对当前图片进行旋转或放大，这些数据不再进行复用
		if (m_image[i].pbmpImageLarge != NULL)
		{
			delete m_image[i].pbmpImageLarge;
			m_image[i].pbmpImageLarge = NULL;
		}

		if (m_image[i].pbmpImageLargest != NULL)
		{
			delete m_image[i].pbmpImageLargest;
			m_image[i].pbmpImageLargest = NULL;
		}
	}

	// save data to image list
	for (i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		memcpy(&m_image[i].pbitmap, &pbitmap_temp[i], sizeof(void*));
		memcpy(&m_image[i].pimage, &pimage_temp[i], sizeof(void*));
	}

	// save bmp image 
	for (i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		for (j=0; j<MAX_IMAGE_BUFFERED; j++)
		{
			if(m_image[i].pbmpImage == pbmpImage[j])
				break;
		}

		if (j>=MAX_IMAGE_BUFFERED)	// not found it 
		{
			for (int k=0; k<MAX_IMAGE_BUFFERED; k++)
			{
				if(pbmpImage[k] == NULL)
				{
					pbmpImage[k] = m_image[i].pbmpImage;  // 放在一个没数据的地方就可以了

					// clean the bitmap
					CDC dc;
					dc.CreateCompatibleDC(NULL);
					LPBYTE pbb;
					HBITMAP holdbmp = (HBITMAP)dc.SelectObject(pbmpImage[k]->GetHBitmap(pbb));
					dc.FillSolidRect(0, 0, pbmpImage[k]->GetWidth(), pbmpImage[k]->GetHeight(), RGB(0,0,0));
					dc.SelectObject(holdbmp);
					break;
				}
			}
		}
	}
	for (i=0; i<MAX_IMAGE_BUFFERED; i++)
	{
		m_image[i].pbmpImage = pbmpImage[i];
	}

	// save image file name
	for (i=index_min; i<=index_max; i++)
	{
		// it must be a valid index.
		if (i>=0 && i<m_pPlayFileList->GetCount())
		{
			_tcscpy(m_image[i-index_min].strFileName, m_pPlayFileList->GetAt(i));
		}
		else
		{
			m_image[i-index_min].strFileName[0] = _T('\0');
		}
	}

	// if the one the need to display is not in the buffered list, create it now.
	if (m_image[MAX_IMAGE_BUFFERED/2].pbitmap == NULL)
	{
		RebuildImage();
	}
	//SetTimer(ID_TIMER_REBUILD_IMAGE, 10);
	m_eventRebuildImage.SetEvent();

	m_mutexRebuildImage.Unlock();
}

void CPhotoShowLayer::OnSelectChange()
{
	ReBufferImage();

	CRect rc;
	GetClientRect(rc);
	InvalidateRect(NULL);
	SetScrollPos(rc.Width()*m_nIndexPlaying);
}

UINT CPhotoShowLayer::RebuildImageProc(LPVOID lpData)
{
	CPhotoShowLayer* pthis = (CPhotoShowLayer*)lpData;
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	while (1)
	{
		if (!pthis->m_singleLock.IsLocked())
		{
			pthis->m_singleLock.Lock();
		}

		if (pthis->m_bExitRebuildImageProc)
		{
			break;
		}

		while (1)
		{
			pthis->m_mutexRebuildImage.Lock();
			BOOL bRet = pthis->RebuildImage();
			pthis->m_mutexRebuildImage.Unlock();
			if(!bRet)
			{
				break;
			}			
		}

		pthis->m_singleLock.Unlock();
	}

	CoUninitialize();

	return 0;
}

BOOL CPhotoShowLayer::RebuildImage()
{
	int index = MAX_IMAGE_BUFFERED/2;  // current showing image's index
	int index_to_load = -1;
	if (m_image[index].pbitmap==NULL && m_image[index].strFileName[0]!=L'\0')
	{
		index_to_load = index;
	} 
	else if (index>0 && m_image[index+1].pbitmap==NULL && m_image[index+1].strFileName[0]!=L'\0')
	{
		index_to_load = index+1;
	}
	else if (index>0 && m_image[index-1].pbitmap==NULL && m_image[index-1].strFileName[0]!=L'\0')
	{
		index_to_load = index-1;
	}
	else if (index>1 && m_image[index+2].pbitmap==NULL && m_image[index+2].strFileName[0]!=L'\0')
	{
		index_to_load = index+2;
	}
	else if (index>1 && m_image[index-2].pbitmap==NULL && m_image[index-2].strFileName[0]!=L'\0')
	{
		index_to_load = index-2;
	}
	else
	{
		//KillTimer(ID_TIMER_REBUILD_IMAGE);
		return FALSE;
	}

	if (index_to_load>=0)
	{
		CRect rc;
		GetClientRect(rc);
		if (LoadImageFromFile(m_image[index_to_load].strFileName, &m_image[index_to_load].pimage, &m_image[index_to_load].pbitmap))
		{
			DWORD dwTick = GetTickCount();
			DrawImage(m_image[index_to_load].pbmpImage, rc, m_image[index_to_load].pbitmap);
			// 			m_image[index_to_load].pbmpImage->SaveFile(L"\\App disk\\s.bmp");
			TRACE(L"DrawImage tick = %d, %s\n", GetTickCount()-dwTick, m_image[index_to_load].strFileName);
		}
		else
		{
			DrawErrorInfo(m_image[index_to_load].pbmpImage, rc);
			TRACE(L"LoadImageFromFile() fail. index_to_load = %d, path=%s\n", index_to_load, m_image[index_to_load].strFileName);
			return FALSE;
		}
	}

	return TRUE;
}


void CPhotoShowLayer::ResizeImage()
{
	int index = MAX_IMAGE_BUFFERED/2;  // current showing image's index
	int index_to_load = -1;
	CRect rc;
	GetClientRect(rc);
	if (m_image[index].pbitmap!=NULL && m_image[index].pbmpImage->GetWidth()!=rc.Width())
	{
		index_to_load = index;
	} 
	else if (m_image[index+1].pbitmap!=NULL && m_image[index+1].pbmpImage->GetWidth()!=rc.Width())
	{
		index_to_load = index+1;
	}
	else if (m_image[index-1].pbitmap!=NULL && m_image[index-1].pbmpImage->GetWidth()!=rc.Width())
	{
		index_to_load = index-1;
	}
	else if (m_image[index+2].pbitmap!=NULL && m_image[index+2].pbmpImage->GetWidth()!=rc.Width())
	{
		index_to_load = index+2;
	}
	else if (m_image[index-2].pbitmap!=NULL && m_image[index-2].pbmpImage->GetWidth()!=rc.Width())
	{
		index_to_load = index-2;
	}
	else
	{
		KillTimer(ID_TIMER_RESIZE_IMAGE);
	}

	if (index_to_load>=0)
	{
		if(m_image[index_to_load].pbmpImage->CreateDIB(rc.Width(), rc.Height()))
		{
			DrawImage(m_image[index_to_load].pbmpImage, rc, m_image[index_to_load].pbitmap);
		}
		else
		{
			TRACE(L"CreateDIB() fail.\n");
		}
	}
}


BOOL CPhotoShowLayer::LimitImageSize(IN OUT IImage** ppimage)
{
	IImagingFactory *pImageFactory = NULL;
	ImageInfo imageInfo;
	IBasicBitmapOps* pbmpOp = NULL; 
	IBitmapImage* pbmpImg = NULL;
	IBitmapImage* pNewbmpImg = NULL;
	IImage* pNewImage = NULL; //Image接口对象;新的IImage图像
	HRESULT hr;
	BOOL bRet = FALSE;

	ASSERT(ppimage && *ppimage);

	//获得原图片的基本信息
	hr = (*ppimage)->GetImageInfo(&imageInfo);
	if (!SUCCEEDED(hr))
	{
		return FALSE;
	}

	// small picture
	if (imageInfo.Width<=(UINT)WceUiGetScreenWidth()*3/2 
		&& imageInfo.Height<=(UINT)WceUiGetScreenHeight()*3/2)
	{
		return TRUE;
	}

	hr = CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (LPVOID*)&pImageFactory);
	if( !SUCCEEDED(hr))
	{
		TRACE(L"CoCreateInstance(CLSID_ImagingFactory...) fail, result = %x\n", hr);
		return FALSE;
	}


	// get bitmap image
	hr = pImageFactory->CreateBitmapFromImage(*ppimage,imageInfo.Width,imageInfo.Height,imageInfo.PixelFormat,InterpolationHintDefault,&pbmpImg);
	if (!SUCCEEDED(hr))
	{
		TRACE(L"CreateBitmapFromImage() fail, result = %x\n", hr);
		goto EXIT;
	}


	hr = pbmpImg->QueryInterface(IID_IBasicBitmapOps,(void **)&pbmpOp);
	if (!SUCCEEDED(hr))
	{
		TRACE(L"QueryInterface(IID_IBasicBitmapOps...) fail, result = %x\n", hr);
		goto EXIT;
	}

	// resize
	double ratio = GetZoomRate(imageInfo.Width, imageInfo.Height, WceUiGetScreenWidth()*3/2, WceUiGetScreenHeight()*3/2);
	int width = (int)(imageInfo.Width*ratio);
	int heigth = (int)(imageInfo.Height*ratio);

	hr = pbmpOp->Resize(width, heigth, imageInfo.PixelFormat, InterpolationHintDefault, &pNewbmpImg);
	if (!SUCCEEDED(hr))
	{
		TRACE(L"Resize() fail, result = %x\n", hr);
		goto EXIT;
	}

	hr = pNewbmpImg->QueryInterface(IID_IImage,(void **)&pNewImage);
	if (!SUCCEEDED(hr))
	{
		TRACE(L"QueryInterface(IID_IImage...) fail, result = %x\n", hr);
		goto EXIT;
	}

	bRet = TRUE;

EXIT:
	SAFE_RELEASE(pbmpImg);
	SAFE_RELEASE(pNewbmpImg);
	SAFE_RELEASE(pbmpOp);
	SAFE_RELEASE(pImageFactory);
	SAFE_RELEASE(pbmpImg);

	if (bRet)
	{
		SAFE_RELEASE(*ppimage);
		*ppimage = pNewImage;
	}
	else
	{
		SAFE_RELEASE(pNewImage);
	}

	return bRet;
}


BOOL CPhotoShowLayer::LoadImageFromFile(LPCTSTR lpszFilePath, OUT GHANDLE* phImage, OUT GHANDLE* phBitmap)
{
	BOOL bRet = FALSE;

	// create image
	*phImage = GCreateImageFromFile((GTCHAR *)lpszFilePath);

	GIMAGEINFO_T ii;
	memset(&ii, 0, sizeof(ii));
	GGetImageInfoEx(*phImage, &ii);

	// resize
	const int LIMIT_WIDTH = WceUiGetScreenWidth()*3/2;
	const int LIMIT_HEIGHT = WceUiGetScreenHeight()*3/2;
	double ratio = GetZoomRate(ii.u4Width, ii.u4Height, LIMIT_WIDTH, LIMIT_HEIGHT);
	int width = (int)(ii.u4Width*ratio);
	int heigth = (int)(ii.u4Height*ratio);

	if (*phImage != NULL)
	{
		UINT32 pixel_format = (WceUiGetBitsPixel() == 16) ? PixelFormat16bppRGB565 : PixelFormat32bppRGB;
		if (GDecodeImage(*phImage, width, heigth, JPGImageDecodeAsyncProc, (GUINT32)this, pixel_format))
		{
			while (!GIsImageDecoded(*phImage))
			{
				Sleep(20);
			}

			GHANDLE hBitmapSink = GGetBitmapImageSink(*phImage);
			*phBitmap = GCloneBitmap(hBitmapSink);
			bRet = (*phBitmap != NULL);
		}
	}

	return bRet;
}

double CPhotoShowLayer::GetZoomRate(int cxSrc, int cySrc, int cxDst, int cyDst)
{
	double rx = (double)cxDst/(double)cxSrc;
	double ry = (double)cyDst/(double)cySrc;
	double rate = min(rx, ry);
	rate = min(rate, 1.0);
	return rate;
}

SIZE CPhotoShowLayer::GetImageSize(GHANDLE hImage)
{
	SIZE size = {0,0};
	GIMAGEINFO_T ii;
	//if( GGetImageInfoEx(hImage, &ii) )
	if( GGetBitmapInfo(hImage, &ii) )
	{
		size.cx = ii.u4Width;
		size.cy = ii.u4Height;
	}
	return size;
}

#include "wceuiloadfont.h"
void CPhotoShowLayer::DrawErrorInfo(CWceUiGenericBitmap* pbmp, CRect rcDst)
{
// 	CWceUiLoadString err_info;
// 	CWceUiLoadFont font;
// 	err_info.SetTextResID(L"DVP_LOAD_PIC_FAIL");
// 	font.SetFontFromRes(L"Tahoma-21.font");
// 
// 	CDC dc;
// 	dc.CreateCompatibleDC(NULL);
// 	LPBYTE pbb;
// 	dc.SelectObject(pbmp->GetHBitmap(pbb));
// 	dc.SelectObject(font.GetFont());
// 	dc.SetTextColor(RGB(255,255,255));
// 	dc.SetBkMode(TRANSPARENT);
// 	dc.DrawText(err_info.GetString(), -1, rcDst, DT_CENTER|DT_VCENTER);
}

BOOL CPhotoShowLayer::DrawImage(CWceUiGenericBitmap* pbmp, CRect rcDst, GHANDLE hImage)
{
	if (pbmp==NULL || hImage==NULL)
	{
		return FALSE;
	}

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	LPBYTE pbb;
	dc.SelectObject(pbmp->GetHBitmap(pbb));

	// black color background
	dc.FillSolidRect(rcDst, RGB(0,0,0));
	rcDst.DeflateRect(5, 0);

	SIZE size = GetImageSize(hImage);

	// 保持正确的纵横比
	double rate = GetZoomRate(size.cx, size.cy, rcDst.Width(), rcDst.Height());
	size.cx = (int)((double)size.cx*rate);
	size.cy = (int)((double)size.cy*rate);

	// 居中
	size.cx = (rcDst.Width() - min(rcDst.Width(), (int)size.cx))/2;
	size.cy = (rcDst.Height() - min(rcDst.Height(), (int)size.cy))/2;
	rcDst.DeflateRect(size);

	GDrawBitmapDC(dc.GetSafeHdc(), hImage, rcDst);

	return TRUE;
}

void CPhotoShowLayer::OnTimer(WORD wIDEvent)
{
	switch(wIDEvent)
	{
	case ID_TIMER_REBUILD_IMAGE:
		RebuildImage();
		break;
	case ID_TIMER_RESIZE_IMAGE:
		ResizeImage();
		break;
	case ID_TIMER_SHOWINVILIDICON:
		{
			KillTimer(ID_TIMER_SHOWINVILIDICON);
			CPhotoPlayerDlg* pdlg = (CPhotoPlayerDlg*)GetParent();
			if (pdlg)
			{
				pdlg->ShowInvalidIcon();
			}
		}
		break;
	case ID_TIMER_SLIDE:
		if (m_nIndexPlaying>=0 && m_nIndexPlaying<GetCount()-1)
		{
			SetScrollFrame(10);
			AnimateScrollWindow(m_cyItemWidth);
		}
		else	// 已播放到最后一个,退出播放状态
		{	
			StopPlaySlide();
			SetFullScreen(FALSE);
		}
		break;
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CPhotoShowLayer::OnScrollWindowEnd()
{
	__super::OnScrollWindowEnd();

	CRect rc;
	GetClientRect(rc);
	if (GetScrollPos() % rc.Width() == 0)
	{
		int index = GetScrollPos() / rc.Width();
		if (index != m_nIndexPlaying)
		{
			m_nIndexPlaying = index;
			OnSelectChange();
			GetCmdReceiver()->OnCommand(UI_CMD_PHOTOSHOW_SELCHANGE, (WPARAM)this, (LPARAM)index);
		}
	}
}

void CPhotoShowLayer::SetFullScreen(BOOL bFullScreen)
{
	if (m_bFullScreen == bFullScreen)
	{
		return;
	}

	m_bFullScreen = bFullScreen;
	GetCmdReceiver()->OnCommand(UI_CMD_PHOTOSHOW_FULLSCREEN, (WPARAM)this, (LPARAM)m_bFullScreen);
}

void CPhotoShowLayer::OnMouseClicked()
{
	SetFullScreen(!m_bFullScreen);
}

void CPhotoShowLayer::OnMouseScrolled()
{
	TRACE(L"mouse scrolled.\n");
}

void CPhotoShowLayer::PlaySlide()
{
	GetCmdReceiver()->OnCommand(UI_CMD_PHOTOSHOW_PLAY_STATUS, (WPARAM)this, (LPARAM)1);

	// 还没有开始播放
	if (m_nIndexPlaying<0)
	{
		return;
	}

	m_bIsSliding = TRUE;
//	SetFullScreen(TRUE);
	SetTimer(ID_TIMER_SLIDE, m_nSlideTime);
}

BOOL CPhotoShowLayer::IsPlayingSlide()
{
	return m_bIsSliding;
}

void CPhotoShowLayer::OnLButtonDown(UINT nFlags, POINT point)
{
	// 还没有开始播放
	if (m_nIndexPlaying<0)
	{
		return;
	}

	int index = MAX_IMAGE_BUFFERED/2;
	if (m_image[index].pbitmap && m_image[index].pbmpImageLarge)
	{
		CRect rc;
		GetClientRect(rc);
		if (m_image[index].pbmpImageLarge->GetWidth() > rc.Width()
			|| m_image[index].pbmpImageLarge->GetHeight() > rc.Height())
		{
			m_ptPrevious = point;
			m_bIsNeedMoving = TRUE;
			m_bPictureMoved = FALSE;

			SetCapture();
			return;
		}
	}

//	StopPlaySlide();

	__super::OnLButtonDown(nFlags, point);
}

void CPhotoShowLayer::StopPlaySlide()
{
	GetCmdReceiver()->OnCommand(UI_CMD_PHOTOSHOW_PLAY_STATUS, (WPARAM)this, (LPARAM)0);

	if (m_bIsSliding)
	{
		KillTimer(ID_TIMER_SLIDE);
		m_bIsSliding = FALSE;
	}
}

void CPhotoShowLayer::OnLButtonUp(UINT nFlags, POINT point)
{
	if (m_bIsNeedMoving)	// just move picture
	{
		if (!m_bPictureMoved)
		{
			OnMouseClicked();	// not moved, switch to full-screen mode or normal mode
		}

		m_bIsNeedMoving = FALSE;
		m_bPictureMoved = FALSE;

		ReleaseCapture();
		return;
	}

	__super::OnLButtonUp(nFlags, point);
}

void CPhotoShowLayer::OnMouseMove(UINT nFlags, POINT point)
{
	if (m_bIsNeedMoving)	// just move picture
	{
		if (m_bPictureMoved || abs(point.x-m_ptPrevious.x)>8 || abs(point.y-m_ptPrevious.y)>8)
		{
			MovePicture(point.x-m_ptPrevious.x, point.y-m_ptPrevious.y);
			m_bPictureMoved = TRUE;
			m_ptPrevious = point;
		}
		return;
	}

	if (m_bIsMouseMoved)	// 该标志是基类在维护
	{
		SetFullScreen(TRUE);	// 图片移动时，自动全屏
	}

	__super::OnMouseMove(nFlags, point);
}

void CPhotoShowLayer::MovePicture(int cx, int cy)
{
	int index = MAX_IMAGE_BUFFERED/2;
	ASSERT (m_image[index].pbitmap && m_image[index].pbmpImageLarge);
	CRect rc;
	GetClientRect(rc);
	int width = m_image[index].pbmpImageLarge->GetWidth();
	int height = m_image[index].pbmpImageLarge->GetHeight();
	if (width>rc.Width())
	{
		m_image[index].ptCenter.x -= cx;
		m_image[index].ptCenter.x = max(rc.Width()/2, m_image[index].ptCenter.x);
		m_image[index].ptCenter.x = min(width-rc.Width()/2, m_image[index].ptCenter.x);
	}

	if (height>rc.Height())
	{
		m_image[index].ptCenter.y -= cy;
		m_image[index].ptCenter.y = max(rc.Height()/2, m_image[index].ptCenter.y);
		m_image[index].ptCenter.y = min(height-rc.Height()/2, m_image[index].ptCenter.y);
	}

	InvalidateRect(NULL);
}

void CPhotoShowLayer::Rotate()
{
	int index = MAX_IMAGE_BUFFERED/2;
	if (m_image[index].pbitmap == NULL)
	{
		return;
	}
	m_mutexRebuildImage.Lock();

	GHANDLE pimage = GRotateBitmap(m_image[index].pbitmap, 90);
	if (pimage != NULL)
	{
		// 如果有放大操作，先释放这些资源
		SAFE_DELETE(m_image[index].pbmpImageLarge);
		SAFE_DELETE(m_image[index].pbmpImageLargest);

		// 保持旋转后的Image
		GDestroyBitmap(m_image[index].pbitmap);
		m_image	[index].pbitmap = pimage;
		CRect rc;
		GetClientRect(rc);
		DrawImage(m_image[index].pbmpImage, rc, m_image[index].pbitmap);
		InvalidateRect();
	}

	m_mutexRebuildImage.Unlock();
}

BOOL CPhotoShowLayer::RotateImage(IN IImage* pimageIn, OUT IImage** ppimageOut)
{
	if (!pimageIn || !ppimageOut)
	{
		return FALSE;
	}

	BOOL bReturn = FALSE;
	ImageInfo imageInfo;
	IImagingFactory *pImageFactory = NULL;
	IBasicBitmapOps *pbmpOp        = NULL; // 旋转操作 
	IBitmapImage    *pbmpImg       = NULL;
	IBitmapImage    *pNewbmpImg    = NULL; // 旋转后的图像 

	if( !SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
		IID_IImagingFactory, (LPVOID*)&pImageFactory)))
	{
		return FALSE;
	}

	if (!SUCCEEDED(pimageIn->GetImageInfo(&imageInfo)))
	{
		goto EXIT;
	}

	// get IBasicBitmapOps interface
	if (!SUCCEEDED(pImageFactory->CreateBitmapFromImage(pimageIn, imageInfo.Width, imageInfo.Height,
		imageInfo.PixelFormat, InterpolationHintDefault, &pbmpImg)))
	{
		goto EXIT;
	}

	if (!SUCCEEDED(pbmpImg->QueryInterface(IID_IBasicBitmapOps, (void **)&pbmpOp)))
	{
		goto EXIT;
	}

	// rotate and generate a new bmp image
	if (!SUCCEEDED(pbmpOp->Rotate(90, InterpolationHintDefault, &pNewbmpImg)))
	{
		goto EXIT;
	}

	// get a new IImage interface
	if (!SUCCEEDED(pNewbmpImg->QueryInterface(IID_IImage,(void **)ppimageOut)))
	{
		goto EXIT;
	}


	bReturn = TRUE;

EXIT:
	SAFE_RELEASE(pbmpImg);
	SAFE_RELEASE(pNewbmpImg);
	SAFE_RELEASE(pbmpOp);
	SAFE_RELEASE(pImageFactory);

	return bReturn;
}

void CPhotoShowLayer::OnImageEvent(GHANDLE hImage, UINT uEvent, UINT uResult)
{
	if (uEvent)
	{
	}
	else
	{
	}
}

void CPhotoShowLayer::InitFileList(CStringArray *paFile, int nIndexPlaying)
{
	m_pPlayFileList = paFile; 
	m_nIndexPlaying = nIndexPlaying;

	ResetScrollRange();
	OnSelectChange();
}

GHANDLE CPhotoShowLayer::GetCurGBitmap()
{
	return m_image[MAX_IMAGE_BUFFERED/2].pbitmap;
}

void CPhotoShowLayer::Next()
{
	int nMin, nMax;
	GetScrollRange(&nMin, &nMax);
	if (GetScrollPos() < nMax)
	{
		AnimateScrollWindow(m_cyItemWidth);
	}
}

void CPhotoShowLayer::Prev()
{
	int nMin, nMax;
	GetScrollRange(&nMin, &nMax);
	if (GetScrollPos() > nMin)
	{
		AnimateScrollWindow(-m_cyItemWidth);
	}
}

void CPhotoShowLayer::PlayItem(int nIndex)
{
	m_bZoom = TRUE;

	int total = m_pPlayFileList->GetCount();
	if (nIndex>=0 && nIndex<total && nIndex!=m_nIndexPlaying)
	{
		AnimateScrollWindow((nIndex-m_nIndexPlaying)*m_cyItemWidth);
	}
}

void CPhotoShowLayer::ZoomIn()
{
	m_bZoom = TRUE;

	int index = MAX_IMAGE_BUFFERED/2;
	if (m_image[index].pbitmap==NULL)
	{
		return;
	}

	GHANDLE pimage = NULL;
	pimage = m_image[index].pbitmap;

	CRect rc;
	GetClientRect(rc);
	SIZE size = GetImageSize(pimage);

	// 还没有放大过，对数据进行初始化
	if (m_image[index].pbmpImageLarge == NULL)
	{
		if (WceUiGetBitsPixel() == 16)
		{
			m_image[index].pbmpImageLarge = new CWceUi565Bitmap();
			m_image[index].pbmpImageLargest = new CWceUi565Bitmap();
		}
		else
		{
			m_image[index].pbmpImageLarge = new CWceUi32Bitmap();
			m_image[index].pbmpImageLargest = new CWceUi32Bitmap();
		}

		ASSERT(m_image[index].pbmpImageLarge && m_image[index].pbmpImageLargest);

		m_image[index].minRatio = GetZoomRate(size.cx, size.cy, rc.Width(), rc.Height());
		m_image[index].maxRatio = max(m_image[index].minRatio*3.0, 1);
		m_image[index].curRatio = m_image[index].minRatio;
		// 
		m_image[index].ptCenter.x = (int)(size.cx*m_image[index].curRatio)/2;
		m_image[index].ptCenter.y = (int)(size.cy*m_image[index].curRatio)/2;
	}

	// If already zoom in to the largest scale, nothing to do.
	if (m_image[index].curRatio >= m_image[index].maxRatio)
	{
		SetTimer(ID_TIMER_SHOWINVILIDICON, 500);
		return;
	}

	// get the enlarged bitmap's size.
	double ratio = m_image[index].curRatio * 1.25f;
	ratio = min(ratio, m_image[index].maxRatio);
	double multi = ratio / m_image[index].curRatio;		// this value may be different with 1.25f
	m_image[index].curRatio  = ratio;

	int width = (int)(size.cx*m_image[index].curRatio);
	int height = (int)(size.cy*m_image[index].curRatio);

	// calculate center point
	m_image[index].ptCenter.x = (int)(m_image[index].ptCenter.x * multi);
	m_image[index].ptCenter.y = (int)(m_image[index].ptCenter.y * multi);


	if (!m_image[index].pbmpImageLargest->IsLoaded())
	{
		int width_max = (int)(size.cx*m_image[index].maxRatio);
		int height_max = (int)(size.cy*m_image[index].maxRatio);
		if (m_image[index].pbmpImageLargest->CreateDIB(width_max, height_max))
		{
			CRect rcDst(0,0,width_max,height_max);
			CDC dc;
			dc.CreateCompatibleDC(NULL);
			LPBYTE pbb = NULL;
			dc.SelectObject(m_image[index].pbmpImageLargest->GetHBitmap(pbb));
			GDrawBitmapDC(dc.GetSafeHdc(), pimage, rcDst);


			// 			HRESULT hr = pimage->Draw(dc.GetSafeHdc(), rcDst, NULL);
			// 			if (!SUCCEEDED(hr))
			// 			{
			// 				TRACE(L"IImage Draw fail. hr = %x\n", hr);
			// 			}
		}
		else
		{
			TRACE(L"Create DIB fail.\n");
		}
	}

	if (m_image[index].pbmpImageLarge->CreateDIB(width, height))
	{
		m_image[index].pbmpImageLarge->StretchBlt(0,0,width,height,m_image[index].pbmpImageLargest,
			0, 0, m_image[index].pbmpImageLargest->GetWidth(), m_image[index].pbmpImageLargest->GetHeight(), SRCCOPY);
	}
	else
	{
		TRACE(L"Create DIB fail.\n");
	}
	InvalidateRect(NULL);
}

void CPhotoShowLayer::ZoomOut()
{
	m_bZoom = FALSE;

	int index = MAX_IMAGE_BUFFERED/2;
	if (m_image[index].pbitmap==NULL)
	{
		return;
	}

	GHANDLE pimage = NULL;
	pimage = m_image[index].pbitmap;

	CRect rc;
	GetClientRect(rc);
	SIZE size = GetImageSize(pimage);

	// 还没有放大过或已经最小了，不允许缩小操作
	if (m_image[index].pbmpImageLarge == NULL
		|| m_image[index].curRatio <= m_image[index].minRatio)
	{
		SetTimer(ID_TIMER_SHOWINVILIDICON, 500);
		return;
	}

	// get the enlarged bitmap.
	double ratio = m_image[index].curRatio / 1.25f;
	ratio = max(ratio, m_image[index].minRatio);
	double multi = ratio / m_image[index].curRatio;		// this value may be different with 1.25f
	m_image[index].curRatio  = ratio;

	int width = (int)(size.cx*m_image[index].curRatio);
	int height = (int)(size.cy*m_image[index].curRatio);

	// calculate center point
	m_image[index].ptCenter.x = (int)(m_image[index].ptCenter.x * multi);
	m_image[index].ptCenter.y = (int)(m_image[index].ptCenter.y * multi);
	// 缩小时需尽量让图片落在显示区域
	m_image[index].ptCenter.x = max(m_image[index].ptCenter.x, rc.Width()/2);
	m_image[index].ptCenter.x = min(m_image[index].ptCenter.x, width-rc.Width()/2);
	m_image[index].ptCenter.x = width<=rc.Width() ? width/2 : m_image[index].ptCenter.x;

	m_image[index].ptCenter.y = max(m_image[index].ptCenter.y, rc.Height()/2);
	m_image[index].ptCenter.y = min(m_image[index].ptCenter.y, height-rc.Height()/2);
	m_image[index].ptCenter.y = height<=rc.Height() ? height/2 : m_image[index].ptCenter.y;

	if (m_image[index].pbmpImageLarge->CreateDIB(width, height))
	{
		m_image[index].pbmpImageLarge->StretchBlt(0,0,width,height,m_image[index].pbmpImageLargest,
			0, 0, m_image[index].pbmpImageLargest->GetWidth(), m_image[index].pbmpImageLargest->GetHeight(), SRCCOPY);
	}
	else
	{
		TRACE(L"Create DIB fail.\n");
	}

	InvalidateRect(NULL);
}


BOOL CPhotoShowLayer::Zoom()
{
	int index = MAX_IMAGE_BUFFERED/2;

	if (m_bZoom)	// 上一次放大,下面的操作也是要放大
	{
		if (m_image[index].pbmpImageLarge != NULL
			&& m_image[index].curRatio >= m_image[index].maxRatio)	// 已经到最大了,缩小操作
		{
			ZoomOut();
		}
		else
		{
			ZoomIn();
		}
	}
	else
	{
		if (m_image[index].curRatio <= m_image[index].minRatio)	// 已经最小了,放大操作
		{
			ZoomIn();
		}
		else
		{
			ZoomOut();
		}
	}

	return m_bZoom;
}
