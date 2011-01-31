
#include "stdafx.h"
#include "taobaoCsv.h"

CString CTaobaoCsv::m_strColumnNames[]=
{
	"宝贝名称",
	"宝贝类目",
	"店铺类目",
	"新旧程度",	
	"省",	
	"城市",	
	"出售方式",	
	"宝贝价格",	
	"加价幅度",	
	"宝贝数量",	
	"有效期",	
	"运费承担",	
	"平邮",	
	"EMS",	
	"快递",	
	"付款方式",	
	"支付宝",	
	"发票",	
	"保修",	
	"自动重发",	
	"放入仓库",
	"橱窗推荐",	
	"开始时间",	
	"心情故事",	
	"宝贝描述",	
	"宝贝图片",	
	"宝贝属性",	
	"团购价",	
	"最小团购件数",
	"邮费模版ID",
	"会员打折",	
	"修改时间",	
	"上传状态",	
	"图片状态",	
	"返点比例",	
	"新图片",	
	"视频",	
	"销售属性组合",	
	"用户输入ID串",	
	"用户输入名-值对",
	"商家编码",	
	"销售属性别名",	
	"代充类型",	
	"宝贝编号",	
	"数字ID"
};

int CTaobaoCsv::m_nColumns=sizeof(CTaobaoCsv::m_strColumnNames)/sizeof(CString);

CTaobaoCsv::CTaobaoCsv()
{
	AddItem();	//预留一行给标题用
}

CTaobaoCsv::~CTaobaoCsv()
{

}

int CTaobaoCsv::AddItem()
{
	vector<CString> strarr;
	for ( int nCol=0; nCol<CTaobaoCsv::m_nColumns; ++nCol ){
		strarr.push_back("");
	}
	m_vtItems.push_back(strarr);
	m_nItemIndex=m_vtItems.size()-1;
	return m_nItemIndex;
}

void CTaobaoCsv::Set(COLUMN nColumn,CString str)
{
	m_vtItems[m_nItemIndex][nColumn]=str;
}

bool CTaobaoCsv::SaveFile(CString strFileName)
{
	bool bOK=false;
	char prefix[]="\xFF\xFE";
	CStringW str;
	CStringW strText;

	CFile file;
	if ( file.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite)==TRUE ){
		file.Write(prefix,2);
		for ( int nCol=0; nCol<CTaobaoCsv::m_nColumns; ++nCol ){
			str=CTaobaoCsv::m_strColumnNames[nCol];
			str+=L"\t";
			file.Write((LPCWSTR)str,str.GetLength()*2);
		}
		file.Write(L"\n",2);
		for ( int nRow=0; nRow<=m_nItemIndex; ++nRow ){
			for ( int nCol=0; nCol<CTaobaoCsv::m_nColumns; ++nCol ){
				str=m_vtItems[nRow][nCol];
				file.Write((LPCWSTR)str,str.GetLength()*2);
			}
		}
		file.Close();
	}

	return bOK;
}
