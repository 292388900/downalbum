
#include "stdafx.h"
#include "XlsOperator.h"

//#pragma comment(lib,"libxl.lib")


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

typedef libxl::IBookT<char>* (__stdcall *TxlCreateBookA)();

bool CXlsOperator::Create()
{
	bool bSuccess = false;
	TCHAR szlibxldll[] = { "libxl.dll" };
	HMODULE hModule = GetModuleHandle(szlibxldll);
	if ( hModule==NULL ){
		hModule = LoadLibrary(szlibxldll);
	}

	if ( hModule==NULL ) {
		m_strLastError = "not found:";
		m_strLastError += szlibxldll;
		return bSuccess;
	}

	TxlCreateBookA fnxlCreateBook = (TxlCreateBookA)::GetProcAddress(hModule,"xlCreateBookA");
	if ( fnxlCreateBook==NULL ) {
		m_strLastError = "not found: libxl.dll::xlCreateBookA";
		return bSuccess;
	}

	m_book = fnxlCreateBook();	//xlCreateBook();
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
bool CXlsOperator::SetCol(int colFirst, int colLast, double width)
{
	bool bRet = false;

	if ( m_sheet!=NULL ) {
		bRet = m_sheet->setCol(colFirst, colLast, width);
	}

	return bRet;
}

//��ӱ��⣺��0�б�ռ���ˣ�Created by LibXL trial version. Please buy the LibXL full version for removing this message.
//�������ǵı���ӵ�1�п�ʼ
bool CXlsOperator::AddTitle(const TCHAR* value)
{
	bool bRet = false;
	IFormatT<TCHAR>* boldFormat = NULL;

	if ( m_sheet!=NULL ) {
		Font* font = m_book->addFont();
		font->setColor(COLOR_BLACK);
		font->setBold(true);
		boldFormat = m_book->addFormat();
		boldFormat->setFont(font);
		boldFormat->setAlignH(ALIGNH_CENTER);	

		bRet = m_sheet->writeStr(1,m_nTitleCount++,value,boldFormat);
	}

	return bRet;
}

//д����
bool CXlsOperator::WriteStr(int row, int col, const TCHAR* value)
{
	bool bRet = false;
	Format* alignFormat = NULL;

	if ( m_sheet!=NULL ) {
		//�����������
		Format* alignFormat = m_book->addFormat();
		alignFormat->setAlignH(ALIGNH_CENTER);

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

string CXlsOperator::GetLastError()
{
	return m_strLastError;
}