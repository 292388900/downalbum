// ToolboxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "global.h"
#include "ImageBat.h"
#include "ToolboxDlg.h"
#include "MainDlg.h"
#include "ClipImageDlg.h"
#include "RenameImageDlg.h"
#include "UpdateTimeDlg.h"
#include "AddCommentsDlg.h"
#include "ThumbnailDlg.h"
#include "WaterMarkerDlg.h"


// CToolboxDlg 对话框

IMPLEMENT_DYNAMIC(CToolboxDlg, CDialog)

CToolboxDlg::CToolboxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolboxDlg::IDD, pParent)
{

}

CToolboxDlg::~CToolboxDlg()
{
}

void CToolboxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW_OPRATE_MENU, m_stcShowOperateMenu);
}


BEGIN_MESSAGE_MAP(CToolboxDlg, CDialog)
	ON_COMMAND_RANGE(IDC_BATCHCMD_START,IDC_BATCHCMD_END,&CToolboxDlg::OnBtnClickEditPhotos)
	ON_BN_CLICKED(IDC_EDIT_WATERMARKER, &CToolboxDlg::OnBnClickedEditWatermarker)
	ON_STN_CLICKED(IDC_STATIC_SHOW_OPRATE_MENU, &CToolboxDlg::OnStnClickedStaticShowOprateMenu)
END_MESSAGE_MAP()


// CToolboxDlg 消息处理程序
BOOL CToolboxDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_stcShowOperateMenu.SetUnderline(CStaticLink::ulAlways);

	return TRUE;
}

void CToolboxDlg::OnBtnClickEditPhotos(UINT uCtrlID)
{
	if ( uCtrlID<=0 ){
		return;
	}

	if ( uCtrlID==IDC_EDIT_WATERMARKER ){
		CWaterMarkerDlg dlg;
		dlg.DoModal();
		return;
	}

	//先检查有无勾选要处理的图片
	int nItemSelected = -1;
	CListCtrl *pCtrl = &(g_pMainDlg->m_lstPhotos);
	for ( int nItem=0; nItem<pCtrl->GetItemCount(); ++nItem ){
		if ( pCtrl->GetCheck(nItem)==TRUE ){
			nItemSelected = nItem;
			break;
		}
	}

	//获取第一个被选中的图片的路径：当前图片浏览目录+图片文件名
	CString strFirstSelectItemFile;

	if ( nItemSelected==-1 ){
		AfxMessageBox(_T("请先勾选您要编辑的图片!"),MB_OK|MB_ICONWARNING);
		return;
	}else{
		ITEMINFO *pItem = (ITEMINFO *)pCtrl->GetItemData(nItemSelected);
		if ( pItem!=NULL ){
			strFirstSelectItemFile = pItem->strFilePath;
		}
	}

	//根据不同的操作打开不同的操作对话框
	INT_PTR nDlgRet = IDCANCEL;
	switch ( uCtrlID )
	{
	case IDC_RENAME:
		{
			CRenameImageDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//开始批量处理
				PROCESS_PARAM_RENAMEIMAGE *pParam = new PROCESS_PARAM_RENAMEIMAGE(dlg.m_stParam);
				AfxBeginThread(CMainDlg::ThreadRenameImageBatch,pParam);
			}
		}
		break;
	case IDC_CLIPIMAGE:
		{
			CClipImageDlg dlg;
			dlg.m_strPreviewImageFile = strFirstSelectItemFile;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//开始批量处理
				PROCESS_PARAM_CLIPIMAGE *pParam = new PROCESS_PARAM_CLIPIMAGE(dlg.m_stParam);
				AfxBeginThread(CMainDlg::ThreadClipImageBatch,pParam);
			}
		}
		break;
	case IDC_ROTATEIMAGE90:
	case IDC_ROTATEIMAGE270:
	case IDC_IMAGEFLIPX:
	case IDC_IMAGEFLIPY:
		{
			int nRotateFlip = Rotate90FlipNone;
			if ( uCtrlID==IDC_ROTATEIMAGE270 ){
				nRotateFlip = Rotate270FlipNone;
			}else if ( uCtrlID==IDC_IMAGEFLIPX ){
				nRotateFlip = RotateNoneFlipX;
			}else if ( uCtrlID==IDC_IMAGEFLIPY ){
				nRotateFlip = RotateNoneFlipY;
			}
			//开始批量处理
			AfxBeginThread(CMainDlg::ThreadRotateFlipImageBatch,(LPVOID)((PBYTE)NULL + nRotateFlip));
		}
		break;
	case IDC_UPDATETIME:
		{
			CUpdateTimeDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//开始批量处理
				PROCESS_PARAM_UPDATETIME *pParam = new PROCESS_PARAM_UPDATETIME;
				pParam->nHours = dlg.m_nHoursToAdd;
				AfxBeginThread(CMainDlg::ThreadAjustTimeImageBatch,pParam);
			}
		}
		break;
	case IDC_THUMBNAIL:
		{
			CThumbnailDlg dlg;
			dlg.m_strImageSrcPath = g_pMainDlg->m_strCurPhotoPath;
			dlg.m_strImageOutPath = g_config.strSavePath;

			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				if ( GetFileAttributes(dlg.m_strImageOutPath)==-1 ){
					CreateDirectory(dlg.m_strImageOutPath,NULL);
				}

				//开始批量处理
				PROCESS_PARAM_THUMBNAIL *pParam = new PROCESS_PARAM_THUMBNAIL;
				pParam->nPixelWidth = dlg.m_nPixelWidth;
				pParam->bResotreExif = dlg.m_bResotreExif;
				AfxBeginThread(CMainDlg::ThreadThumbnailImageBatch,pParam);
			}
		}
		break;
	case IDC_DELETEEXIF:
		{
			nDlgRet = AfxMessageBox("确定要删除图片Exif信息？删除后将无法恢复",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//开始批量处理
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifBatch,NULL);
			}
		}
		break;
	case IDC_DELETEEXIF_NOT_ORIGINAL:
		{
			nDlgRet = AfxMessageBox("删除非原始exif信息（例如Photoshop，美图秀秀之类编辑后修改的信息），确定删除吗？",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//开始批量处理
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifNotOrignalBatch,NULL);
			}
		}
		break;
	case IDC_DELETE_REMARKS:
		{
			nDlgRet = AfxMessageBox("删除jpeg信息中的备注，确定删除吗？",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//开始批量处理
				AfxBeginThread(CMainDlg::ThreadDeleteRemarksBatch,NULL);
			}
		}
		break;
	case IDC_PUREJPG:
		{
			nDlgRet = AfxMessageBox("删除所有jpeg文件非必须的信息，图片不会失真，确定删除吗？",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//开始批量处理
				AfxBeginThread(CMainDlg::ThreadPureJpgBatch,NULL);
			}
		}
		break;
	case IDC_AUTOROTATE:
		{
			nDlgRet = AfxMessageBox("根据图片记录水平信息自动转动图片（图片不会失真）？",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//开始批量处理
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifBatch,NULL);
			}
		}
		break;
	case IDC_ADD_REMARKS:
		{
			CAddCommentsDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//开始批量处理
				PROCESS_PARAM_ADDCOMMENTS *pParam = new PROCESS_PARAM_ADDCOMMENTS;
				pParam->strComments = dlg.m_strAddComments;
				AfxBeginThread(CMainDlg::ThreadAddRemarksBatch,pParam);
			}
		}
		break;
	case IDC_ADD_WATERMARKER:
		{
			CString strTitle = "选择水印模板";
			CFileDialog dlg(TRUE);
			dlg.m_ofn.lpstrInitialDir = g_config.m_strWatermarkTemplatePath;
			dlg.m_ofn.lpstrTitle = (LPSTR)(LPCTSTR)strTitle;
			if ( dlg.DoModal() == IDOK ){
				//开始批量处理
				PROCESS_PARAM_ADDWATERMARKER *pParam = new PROCESS_PARAM_ADDWATERMARKER;
				pParam->strWatermarkTemplateFile = dlg.GetPathName();
				AfxBeginThread(CMainDlg::ThreadAddWaterMarkerBatch,pParam);
			} 
		}
		break;
	}

}

void CToolboxDlg::OnBnClickedEditWatermarker()
{
	CWaterMarkerDlg dlg;
	dlg.DoModal();
}

void CToolboxDlg::OnStnClickedStaticShowOprateMenu()
{
	//取主菜单的第一个子菜单显示
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_MAIN);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);
	OnBtnClickEditPhotos(dwID);
}
