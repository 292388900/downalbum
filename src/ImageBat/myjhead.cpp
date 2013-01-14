
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

	//处理完要恢复标记值
	DeleteExif = FALSE;
	DoModify = FALSE;
}

//根据图片记录水平信息自动转动图片
void AutoRotateImageFile(const char * FileName)
{
	AutoRotate = 1;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	//处理完要恢复标记值
	AutoRotate = FALSE;
	DoModify = FALSE;
}

//导入一个图片的exif信息到另一个文件中
void ImportExif(const char * srcFileName,const char * dstFileName)
{
	ExifXferScrFile = (char *)srcFileName;
	DoModify |= MODIFY_JPEG;

	ProcessFile(dstFileName);

	ExifXferScrFile = NULL;
	DoModify = FALSE;
}

//删除非原始exif信息（例如Photoshop，美图秀秀之类编辑后修改的信息）
void DeleteImageExifNotOrignal(const char * FileName)
{
	DeleteUnknown = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	DeleteUnknown = FALSE;
	DoModify = FALSE;
}

//删除所有jpeg文件非必须的信息
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

//删除jpeg信息中的备注
void DeleteImageRemarks(const char * FileName)
{
	DeleteComments = TRUE;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	DeleteComments = FALSE;
	DoModify = FALSE;
}

//添加jpeg信息中的备注
void AddImageRemarks(const char * FileName,const char *szComments)
{
	CommentInsertLiteral = (char *)szComments;
	DoModify |= MODIFY_JPEG;

	ProcessFile(FileName);

	CommentInsertLiteral = NULL;
	DoModify = FALSE;
}