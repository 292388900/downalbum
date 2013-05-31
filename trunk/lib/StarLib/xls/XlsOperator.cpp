
#include "stdafx.h"
#include "XlsOperator.h"

#pragma comment(lib,"libxl.lib")


CXlsOperator::CXlsOperator()
{
	m_book = NULL;
	m_sheet = NULL;
	m_nTitleCount = 0;
}

CXlsOperator::~CXlsOperator()
{
	if ( m_book!=NULL) {
		m_book->release();
	}
}

bool CXlsOperator::Create()
{
	bool bSuccess = false;
	m_book = xlCreateBook();
	if ( m_book==NULL ) {
		return bSuccess;
	}

	m_sheet = m_book->addSheet(_T("Sheet1"));
	if ( m_sheet==NULL ) {
		m_book->release();
		return bSuccess;
	}

	bSuccess = true;
	return bSuccess;
}

//�������Ŀ��
bool CXlsOperator::SetCol(int colFirst, int colLast, double width, IFormatT<TCHAR>* format/* = 0*/, bool hidden/* = false*/)
{
	bool bRet = false;

	if ( m_sheet!=NULL ) {
		bRet = m_sheet->setCol(colFirst, colLast, width, format, hidden);
	}

	return bRet;
}

//��ӱ��⣺��0�б�ռ���ˣ�Created by LibXL trial version. Please buy the LibXL full version for removing this message.
//�������ǵı���ӵ�1�п�ʼ
bool CXlsOperator::AddTitle(const TCHAR* value, IFormatT<TCHAR>* format/* = 0*/)
{
	bool bRet = false;
	IFormatT<TCHAR>* boldFormat = format;

	if ( m_sheet!=NULL ) {
		if ( boldFormat==NULL ) {
			Font* font = m_book->addFont();
			font->setColor(COLOR_BLACK);
			font->setBold(true);
			boldFormat = m_book->addFormat();
			boldFormat->setFont(font);
			boldFormat->setAlignH(ALIGNH_CENTER);	
		}
		bRet = m_sheet->writeStr(1,m_nTitleCount++,value,boldFormat);
	}

	return bRet;
}

//д����
bool CXlsOperator::WriteStr(int row, int col, const TCHAR* value, IFormatT<TCHAR>* format/* = 0*/)
{
	bool bRet = false;
	Format* alignFormat = format;

	if ( m_sheet!=NULL ) {
		if ( alignFormat==NULL ) {
			//�����������
			Format* alignFormat = m_book->addFormat();
			alignFormat->setAlignH(ALIGNH_CENTER);
		}
		bRet = m_sheet->writeStr(row, col, value, alignFormat);
	}

	return bRet;
}

//��ȡ����
int CXlsOperator::GetRowsCount()
{
	int nCount = 0;

	if ( m_sheet!=NULL ) {
		nCount = m_sheet->lastRow();
	}

	return nCount;
}

//��ȡ����
int CXlsOperator::GetColsCount()
{
	//int nCount = 0;

	//if ( m_sheet!=NULL ) {
	//	nCount = m_sheet->lastCol();
	//}

	//return nCount;
	return m_nTitleCount;
}

bool CXlsOperator::Save(const TCHAR* lpszFileName)
{
	bool bRet = false;

	if ( m_book!=NULL ) {
		bRet = m_book->save(lpszFileName);
	}

	return bRet;
}