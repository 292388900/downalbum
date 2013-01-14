
#pragma once
#include "exif.h"
#include "ImageHelper.h"


//listctrl的项目属性
typedef class CImageItem 
{
public:
	CImageItem();
	~CImageItem();

public:
	int		nItem;			//索引
	CString	strName;		//名称(含扩展名)
	CString strFilePath;	//文件全路径
	int		nWidth;			//照片宽
	int		nHeight;		//照片高
	int		nFileSize;		//文件大小
	int		nImageType;		//图片类型
	BOOL	bIsExif;		//是否有exif信息
	CString strPhotoTime;	//拍摄日期
	CString strCameraMaker;	//生产者 指产品生产厂家
	CString strCameraModel;	//型号 指设备型号
	float	fShutterTime;	//曝光时间 即快门速度
	float	fAperture;		//光圈系数
	float	fBias;			//曝光补偿
	int		nWhitebalance;	//光源 指白平衡设置
	int		nExposureProgram;//曝光程序 指程序式自动曝光的设置，各相机不同，可能是Shutter Priority（快门优先，Tv）、Aperture Priority（快门优先，Av）等等。
	int		nISOequivalent;	//感光度
	int		nFlashUsed;		//是否使用闪光灯。

public:
	void LoadImage();

	//旋转图片
	CWinThread *RotateFlip(int nRotateFlip);

	//校正时间
	CWinThread *AjustTime(PPROCESS_PARAM_UPDATETIME pParam);

	//缩小图片
	CWinThread *Thumbnail(PPROCESS_PARAM_THUMBNAIL pParam);

	//添加水印
	CWinThread *AddWaterMarker(PPROCESS_PARAM_ADDWATERMARKER pParam);

	//删除exif信息
	CWinThread *DeleteExif();

	//删除非原始exif信息（例如Photoshop，美图秀秀之类编辑后修改的信息）
	CWinThread *DeleteExifNotOrignal();

	//删除jpeg信息中的备注
	CWinThread *DeleteRemarks();

	//删除所有jpeg文件非必须的信息
	CWinThread *PureJpg();

	//添加jpeg信息中的备注
	CWinThread *AddRemarks(const char *szComments);

	//根据图片记录水平信息自动转动图片
	CWinThread *AutoRotate();

public:
	static UINT ThreadLoadImage(LPVOID lpParam);
	void _LoadImage();

	static UINT ThreadRotateFlip(LPVOID lpParam);
	void _RotateFlip(int nRotateFlip);

	//校正时间
	static UINT ThreadAjustTime(LPVOID lpParam);
	void _AjustTime(LPVOID lpParam);

	//缩小图片
	static UINT ThreadThumbnail(LPVOID lpParam);
	void _Thumbnail(LPVOID lpParam);

	//添加水印
	static UINT ThreadAddWaterMarker(LPVOID lpParam);
	void _AddWaterMarker(LPVOID lpParam);
	

private:
	static long	m_nRefCnt;
	static CRITICAL_SECTION m_csLocker;

} ITEMINFO,PHOTOINFO;


//////////////////////////////////////////////////////////////////////////
typedef struct ROTATEFLIP_PARAM 
{
	ITEMINFO *pItem;
	int nRotateFlip;
}*PROTATEFLIP_PARAM;

typedef struct AJUSTTIME_PARAM 
{
	ITEMINFO *pItem;
	int nDays;
	int nHours;
}*PAJUSTTIME_PARAM;

typedef struct THUMBNAIL_PARAM 
{
	ITEMINFO *pItem;
	int nPixelWidth;
	BOOL bRestoreExif;
}*PTHUMBNAIL_PARAM;

typedef struct ADDWATERMARKER_PARAM 
{
	ITEMINFO *pItem;
	BOOL bRestoreExif;
	CControlTracker *pControlTracker;
}*PADDWATERMARKER_PARAM;
//////////////////////////////////////////////////////////////////////////

