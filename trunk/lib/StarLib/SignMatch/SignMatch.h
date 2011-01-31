
/*------------------------------------------------------------------------
[8/6/2009 zhuxingxing]
˵��:�ڴ�ɨ�����������
ʹ��ʾ��:
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
	//�û�ָ�����������ַ���
	CString m_strOrigSign;	
	//m_strOrigSignһ�δ�����������,��Ҫ��?�任Ϊ0
	BYTE*	m_pPureSign;	
	//�����������,�������ģ��ƥ��
	BYTE*	m_pMaskSign;
	//�����볤��
	int		m_nSignLen;

	//��������ʼrva
	DWORD	m_dwSearchBegin;
	//�����Ľ���rva
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