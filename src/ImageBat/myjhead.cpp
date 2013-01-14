
#include "stdafx.h"
#include "myjhead.h"


// Bitmasks for DoModify:
#define MODIFY_ANY  1
#define READ_ANY    2
#define JPEGS_ONLY  4
#define MODIFY_JPEG 5
#define READ_JPEG   6



#ifdef _DEBUG
#pragma comment(lib,"jheadD.lib")
#else
#pragma comment(lib,"jhead.lib")
#endif



void DeleteImageFileExif(const char * FileName)
{
	DeleteExif = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	//������Ҫ�ָ����ֵ
	DeleteExif = FALSE;
	DoModify = FALSE;
}

//����ͼƬ��¼ˮƽ��Ϣ�Զ�ת��ͼƬ
void AutoRotateImageFile(const char * FileName)
{
	AutoRotate = 1;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	//������Ҫ�ָ����ֵ
	AutoRotate = FALSE;
	DoModify = FALSE;
}

//����һ��ͼƬ��exif��Ϣ����һ���ļ���
void ImportExif(const char * srcFileName,const char * dstFileName)
{
	ExifXferScrFile = (char *)srcFileName;
	DoModify |= MODIFY_JPEG;

	ProcessFile(dstFileName);

	ExifXferScrFile = NULL;
	DoModify = FALSE;
}

//ɾ����ԭʼexif��Ϣ������Photoshop����ͼ����֮��༭���޸ĵ���Ϣ��
void DeleteImageExifNotOrignal(const char * FileName)
{
	DeleteUnknown = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	DeleteUnknown = FALSE;
	DoModify = FALSE;
}

//ɾ������jpeg�ļ��Ǳ������Ϣ
void ImagePureJpg(const char * FileName)
{
	DeleteExif = TRUE;
	DeleteComments = TRUE;
	DeleteIptc = TRUE;
	DeleteUnknown = TRUE;
	DeleteXmp = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	DeleteExif = FALSE;
	DeleteComments = FALSE;
	DeleteIptc = FALSE;
	DeleteUnknown = FALSE;
	DeleteXmp = FALSE;
	DoModify = FALSE;
}

//ɾ��jpeg��Ϣ�еı�ע
void DeleteImageRemarks(const char * FileName)
{
	DeleteComments = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	DeleteComments = FALSE;
	DoModify = FALSE;
}

//���jpeg��Ϣ�еı�ע
void AddImageRemarks(const char * FileName,const char *szComments)
{
	CommentInsertLiteral = (char *)szComments;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	CommentInsertLiteral = NULL;
	DoModify = FALSE;
}