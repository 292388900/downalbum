
#include "stdafx.h"
#include "taobaoCsv.h"

CString CTaobaoCsv::m_strColumnNames[]=
{
	"��������",
	"������Ŀ",
	"������Ŀ",
	"�¾ɳ̶�",	
	"ʡ",	
	"����",	
	"���۷�ʽ",	
	"�����۸�",	
	"�Ӽ۷���",	
	"��������",	
	"��Ч��",	
	"�˷ѳе�",	
	"ƽ��",	
	"EMS",	
	"���",	
	"���ʽ",	
	"֧����",	
	"��Ʊ",	
	"����",	
	"�Զ��ط�",	
	"����ֿ�",
	"�����Ƽ�",	
	"��ʼʱ��",	
	"�������",	
	"��������",	
	"����ͼƬ",	
	"��������",	
	"�Ź���",	
	"��С�Ź�����",
	"�ʷ�ģ��ID",
	"��Ա����",	
	"�޸�ʱ��",	
	"�ϴ�״̬",	
	"ͼƬ״̬",	
	"�������",	
	"��ͼƬ",	
	"��Ƶ",	
	"�����������",	
	"�û�����ID��",	
	"�û�������-ֵ��",
	"�̼ұ���",	
	"�������Ա���",	
	"��������",	
	"�������",	
	"����ID"
};

int CTaobaoCsv::m_nColumns=sizeof(CTaobaoCsv::m_strColumnNames)/sizeof(CString);

CTaobaoCsv::CTaobaoCsv()
{
	AddItem();	//Ԥ��һ�и�������
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
