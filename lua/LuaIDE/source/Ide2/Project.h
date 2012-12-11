// Project.h: interface for the CProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_)
#define AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProjectFile.h"

class CWorkspaceWnd;

typedef CTypedPtrArray<CPtrArray, CProjectFile*> CProjectFileArray;

class CProject  
{
public:
	void RemoveFile(CProjectFile* pPF);
	BOOL Close();
	BOOL New();
	void UpdateDirectories();
	BOOL HasBreakPoint(const char* szFile, int nLine);
	CProject();
	virtual ~CProject();

	BOOL PositionBreakPoints();
	void CleanIntermediateAndOutputFiles();
	BOOL CheckBeforeBuild();
	BOOL Compile(CProjectFile* pPF);
	BOOL BuildOutputFiles();
	BOOL BuildIntermediateFiles();
	BOOL Build();

	void SetModifiedFlag(BOOL bModified) { m_bModified = bModified; };
	void SaveModified();
	BOOL SaveAs();
	BOOL Save();
	BOOL Save(CString strPathName);
	BOOL Save(CArchive& ar);
	BOOL Load();
	BOOL Load(CArchive& ar);

	void RemoveProjectFiles();
	void AddFile(CString strPathName);
	void AddFile(CProjectFile* pPF);
	int NofFiles() { return m_files.GetSize(); };
	void Properties();
	void AddFiles();
	CProjectFile* GetProjectFile(CString strPathName);
	void RedrawFilesTree();
	void NewProject();

	void SetPathName(CString strPathName) { m_strPathName=strPathName; };
	CString GetProjectDir();
	CString GetPathName() { return m_strPathName; };
	CString GetName();
	CString GetNameExt();
	CString GetDebugNameExt() { return m_strOutputPrefix + ".debug"; };
	CString GetReleaseNameExt() { return m_strOutputPrefix + ".release"; };
	CString GetListingNameExt() { return m_strOutputPrefix + ".listing"; };
	CString GetDebugPathNameExt() { return m_strOutputDirRoot + "\\" + GetDebugNameExt(); };
	CString GetReleasePathNameExt() { return m_strOutputDirRoot + "\\" + GetReleaseNameExt(); };
	CString GetListingPathNameExt() { return m_strOutputDirRoot + "\\" + GetListingNameExt(); };
	CString GetOutputDir() { return m_strOutputDirRoot; };
	CString GetIntermediateDir() { return m_strIntermediateDirRoot; };
	BOOL CreateIntermediateDir();
	BOOL CreateOutputDir();

protected:
	BOOL m_bModified;

	CProjectFileArray	m_files;
	CString m_strPathName;
	CString m_strIntermediateDir;
	CString m_strOutputDir;
	CString m_strOutputPrefix;
	BOOL m_bGenerateListing;

	CString m_strOutputDirRoot, m_strIntermediateDirRoot;
};

#endif // !defined(AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_)
