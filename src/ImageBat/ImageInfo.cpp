
#include "stdafx.h"
#include "global.h"
#include "ImageInfo.h"
#include "MainDlg.h"
#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;


//////////////////////////////////////////////////////////////////////////

long CImageItem::m_nRefCnt = 0;
CRITICAL_SECTION CImageItem::m_csLocker;

CImageItem::CImageItem()
{
	bIsExif = FALSE;

	//先初始化全局数据
	if ( InterlockedIncrement(&m_nRefCnt) == 1 ){
		InitializeCriticalSection(&m_csLocker);
	}
}

CImageItem::~CImageItem()
{
	//释放初始化全局数据
	if ( InterlockedDecrement(&m_nRefCnt)==0 ){
		DeleteCriticalSection(&m_csLocker);
	}
}

void CImageItem::LoadImage()
{
	AfxBeginThread(ThreadLoadImage,this);
}

UINT CImageItem::ThreadLoadImage(LPVOID lpParam)
{
	CoInitialize(NULL);
	if ( lpParam ){
		PHOTOINFO *pPhoto = (PHOTOINFO *)lpParam;
		pPhoto->_LoadImage();
	}
	CoUninitialize();
	return 0;
}

void CImageItem::_LoadImage()
{
	CImage imageBack;
	int nImageWith = g_config.nImageViewWidth;
	int nImageHeight = g_config.nImageViewHeight;
	CListCtrl &listCtrl = g_pMainDlg->m_lstPhotos;
	CStringW strFilePathW;
	strFilePathW = strFilePath;
	int nPosX = 0;
	int nPosY = 0;

	Image *pImage = new Image(strFilePathW);
	if ( pImage ){

		float fWidthScale = (float)pImage->GetWidth()/nImageWith;
		float fHeightScale = (float)pImage->GetHeight()/nImageHeight;
		float fScale = fWidthScale;
		if ( fScale<fHeightScale ){
			fScale = fHeightScale;
		}

		//因为缩放比例选的是较大的那个，如果为0说明图片的长宽都较小，则使用图片的实际尺寸
		if ( fScale<=1.0 ){
			nImageWith = pImage->GetWidth();
			nImageHeight = pImage->GetHeight();
		}else{
			if ( fWidthScale>fHeightScale ){
				nImageWith = g_config.nImageViewWidth;
				nImageHeight = (int)(pImage->GetHeight()/fScale);
				nPosY = (g_config.nImageViewHeight - nImageHeight) / 2;
			}else{
				nImageWith = (int)(pImage->GetWidth()/fScale);
				nImageHeight = g_config.nImageViewHeight;
				nPosX = (g_config.nImageViewWidth - nImageWith) / 2;
			}
		}

		Image *ThumbnailImage = pImage->GetThumbnailImage(nImageWith,nImageHeight,NULL,NULL);
		if ( ThumbnailImage ){
#if 0
			HDC hDC = ::GetDC(NULL);
			HDC hdcBack = CreateCompatibleDC(hDC);
			HBITMAP hBmp = CreateCompatibleBitmap(hDC, g_config.nImageViewWidth, g_config.nImageViewHeight);
			SelectObject(hdcBack, hBmp);
			::SetStretchBltMode(hdcBack,HALFTONE);	//COLORONCOLOR
			Gdiplus::Graphics graphics(hdcBack);
			graphics.SetInterpolationMode(InterpolationModeHighQuality);
			graphics.DrawImage(ThumbnailImage, nPosX, nPosY, nImageWith, nImageHeight);
			delete ThumbnailImage;

			EnterCriticalSection(&m_csLocker);
			int nImageIndex = ImageList_Add(g_pMainDlg->m_imageList.GetSafeHandle(),hBmp,NULL);
			//int nImageIndex = g_pMainDlg->m_imageList.Add(CBitmap::FromHandle(hBmp),RGB(0,0,0));
			LeaveCriticalSection(&m_csLocker);

			DeleteObject(hBmp);
			DeleteDC(hdcBack);
			::ReleaseDC(NULL,hDC);
#else

			imageBack.Create(g_config.nImageViewWidth,g_config.nImageViewHeight,ILC_COLOR24);
			HDC hdc = imageBack.GetDC();
			::SetStretchBltMode(hdc,HALFTONE);	//COLORONCOLOR
			Gdiplus::Graphics graphics(hdc);
			graphics.SetInterpolationMode(InterpolationModeHighQuality);
			graphics.DrawImage(ThumbnailImage, nPosX, nPosY, nImageWith, nImageHeight);
			imageBack.ReleaseDC();
			delete ThumbnailImage;

			EnterCriticalSection(&m_csLocker);
			int nImageIndex = ImageList_Add(g_pMainDlg->m_imageList.GetSafeHandle(),imageBack.Detach(),NULL);
			LeaveCriticalSection(&m_csLocker);
#endif

			//重新设置图片索引
			LV_ITEM   lvitem;
			lvitem.iItem = nItem;
			lvitem.mask = LVIF_IMAGE;
			listCtrl.GetItem(&lvitem);
			lvitem.iImage = nImageIndex;
			listCtrl.SetItem(&lvitem);
		}

		delete pImage;
	}
}


//void CImageItem::_LoadImage()
//{
//	CImage  image,imageBack;
//	HDC hdc = NULL;
//	CBitmap *pBmp = NULL;
//	int nImageWith = g_pMainDlg->m_nImageWith;
//	int nImageHeight = g_pMainDlg->m_nImageHeight;
//	CListCtrl &listCtrl = g_pMainDlg->m_lstPhotos;
//	
//	if ( SUCCEEDED(image.Load(strFilePath)) ){
//		imageBack.Create(nImageWith,nImageHeight,32);
//		hdc = imageBack.GetDC();
//		::SetStretchBltMode(hdc,HALFTONE);	//COLORONCOLOR
//
//		PatBlt(hdc,0,0,nImageWith,nImageHeight,WHITENESS);
//		//如果相册封面图较小，就不拉伸了直接画在中间，否则才进行拉伸
//		if ( image.GetHeight()>nImageHeight || image.GetWidth()>nImageWith ){
//			image.StretchBlt(hdc,0,0,nImageWith,nImageHeight,SRCCOPY);
//		}else{
//			image.Draw(hdc,(imageBack.GetWidth()-image.GetWidth())/2,
//				(imageBack.GetHeight()-image.GetHeight())/2);
//		}
//		image.Detach();
//		//imageBack.Save(_T("h:\\1.bmp"),Gdiplus::ImageFormatBMP);
//		imageBack.ReleaseDC();
//		pBmp = CBitmap::FromHandle(imageBack.Detach());
//
//		EnterCriticalSection(&m_csLocker);
//		int nImageIndex = g_pMainDlg->m_imageList.Add(pBmp,RGB(0,0,0));
//		LeaveCriticalSection(&m_csLocker);
//
//		//重新设置图片索引
//		LV_ITEM   lvitem;
//		lvitem.iItem = nItem;
//		lvitem.mask = LVIF_IMAGE;
//		listCtrl.GetItem(&lvitem);
//		lvitem.iImage = nImageIndex;
//		listCtrl.SetItem(&lvitem);
//	}else{
//		int a;
//		a++;
//	}
//
//}

//////////////////////////////////////////////////////////////////////////


//旋转图片
CWinThread *CImageItem::RotateFlip(int nRotateFlip)
{
	ROTATEFLIP_PARAM *pParam = new ROTATEFLIP_PARAM;
	pParam->pItem = this;
	pParam->nRotateFlip = nRotateFlip;
	return AfxBeginThread(ThreadRotateFlip,pParam);
}

UINT CImageItem::ThreadRotateFlip(LPVOID lpParam)
{
	CoInitialize(NULL);
	if ( lpParam ){
		ROTATEFLIP_PARAM *pParam = (ROTATEFLIP_PARAM *)lpParam;
		pParam->pItem->_RotateFlip(pParam->nRotateFlip);
		delete lpParam;
	}
	CoUninitialize();
	return 0;
}
void CImageItem::_RotateFlip(int nRotateFlip)
{
	CStringW strFilePathW;
	strFilePathW = this->strFilePath;

	Image *pImage = new Image(strFilePathW,FALSE);
	if ( pImage ){
		pImage->RotateFlip((Gdiplus::RotateFlipType)nRotateFlip);
		CImageHelper::SaveFile(pImage,strFilePathW);
		this->LoadImage();
		delete pImage;
	}

	AppendTextV("已旋转第 %d 张图片：%s",nItem+1,strName);
}

//校正时间
CWinThread *CImageItem::AjustTime(PPROCESS_PARAM_UPDATETIME pParam)
{
	PAJUSTTIME_PARAM lpParam = new AJUSTTIME_PARAM;
	lpParam->pItem = this;
	lpParam->nDays = pParam->nDays;
	lpParam->nHours = pParam->nHours;
	return AfxBeginThread(ThreadAjustTime,lpParam);
}

UINT CImageItem::ThreadAjustTime(LPVOID lpParam)
{
	if ( lpParam ){
		PAJUSTTIME_PARAM pParam = (PAJUSTTIME_PARAM)lpParam;
		CoInitialize(NULL);
		pParam->pItem->_AjustTime(pParam);
		delete lpParam;
		CoUninitialize();
	}
	return 0;
}

void CImageItem::_AjustTime(LPVOID lpParam)
{
	EXIFINFO exifinfo = {0};
	Cexif exif(&exifinfo);

	PAJUSTTIME_PARAM pParam = (PAJUSTTIME_PARAM)lpParam;

	exif.OpenFile(strFilePath);
	exif.DecodeExif();
	CString strTime = CImageHelper::AjustTime(exifinfo.DateTime,pParam->nDays,pParam->nHours);
	if ( strTime.GetLength()==19 ){
		this->strPhotoTime = strTime;
		exif.UpdateOriginalDateTime(strTime);
	}
	exif.CloseFile();

	AppendTextV("已校正第 %d 张图片：%s",nItem+1,strName);
}

//缩小图片
CWinThread *CImageItem::Thumbnail(PPROCESS_PARAM_THUMBNAIL pParam)
{
	PTHUMBNAIL_PARAM lpParam = new THUMBNAIL_PARAM;
	lpParam->pItem = this;
	lpParam->nPixelWidth = pParam->nPixelWidth;
	lpParam->bRestoreExif = pParam->bResotreExif;
	return AfxBeginThread(ThreadThumbnail,lpParam);
}

UINT CImageItem::ThreadThumbnail(LPVOID lpParam)
{
	if ( lpParam ){
		PTHUMBNAIL_PARAM pParam = (PTHUMBNAIL_PARAM)lpParam;
		CoInitialize(NULL);
		pParam->pItem->_Thumbnail(pParam);
		delete lpParam;
		CoUninitialize();
	}
	return 0;
}

void CImageItem::_Thumbnail(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return;
	}

	PTHUMBNAIL_PARAM pParam = (PTHUMBNAIL_PARAM)lpParam;
	if ( nWidth<=pParam->nPixelWidth && nHeight<=pParam->nPixelWidth ){
		//长和宽都小于指定尺寸，不处理。
		return;
	}

	Bitmap *desiredBitmap = NULL;
	BSTR lpszFilePathW = strFilePath.AllocSysString();
	Image image(lpszFilePathW);
	SysFreeString(lpszFilePathW);

	int nNewWidth = nWidth;
	int nNewHeight = nHeight;

	double fScaleX = (double)nWidth / pParam->nPixelWidth;
	double fScaleY = (double)nHeight / pParam->nPixelWidth;
	double fScale = fScaleX;

	if ( nWidth > nHeight ){
		nNewWidth = pParam->nPixelWidth;
		nNewHeight = (int)((double)nHeight * pParam->nPixelWidth / nWidth);
	}else{
		nNewHeight = pParam->nPixelWidth;
		nNewWidth = (int)((double)nWidth * pParam->nPixelWidth / nHeight);
	}


	desiredBitmap = new Bitmap(nNewWidth, nNewHeight,PixelFormat24bppRGB );
	desiredBitmap->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

	Graphics *g = Graphics::FromImage( desiredBitmap );
	Color colorW(255, 255, 255, 255);
	g->Clear( colorW );
	g->SetInterpolationMode( InterpolationModeHighQualityBicubic );
	g->DrawImage( &image, Rect(0, 0, nNewWidth, nNewHeight) );

	lpszFilePathW = (g_config.strSavePath + strName).AllocSysString();
	CImageHelper::SaveFile(desiredBitmap, lpszFilePathW);
	SysFreeString(lpszFilePathW);

	if ( pParam->bRestoreExif ){
		::ImportExif(this->strFilePath,g_config.strSavePath + strName);
	}

	delete g;
	delete desiredBitmap;

	AppendTextV("已缩小第 %d 张图片：%s",nItem+1,strName);
}

//添加水印
CWinThread *CImageItem::AddWaterMarker(PPROCESS_PARAM_ADDWATERMARKER pParam)
{
	PADDWATERMARKER_PARAM lpParam = new ADDWATERMARKER_PARAM;
	lpParam->pItem = this;
	lpParam->bRestoreExif = pParam->bResotreExif;
	lpParam->pControlTracker = pParam->pControlTracker;
	return AfxBeginThread(ThreadThumbnail,lpParam);
}

UINT CImageItem::ThreadAddWaterMarker(LPVOID lpParam)
{
	if ( lpParam ){
		PADDWATERMARKER_PARAM pParam = (PADDWATERMARKER_PARAM)lpParam;
		CoInitialize(NULL);
		pParam->pItem->_Thumbnail(pParam);
		delete lpParam;
		CoUninitialize();
	}
	return 0;
}

void CImageItem::_AddWaterMarker(LPVOID lpParam)
{
	if ( lpParam==NULL ){
		return;
	}

	PADDWATERMARKER_PARAM pParam = (PADDWATERMARKER_PARAM)lpParam;
	//if ( nWidth<=pParam->nPixelWidth && nHeight<=pParam->nPixelWidth ){
	//	//长和宽都小于指定尺寸，不处理。
	//	return;
	//}

	Bitmap *desiredBitmap = NULL;
	BSTR lpszFilePathW = strFilePath.AllocSysString();
	Image image(lpszFilePathW);
	SysFreeString(lpszFilePathW);

	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();

	CWaterMarkersHeader *pWaterMarkHdr = (CWaterMarkersHeader *)&pParam->pControlTracker->m_header;

	//是否添加边框
	if ( pWaterMarkHdr->m_nFrameStyle ){
		nWidth += pWaterMarkHdr->m_nFrameLeftWidth + pWaterMarkHdr->m_nFrameRightWidth;
		nHeight += pWaterMarkHdr->m_nFrameTopHeight + pWaterMarkHdr->m_nFrameBottomHeight;
	}

	desiredBitmap = new Bitmap(nWidth, nHeight,PixelFormat32bppARGB );
	desiredBitmap->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

	Graphics *g = Graphics::FromImage(desiredBitmap);
	g->SetInterpolationMode( InterpolationModeHighQualityBicubic );

	if ( pWaterMarkHdr->m_nFrameStyle ){
		//画背景
		Color clrFrame;
		clrFrame.SetFromCOLORREF(pWaterMarkHdr->m_clrFrame);
		SolidBrush bkBrush(clrFrame);
		g->FillRectangle(&bkBrush,Rect(0, 0, nWidth, nHeight));

		//绘制原图
		g->DrawImage( &image, Rect(pWaterMarkHdr->m_nFrameLeftWidth, pWaterMarkHdr->m_nFrameTopHeight, image.GetWidth(), image.GetHeight()) );
	}else{
		//绘制原图
		g->DrawImage( &image, Rect(0, 0, nWidth, nHeight) );
	}
	
	//假如是居中显示
	//CControlTracker *pControlTracker = pParam->pControlTracker;
	HDC hdc = g->GetHDC();
	pParam->pControlTracker->Draw(hdc,CRect(0,0,nWidth, nHeight),this);
	g->ReleaseHDC(hdc);


	lpszFilePathW = (g_config.strSavePath + strName).AllocSysString();
	CImageHelper::SaveFile(desiredBitmap, lpszFilePathW);
	SysFreeString(lpszFilePathW);

	if ( pParam->bRestoreExif ){
		::ImportExif(this->strFilePath,g_config.strSavePath + strName);
	}

	delete g;
	delete desiredBitmap;

	AppendTextV("已为第 %d 张图片添加水印：%s",nItem+1,strName);
}

//删除exif信息
CWinThread *CImageItem::DeleteExif()
{
	DeleteImageFileExif(this->strFilePath);
	return NULL;
}

//删除非原始exif信息（例如Photoshop，美图秀秀之类编辑后修改的信息）
CWinThread *CImageItem::DeleteExifNotOrignal()
{
	DeleteImageExifNotOrignal(this->strFilePath);
	return NULL;
}

//删除所有jpeg文件非必须的信息
CWinThread *CImageItem::PureJpg()
{
	ImagePureJpg(this->strFilePath);
	return NULL;
}

//删除jpeg信息中的备注
CWinThread *CImageItem::DeleteRemarks()
{
	DeleteImageRemarks(this->strFilePath);
	return NULL;
}

//添加jpeg信息中的备注
CWinThread *CImageItem::AddRemarks(const char *szComments)
{
	AddImageRemarks(this->strFilePath,szComments);
	return NULL;
}

//根据图片记录水平信息自动转动图片
CWinThread *CImageItem::AutoRotate()
{
	DeleteImageFileExif(this->strFilePath);
	return NULL;
}
