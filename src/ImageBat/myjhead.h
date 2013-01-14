

/*
主要对jhead的函数显示声明一下，原jhead库不作任何修改。
*/

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

	#include <jhead/src/jhead.h>

	extern int DeleteExif;

	extern int DoModify;


	extern int FilesMatched;
	extern int FileSequence;

	extern const char * CurrentFile;

	extern const char * progname;   // program name for error messages

	//--------------------------------------------------------------------------
	// Command line options flags
	extern int TrimExif;        // Cut off exif beyond interesting data.
	extern int RenameToDate;        // 1=rename, 2=rename all.
#ifdef _WIN32
	extern int RenameAssociatedFiles;
#endif
	extern char * strftime_args; // Format for new file name.
	extern int Exif2FileTime;
	extern int ShowTags;    // Do not show raw by default.
	extern int Quiet;    // Be quiet on success (like unix programs)
	extern int DumpExifMap;
	extern int ShowConcise;
	extern int CreateExifSection;
	extern char * ApplyCommand;  // Apply this command to all images.
	extern char * FilterModel ;
	extern int    ExifOnly   ;
	extern int    PortraitOnly;
	extern time_t ExifTimeAdjust ;   // Timezone adjust
	extern time_t ExifTimeSet ;      // Set exif time to a value.
	extern char DateSet[11];
	extern unsigned DateSetChars ;
	extern unsigned FileTimeToExif;

	extern int DeleteComments;
	//extern "C" int DeleteExif;
	extern int DeleteIptc;
	extern int DeleteXmp;
	extern int DeleteUnknown;
	extern char * ThumbSaveName ; // If not NULL, use this string to make up
	// the filename to store the thumbnail to.

	extern char * ThumbInsertName ; // If not NULL, use this string to make up
	// the filename to retrieve the thumbnail from.

	extern int RegenThumbnail;

	extern char * ExifXferScrFile ;// Extract Exif header from this file, and
	// put it into the Jpegs processed.

	extern int EditComment;     // Invoke an editor for editing the comment
	extern int SupressNonFatalErrors; // Wether or not to pint warnings on recoverable errors

	extern char * CommentSavefileName ; // Save comment to this file.
	extern char * CommentInsertfileName ; // Insert comment from this file.
	extern char * CommentInsertLiteral ;  // Insert this comment (from command line)

	extern int AutoRotate;
	extern int ZeroRotateTagOnly;

	extern int ShowFileInfo;     // Indicates to show standard file info
	// (file name, file size, file date)


#ifdef MATTHIAS
	// This #ifdef to take out less than elegant stuff for editing
	// the comments in a JPEG.  The programs rdjpgcom and wrjpgcom
	// included with Linux distributions do a better job.

	extern char * AddComment; // Add this tag.
	extern char * RemComment; // Remove this tag
	extern int AutoResize;
#endif // MATTHIAS



	//该函数的实现在jhead.c中，具体修改参数见jhead.c文件main函数的参数解析
	extern "C" void ProcessFile(const char * FileName);

#ifdef __cplusplus
}
#endif






//以下根据ProcessFile的使用方法封装几组函数

//完全删除图片文件的exif信息，参数形式：-de
void DeleteImageFileExif(const char * FileName);

//根据图片记录水平信息自动转动图片
void AutoRotateImageFile(const char * FileName);

//导入一个图片的exif信息到另一个文件中
void ImportExif(const char * srcFileName,const char * dstFileName);

//删除非原始exif信息（例如Photoshop，美图秀秀之类编辑后修改的信息）
void DeleteImageExifNotOrignal(const char * FileName);

//删除所有jpeg文件非必须的信息
void ImagePureJpg(const char * FileName);

//删除jpeg信息中的备注
void DeleteImageRemarks(const char * FileName);

//添加jpeg信息中的备注
void AddImageRemarks(const char * FileName,const char *szComments);