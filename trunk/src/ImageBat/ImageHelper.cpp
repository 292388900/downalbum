
#include "StdAfx.h"
#include "ImageHelper.h"

CImageHelper::CImageHelper(void)
{
}

CImageHelper::~CImageHelper(void)
{
}

//�ο���http://blog.csdn.net/srxumin/article/details/6862826
int CImageHelper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;                     // number of image encoders   
	UINT size = 0;                   // size of the image encoder array in bytes   
	ImageCodecInfo* pImageCodecInfo = NULL;   
	GetImageEncodersSize(&num, &size);   
	if(size == 0)   
		return -1;     //   Failure   

	pImageCodecInfo =  (ImageCodecInfo*)(malloc(size));   
	if(pImageCodecInfo == NULL)   
		return -1;     //   Failure   

	GetImageEncoders(num, size, pImageCodecInfo);   
	for(UINT j = 0; j < num; ++j)   
	{   
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )   
		{   
			*pClsid = pImageCodecInfo[j].Clsid;   
			free(pImageCodecInfo);   
			return j;     //   Success   
		}           
	}   
	free(pImageCodecInfo);   
	return -1;     //   Failure   
}

//��װ����ͼƬ�������ļ�
void CImageHelper::SaveFile(Image *pImage,const wchar_t* pFileName)
{
	EncoderParameters encoderParameters;
	CLSID jpgClsid; GetEncoderClsid(L"image/jpeg", &jpgClsid);
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	// Save the image as a JPEG with quality level 100.
	ULONG             quality;
	quality = 100;
	encoderParameters.Parameter[0].Value = &quality;
	Status status = pImage->Save(pFileName, &jpgClsid, &encoderParameters);
	if (status != Ok) 
	{
		wprintf(L"%d  Attempt to save %s failed.\n", status, pFileName);
	}
}

CString CImageHelper::FormatImageSize(int nSize)
{
	CString strText;
	float fSize = (float)nSize;

	if ( nSize>=0x100000 ){
		fSize = fSize/0x100000;
		strText.Format(_T("%.2f MB"),fSize);
	}else if ( nSize>=0x400 ){
		fSize = fSize/0x400;
		strText.Format(_T("%.2f KB"),fSize);
	}else{
		strText.Format(_T("%d bytes"),nSize);
	}

	return strText;
}

void CImageHelper::AjustClipArea(int nSrcWidth, int nSrcHeight, int&nPosX,int&nPosY, int&nWidth ,int&nHeight)
{
	int nDstX = nPosX;
	int nDstY = nPosY;
	int nDstWidth = 0;
	int nDstHeight = 0;

	if ( nDstX >nSrcWidth ){
		nDstX = 0;        //�����߽��˾ͻָ�Ϊ
	} else if ( nDstX<0 ){
		nDstX = nSrcWidth + nDstX;
		if ( nDstX <0 ){
			nDstX = 0;  // �����߽��˾ͻָ�Ϊ
		}
	}

	if ( nDstY >nSrcHeight ){
		nDstY = 0;        //�����߽��˾ͻָ�Ϊ
	} else if ( nDstY<0 ){
		nDstY = nSrcHeight + nDstY;
		if ( nDstY <0 ){
			nDstY = 0;  // �����߽��˾ͻָ�Ϊ
		}
	}

	//����nDstX ��nDstY��ʱ���� nDstWidth��nDstHeight
	nDstWidth = nSrcWidth - nDstX;
	nDstHeight = nSrcHeight - nDstY;

	if ( nWidth >0 && nWidth< nDstWidth ){
		nDstWidth = nWidth ;
	} else if ( nWidth<0 ){
		nDstWidth += nWidth ;
		if ( nDstWidth <=0 ){
			nDstWidth = nSrcWidth - nDstX;
		}
	}

	if ( nHeight >0 && nHeight< nDstHeight ){
		nDstHeight = nHeight ;
	} else if ( nHeight<0 ){
		nDstHeight += nHeight ;
		if ( nDstHeight <=0 ){
			nDstHeight = nSrcHeight - nDstY;
		}
	}

	nPosX = nDstX;
	nPosY = nDstY;
	nWidth = nDstWidth;
	nHeight = nDstHeight;
}

//���nPosX ��nPosY�� nWidth��nHeight Ϊ����������˵������������Ͻǵ�ƫ�ƣ���Ϊ������Ϊ��������½ǵ�ƫ�ơ�
void CImageHelper::ClipImageFile(LPCTSTR lpszSrcImageFile,LPCTSTR lpszDstImageFile, int nPosX,int nPosY, int nWidth ,int nHeight)
{
	int nSrcWidth = 0;
	int nSrcHeight = 0;
	int nDstX = 0;
	int nDstY = 0;
	int nDstWidth = 0;
	int nDstHeight = 0;

	CImage imgSrc ;
	imgSrc.Load(lpszSrcImageFile);
	nSrcWidth = imgSrc.GetWidth();
	nSrcHeight = imgSrc.GetHeight();

	if ( nDstX >nSrcWidth ){
		nDstX = 0;        //�����߽��˾ͻָ�Ϊ
	} else if ( nDstX<0 ){
		nDstX = nSrcWidth + nDstX;
		if ( nDstX <0 ){
			nDstX = 0;  // �����߽��˾ͻָ�Ϊ
		}
	}

	if ( nDstY >nSrcHeight ){
		nDstY = 0;        //�����߽��˾ͻָ�Ϊ
	} else if ( nDstY<0 ){
		nDstY = nSrcHeight + nDstY;
		if ( nDstY <0 ){
			nDstY = 0;  // �����߽��˾ͻָ�Ϊ
		}
	}

	//����nDstX ��nDstY��ʱ���� nDstWidth��nDstHeight
	nDstWidth = nSrcWidth - nDstX;
	nDstHeight = nSrcHeight - nDstY;

	if ( nWidth >0 && nWidth< nDstWidth ){
		nDstWidth = nWidth ;
	} else if ( nWidth<0 ){
		nDstWidth += nWidth ;
		if ( nDstWidth <=0 ){
			nDstWidth = nSrcWidth - nDstX;
		}
	}

	if ( nHeight >0 && nHeight< nDstHeight ){
		nDstHeight = nHeight ;
	} else if ( nHeight<0 ){
		nDstHeight += nHeight ;
		if ( nDstHeight <=0 ){
			nDstHeight = nSrcHeight - nDstY;
		}
	}

	CImage imgDst;
	imgDst.Create(nDstWidth, nDstHeight,imgSrc.GetBPP());
	imgSrc.BitBlt(imgDst.GetDC(),0,0,nDstWidth ,nDstHeight, nDstX,nDstY ,SRCCOPY);
	imgSrc.Detach();
	if ( GetFileAttributes(lpszDstImageFile)!=-1 ){
		::DeleteFile(lpszDstImageFile );
	}
	imgDst.Save(lpszDstImageFile);
	imgDst.ReleaseDC();
	imgDst.Destroy();
}

CString CImageHelper::AjustTime(const CString&strTime,int nDays,int nHours)
{
	CString strNewTime;
	if ( strTime.GetLength()!=19 ){
		return strNewTime;
	}

	int nYear = StrToInt(strTime.Left(4));
	int nMonth = StrToInt(strTime.Mid(5,2));
	int nDay = StrToInt(strTime.Mid(8,2));
	int nHour = StrToInt(strTime.Mid(11,2));
	int nMin = StrToInt(strTime.Mid(14,2));
	int nSec = StrToInt(strTime.Mid(17,2));

	//�򵥵���֤ʱ���Ƿ���ȷ
	//...
	CTime time(nYear,nMonth,nDay,nHour,nMin,nSec);
	CTimeSpan span(nDays,nHours,0,0);
	time = time + span;
	return time.Format(_T("%Y:%m:%d %H:%M:%S"));
}