

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

	//设置栏的宽度
	bool SetCol(int colFirst, int colLast, double width);

	//添加标题
	bool AddTitle(const TCHAR* value);

	//写内容
	bool WriteStr(int row, int col, const TCHAR* value);

	//获取行数
	int GetRowsCount();

	//获取列数
	int GetColsCount();

	bool Save(const TCHAR* lpszFileName);

	string GetLastError();
private:
	Book  *m_book;
	Sheet *m_sheet;
	int m_nTitleCount;	//标题个数

	string m_strLastError;
};