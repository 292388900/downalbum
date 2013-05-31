

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

	//设置栏的宽度
	bool SetCol(int colFirst, int colLast, double width, IFormatT<TCHAR>* format = 0, bool hidden = false);

	//添加标题
	bool AddTitle(const TCHAR* value, IFormatT<TCHAR>* format = 0);

	//写内容
	bool WriteStr(int row, int col, const TCHAR* value, IFormatT<TCHAR>* format = 0);

	//获取行数
	int GetRowsCount();

	//获取列数
	int GetColsCount();

	bool Save(const TCHAR* lpszFileName);
private:
	Book  *m_book;
	Sheet *m_sheet;
	int m_nTitleCount;	//标题个数
};