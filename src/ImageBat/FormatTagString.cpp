
#include "stdafx.h"
#include "FormatTagString.h"



CFormatTagString::CFormatTagString()
{
}

CFormatTagString::~CFormatTagString()
{
}

void CFormatTagString::Init(int nCount,const CString&strRules)
{
	if ( nCount<=0 ){
		nCount = 1;
	}

	m_strIdexFormat.Format("%dd",(int)log10((double)nCount)+1);
	m_strIdexFormat = "%0" + m_strIdexFormat;

	ParaseTags(strRules);
}

void CFormatTagString::ParaseTags(const CString&strRules)
{
	CString strText;
	CString strTag;
	int nTagIndex = -1;
	int nPos1 = 0;
	int nPos2 = 0;
	int nLastTagPos = 0;	//上一个完整标签的结束位置
	int nFormatCnt = 0;

	m_strArrays.RemoveAll();


	TAGINFO tags[] = {
		{ "no", &CFormatTagString::FormatIndex },
		{ "photodate", &CFormatTagString::FormatPhotodate },
		{ "originalname", &CFormatTagString::FormatOriginalname },
		{ "maker", &CFormatTagString::FormatCameraMaker },
		{ "model", &CFormatTagString::FormatCameraModel },
		{ "s", &CFormatTagString::FormatShutterTime },
		{ "a", &CFormatTagString::FormatAperture },
		{ "bias", &CFormatTagString::FormatBias },
		{ "iso", &CFormatTagString::FormatISO },
		{ "program", &CFormatTagString::FormatExposureProgram },
		{ "wb", &CFormatTagString::FormatWhiteBalance },
		{ "flash", &CFormatTagString::FormatFlashused },
	};


	//先把标签取出来
	while ( TRUE ){
		nPos1 = strRules.Find('<',nPos2);
		if ( nPos1==-1 ){
			break;
		}
		nPos2 = strRules.Find('>',nPos1+1);
		if ( nPos2==-1 ){
			break;
		}
		strTag = strRules.Mid(nPos1+1,nPos2-nPos1-1);
		nTagIndex = -1;

		//获取tag的函数
		for ( int i=0; i<sizeof(tags)/sizeof(TAGINFO); ++i ){
			if ( strTag==tags[i].strTagName ){
				m_vtFormatFunction.push_back(tags[i].pfnTagFunction);
				strText = strRules.Mid(nLastTagPos,nPos1-nLastTagPos);
				m_strArrays.Add(strText);
				nLastTagPos = nPos2+1;
				break;
			}
		}//end for

	}//end while

	//从上一个完整标签的结束位置到尾部
	strText = strRules.Mid(nLastTagPos);
	m_strArrays.Add(strText);
}

CString CFormatTagString::GetFormaString(ITEMINFO *pItem)
{
	CString strResult;

	//格式化
	for ( int i = 0; i < (int)m_vtFormatFunction.size(); ++i ){
		strResult += m_strArrays[i] + (this->*(m_vtFormatFunction[i]))(pItem);
	}
	strResult += m_strArrays[m_strArrays.GetCount()-1];

	return strResult;
}

CString CFormatTagString::GetPreviewText()
{
	ITEMINFO stItem;
	stItem.nItem = 1;
	stItem.strName = "圣托里尼岛日照.jpg";
	stItem.strPhotoTime = "2012-11-17 08:10:45";
	stItem.strCameraMaker = "NIKON CORPORATION";
	stItem.strCameraModel = "NIKON D90";
	stItem.fShutterTime = 1;
	stItem.fAperture = (float)5.6;
	stItem.fBias = (float)-0.3;
	stItem.nWhitebalance = 3000;
	stItem.nExposureProgram = 0;	//a,v
	stItem.nISOequivalent = 100;
	stItem.nFlashUsed = 0;

	return GetFormaString(&stItem);
}

//<no>
CString CFormatTagString::FormatIndex(ITEMINFO *pItem){
	CString str;
	str.Format(m_strIdexFormat,pItem->nItem);
	return str;
}

//<photodate>
CString CFormatTagString::FormatPhotodate(ITEMINFO *pItem){
	return pItem->strPhotoTime;
}

//<originalname>
CString CFormatTagString::FormatOriginalname(ITEMINFO *pItem){
	int nPos1 = 0;
	nPos1 = pItem->strName.ReverseFind('.');
	if ( nPos1!=-1 ){
		return pItem->strName.Mid(0,nPos1);
	}
	return pItem->strName;
}

//<maker>
CString CFormatTagString::FormatCameraMaker(ITEMINFO *pItem)
{
	return pItem->strCameraMaker;
}

//<model>
CString CFormatTagString::FormatCameraModel(ITEMINFO *pItem)
{
	return pItem->strCameraModel;
}

//<program>
CString CFormatTagString::FormatExposureProgram(ITEMINFO *pItem)
{
	CString str;
	str.Format("%d",pItem->nExposureProgram);
	return str;
}

//<s>
CString CFormatTagString::FormatShutterTime(ITEMINFO *pItem)
{
	CString str;
	if ( pItem->fShutterTime<1 ){
		int nShutterTime = (int)ceil( 1 / pItem->fShutterTime );
		str.Format("1/%d\'",nShutterTime);
	}else{
		str.Format("%.1f\'",pItem->fShutterTime);
	}
	return str;
}

//<a>
CString CFormatTagString::FormatAperture(ITEMINFO *pItem)
{
	CString str;
	str.Format("%.1f",pItem->fAperture);
	return str;
}

//<bias>
CString CFormatTagString::FormatBias(ITEMINFO *pItem)
{
	CString str;
	str.Format("%.1f",pItem->fBias);
	return str;
}

//<iso>
CString CFormatTagString::FormatISO(ITEMINFO *pItem)
{
	CString str;
	str.Format("%d",pItem->nISOequivalent);
	return str;
}

//<wb>
CString CFormatTagString::FormatWhiteBalance(ITEMINFO *pItem)
{
	CString str;
	str.Format("%d",pItem->nWhitebalance);
	return str;
}

//<flash>
CString CFormatTagString::FormatFlashused(ITEMINFO *pItem)
{
	CString str;
	str.Format("%d",pItem->nFlashUsed);
	return str;
}
