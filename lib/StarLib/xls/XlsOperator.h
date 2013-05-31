

#pragma once
#include <libxl/include_cpp/libxl.h>
using namespace libxl;

class CXlsOperator
{
public:
	CXlsOperator();
	~CXlsOperator();
public:
	bool Create();

	//�������Ŀ��
	bool SetCol(int colFirst, int colLast, double width, IFormatT<TCHAR>* format = 0, bool hidden = false);

	//��ӱ���
	bool AddTitle(const TCHAR* value, IFormatT<TCHAR>* format = 0);

	//д����
	bool WriteStr(int row, int col, const TCHAR* value, IFormatT<TCHAR>* format = 0);

	//��ȡ����
	int GetRowsCount();

	//��ȡ����
	int GetColsCount();

	bool Save(const TCHAR* lpszFileName);
private:
	Book  *m_book;
	Sheet *m_sheet;
	int m_nTitleCount;	//�������
};