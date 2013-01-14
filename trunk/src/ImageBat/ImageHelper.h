

/*
ͼƬ�����࣬����ʵ����ͼƬ�ĸ��ֲ���
*/

#pragma once

#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include "myjhead.h"
#include "ControlTracker.h"

#pragma comment(lib, "gdiplus.lib")

//��������������Ĳ���
typedef struct PROCESS_PARAM_RENAMEIMAGE{
	CString strRenameRules;
	int nStartNumber;
	int nIncreaseStep;
	int nFileExtCaseMethod;			//�ļ���չ���Ĵ��������仯��ȫ����Сд��ȫ���Ĵ�д
}*PPROCESS_PARAM_RENAMEIMAGE;


//�����ü�����Ĳ���
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


//����У������ʱ������Ĳ���
typedef struct PROCESS_PARAM_UPDATETIME{
	int nDays;
	int nHours;
	PROCESS_PARAM_UPDATETIME(){
		nDays = 0;
		nHours = 0;
	}
}*PPROCESS_PARAM_UPDATETIME;

//��ӱ�ע��Ϣ����Ĳ���
typedef struct PROCESS_PARAM_ADDCOMMENTS{
	CString strComments;
}*PPROCESS_PARAM_ADDCOMMENTS;

//������СͼƬ����Ĳ���
typedef struct PROCESS_PARAM_THUMBNAIL{
	int nPixelWidth;
	BOOL bResotreExif;
}*PPROCESS_PARAM_THUMBNAIL;

//�������ˮӡ����Ĳ���
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
	//int nAlignment;				//���뷽ʽ
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

	//��װ����ͼƬ�������ļ�
	static void SaveFile(Image *pImage,const wchar_t* pFileName);

	static CString FormatImageSize(int nSize);

	//�ü�ͼƬ��Ӧ�þ�����ClipImageFile("caijian.bmp" ,"caijian2.bmp",0,0,0,-100);
	//���nPosX ��nPosY�� nWidth��nHeight Ϊ����������˵������������Ͻǵ�ƫ�ƣ���Ϊ������Ϊ��������½ǵ�ƫ�ơ�
	void AjustClipArea(int nSrcWidth, int nSrcHeight, int&nPosX,int&nPosY, int&nWidth ,int&nHeight);
	void ClipImageFile(LPCTSTR lpszSrcImageFile,LPCTSTR lpszDstImageFile, int nPosX,int nPosY, int nWidth ,int nHeight);

	//Ϊʱ��strTime����У��
	static CString AjustTime(const CString&strTime,int nDays,int nHours);
};
