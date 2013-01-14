

/*
图片操作类，这里实现了图片的各种操作
*/

#pragma once

#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include "myjhead.h"
#include "ControlTracker.h"

#pragma comment(lib, "gdiplus.lib")

//批量重命名所需的参数
typedef struct PROCESS_PARAM_RENAMEIMAGE{
	CString strRenameRules;
	int nStartNumber;
	int nIncreaseStep;
	int nFileExtCaseMethod;			//文件扩展名的处理：不做变化、全部改小写、全部改大写
}*PPROCESS_PARAM_RENAMEIMAGE;


//批量裁剪所需的参数
typedef struct PROCESS_PARAM_CLIPIMAGE{
	int nPosX;
	int nPosY;
	int nWidth;
	int nHeight;
	PROCESS_PARAM_CLIPIMAGE(){
		nPosX = 0;
		nPosY = 0;
		nWidth = 0;
		nHeight = 0;
	}
	PROCESS_PARAM_CLIPIMAGE(PROCESS_PARAM_CLIPIMAGE&a){
		nPosX = a.nPosX;
		nPosY = a.nPosY;
		nWidth = a.nWidth;
		nHeight = a.nHeight;
	}
}*PPROCESS_PARAM_CLIPIMAGE;


//批量校正拍摄时间所需的参数
typedef struct PROCESS_PARAM_UPDATETIME{
	int nDays;
	int nHours;
	PROCESS_PARAM_UPDATETIME(){
		nDays = 0;
		nHours = 0;
	}
}*PPROCESS_PARAM_UPDATETIME;

//添加备注信息所需的参数
typedef struct PROCESS_PARAM_ADDCOMMENTS{
	CString strComments;
}*PPROCESS_PARAM_ADDCOMMENTS;

//批量缩小图片所需的参数
typedef struct PROCESS_PARAM_THUMBNAIL{
	int nPixelWidth;
	BOOL bResotreExif;
}*PPROCESS_PARAM_THUMBNAIL;

//批量添加水印所需的参数
typedef struct PROCESS_PARAM_ADDWATERMARKER{
	enum{
		eAlignment_custom		= -1,
		eAlignment_topleft		= 0,
		eAlignment_topmid		= 1,
		eAlignment_righttop		= 2,
		eAlignment_leftmid		= 3,
		eAlignment_midmid		= 4,
		eAlignment_rightmid		= 5,
		eAlignment_leftbottom	= 6,
		eAlignment_bottommid	= 7,
		eAlignment_bottomright	= 8,
	};
	//int nAlignment;				//对齐方式
	//int nPosX;
	//int nPosY;
	//int nWidth;
	//int nHeight;
	BOOL bResotreExif;
	CControlTracker *pControlTracker;
	CString strWatermarkTemplateFile;
}*PPROCESS_PARAM_ADDWATERMARKER;




class CImageHelper
{
public:
	CImageHelper(void);
	~CImageHelper(void);
public:
	enum{
		PROCESSTYPE_RENAMEIMAGE,
		PROCESSTYPE_CLIPIMAGE,
		PROCESSTYPE_ROTATEFLIPIMAGE,
		PROCESSTYPE_UPDATETIME,
		PROCESSTYPE_THUMBNAIL,
		PROCESSTYPE_ADDWATERMARKER,
		PROCESSTYPE_DELETEEXIF,
		PROCESSTYPE_DELETEEXIF_NOT_ORIGINAL,
		PROCESSTYPE_DELETE_REMARKS,
		PROCESSTYPE_ADD_REMARKS,
		PROCESSTYPE_PUREJPG,
		PROCESSTYPE_AUTOROTATE,
	};
public:

	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	//封装：将图片保存至文件
	static void SaveFile(Image *pImage,const wchar_t* pFileName);

	static CString FormatImageSize(int nSize);

	//裁剪图片，应用举例：ClipImageFile("caijian.bmp" ,"caijian2.bmp",0,0,0,-100);
	//如果nPosX ，nPosY， nWidth，nHeight 为或者正数，说明是相对于左上角的偏移，若为负数则为相对于左下角的偏移。
	void AjustClipArea(int nSrcWidth, int nSrcHeight, int&nPosX,int&nPosY, int&nWidth ,int&nHeight);
	void ClipImageFile(LPCTSTR lpszSrcImageFile,LPCTSTR lpszDstImageFile, int nPosX,int nPosY, int nWidth ,int nHeight);

	//为时间strTime进行校正
	static CString AjustTime(const CString&strTime,int nDays,int nHours);
};
