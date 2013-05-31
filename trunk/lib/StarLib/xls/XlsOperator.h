

#pragma once
#include <libxl/include_cpp/libxl.h>
#include <xstring>
using namespace std;
using namespace libxl;

class CXlsOperator
{
public:
	CXlsOperator();
	~CXlsOperator();
public:
	bool Create();

	//�������Ŀ��
	bool SetCol(int colFirst, int colLast, double width);

	//��ӱ���
	bool AddTitle(const TCHAR* value);

	//д����
	bool WriteStr(int row, int col, const TCHAR* value);

	//��ȡ����
	int GetRowsCount();

	//��ȡ����
	int GetColsCount();

	bool Save(const TCHAR* lpszFileName);

	string GetLastError();
private:
	Book  *m_book;
	Sheet *m_sheet;
	int m_nTitleCount;	//�������

	string m_strLastError;
};