
#pragma once

#include "ImageInfo.h"
#include <vector>
using namespace std;




class CFormatTagString
{
	/*指向类的非静态成员函数的指针*/
	typedef CString (CFormatTagString::*TFormatFunction)(ITEMINFO *pItem);
	
	typedef struct TAGINFO 
	{
		CString strTagName;
		TFormatFunction pfnTagFunction;
	};

public:
	CFormatTagString();
	~CFormatTagString();
public:
	void Init(int nCount,const CString&strRules);
	CString GetPreviewText();
	CString GetFormaString(ITEMINFO *pItem);

	void ParaseTags(const CString&strRules);

	//<no>
	CString FormatIndex(ITEMINFO *pItem);

	//<photodate>
	CString FormatPhotodate(ITEMINFO *pItem);

	//<originalname>
	CString FormatOriginalname(ITEMINFO *pItem);

	//<maker>
	CString FormatCameraMaker(ITEMINFO *pItem);

	//<model>
	CString FormatCameraModel(ITEMINFO *pItem);

	//<program>
	CString FormatExposureProgram(ITEMINFO *pItem);

	//<s>
	CString FormatShutterTime(ITEMINFO *pItem);

	//<a>
	CString FormatAperture(ITEMINFO *pItem);

	//<bias>
	CString FormatBias(ITEMINFO *pItem);

	//<iso>
	CString FormatISO(ITEMINFO *pItem);

	//<wb>
	CString FormatWhiteBalance(ITEMINFO *pItem);

	//<flash>
	CString FormatFlashused(ITEMINFO *pItem);

	//<md5>
	CString FormatFileMd5(ITEMINFO *pItem);

private:
	//根据个数动态生成格式化，是%2d %3d 还是%4d
	CString m_strIdexFormat;
	vector<TFormatFunction>m_vtFormatFunction;
	CStringArray m_strArrays;
};
