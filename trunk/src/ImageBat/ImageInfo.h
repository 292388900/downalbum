
#pragma once
#include "exif.h"
#include "ImageHelper.h"


//listctrl����Ŀ����
typedef class CImageItem 
{
public:
	CImageItem();
	~CImageItem();

public:
	int		nItem;			//����
	CString	strName;		//����(����չ��)
	CString strFilePath;	//�ļ�ȫ·��
	int		nWidth;			//��Ƭ��
	int		nHeight;		//��Ƭ��
	int		nFileSize;		//�ļ���С
	int		nImageType;		//ͼƬ����
	BOOL	bIsExif;		//�Ƿ���exif��Ϣ
	CString strPhotoTime;	//��������
	CString strCameraMaker;	//������ ָ��Ʒ��������
	CString strCameraModel;	//�ͺ� ָ�豸�ͺ�
	float	fShutterTime;	//�ع�ʱ�� �������ٶ�
	float	fAperture;		//��Ȧϵ��
	float	fBias;			//�عⲹ��
	int		nWhitebalance;	//��Դ ָ��ƽ������
	int		nExposureProgram;//�ع���� ָ����ʽ�Զ��ع�����ã��������ͬ��������Shutter Priority���������ȣ�Tv����Aperture Priority���������ȣ�Av���ȵȡ�
	int		nISOequivalent;	//�й��
	int		nFlashUsed;		//�Ƿ�ʹ������ơ�

public:
	void LoadImage();

	//��תͼƬ
	CWinThread *RotateFlip(int nRotateFlip);

	//У��ʱ��
	CWinThread *AjustTime(PPROCESS_PARAM_UPDATETIME pParam);

	//��СͼƬ
	CWinThread *Thumbnail(PPROCESS_PARAM_THUMBNAIL pParam);

	//���ˮӡ
	CWinThread *AddWaterMarker(PPROCESS_PARAM_ADDWATERMARKER pParam);

	//ɾ��exif��Ϣ
	CWinThread *DeleteExif();

	//ɾ����ԭʼexif��Ϣ������Photoshop����ͼ����֮��༭���޸ĵ���Ϣ��
	CWinThread *DeleteExifNotOrignal();

	//ɾ��jpeg��Ϣ�еı�ע
	CWinThread *DeleteRemarks();

	//ɾ������jpeg�ļ��Ǳ������Ϣ
	CWinThread *PureJpg();

	//���jpeg��Ϣ�еı�ע
	CWinThread *AddRemarks(const char *szComments);

	//����ͼƬ��¼ˮƽ��Ϣ�Զ�ת��ͼƬ
	CWinThread *AutoRotate();

public:
	static UINT ThreadLoadImage(LPVOID lpParam);
	void _LoadImage();

	static UINT ThreadRotateFlip(LPVOID lpParam);
	void _RotateFlip(int nRotateFlip);

	//У��ʱ��
	static UINT ThreadAjustTime(LPVOID lpParam);
	void _AjustTime(LPVOID lpParam);

	//��СͼƬ
	static UINT ThreadThumbnail(LPVOID lpParam);
	void _Thumbnail(LPVOID lpParam);

	//���ˮӡ
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

