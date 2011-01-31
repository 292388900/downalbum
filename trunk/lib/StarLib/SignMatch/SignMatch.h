
/*------------------------------------------------------------------------
[8/6/2009 zhuxingxing]
说明:内存扫描特征码的类
使用示例:
CSignMatch g_memScan("??7a42?5666600cccc",0x2d854,0x2d864);
...
DWORD dwPos=g_memScan.SearchMemSign(hProcess,(DWORD)hModule);
------------------------------------------------------------------------*/

#pragma once
#include <afx.h>
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")


class CSignMatch
{
public:
	enum{MinSignLen=1,MaxSignLen=0x100,MaxScanRange=0x10000};
	//用户指定的特征码字符串
	CString m_strOrigSign;	
	//m_strOrigSign一次处理后的特征码,主要将?变换为0
	BYTE*	m_pPureSign;	
	//特征码的掩码,用以完成模糊匹配
	BYTE*	m_pMaskSign;
	//特征码长度
	int		m_nSignLen;

	//搜索的起始rva
	DWORD	m_dwSearchBegin;
	//搜索的结束rva
	DWORD	m_dwSearchEnd;
	DWORD	m_nRange;
	DWORD	m_dwMatchPos;
private:
	void	FreeMemory();
	BYTE	HexCharToBin(BYTE chHex);

public:
	CSignMatch();
	CSignMatch(const CSignMatch&obj);
	CSignMatch(CString strOrigSign,DWORD dwStartRva=0,DWORD dwEndRva=0);
	virtual ~CSignMatch();

	BOOL	SetOrigSign(CString strOrigSign,DWORD dwStartRva=0,DWORD dwEndRva=0);
	PBYTE	SearchSign(PBYTE pBase);
	PBYTE	SearchProcessSign(HANDLE hProcess,PBYTE pImageBase);
};