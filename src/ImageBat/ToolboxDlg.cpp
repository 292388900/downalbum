// ToolboxDlg.cpp : ʵ���ļ�
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


// CToolboxDlg �Ի���

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


// CToolboxDlg ��Ϣ�������
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

	//�ȼ�����޹�ѡҪ�����ͼƬ
	int nItemSelected = -1;
	CListCtrl *pCtrl = &(g_pMainDlg->m_lstPhotos);
	for ( int nItem=0; nItem<pCtrl->GetItemCount(); ++nItem ){
		if ( pCtrl->GetCheck(nItem)==TRUE ){
			nItemSelected = nItem;
			break;
		}
	}

	//��ȡ��һ����ѡ�е�ͼƬ��·������ǰͼƬ���Ŀ¼+ͼƬ�ļ���
	CString strFirstSelectItemFile;

	if ( nItemSelected==-1 ){
		AfxMessageBox(_T("���ȹ�ѡ��Ҫ�༭��ͼƬ!"),MB_OK|MB_ICONWARNING);
		return;
	}else{
		ITEMINFO *pItem = (ITEMINFO *)pCtrl->GetItemData(nItemSelected);
		if ( pItem!=NULL ){
			strFirstSelectItemFile = pItem->strFilePath;
		}
	}

	//���ݲ�ͬ�Ĳ����򿪲�ͬ�Ĳ����Ի���
	INT_PTR nDlgRet = IDCANCEL;
	switch ( uCtrlID )
	{
	case IDC_RENAME:
		{
			CRenameImageDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//��ʼ��������
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
				//��ʼ��������
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
			//��ʼ��������
			AfxBeginThread(CMainDlg::ThreadRotateFlipImageBatch,(LPVOID)((PBYTE)NULL + nRotateFlip));
		}
		break;
	case IDC_UPDATETIME:
		{
			CUpdateTimeDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//��ʼ��������
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

				//��ʼ��������
				PROCESS_PARAM_THUMBNAIL *pParam = new PROCESS_PARAM_THUMBNAIL;
				pParam->nPixelWidth = dlg.m_nPixelWidth;
				pParam->bResotreExif = dlg.m_bResotreExif;
				AfxBeginThread(CMainDlg::ThreadThumbnailImageBatch,pParam);
			}
		}
		break;
	case IDC_DELETEEXIF:
		{
			nDlgRet = AfxMessageBox("ȷ��Ҫɾ��ͼƬExif��Ϣ��ɾ�����޷��ָ�",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//��ʼ��������
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifBatch,NULL);
			}
		}
		break;
	case IDC_DELETEEXIF_NOT_ORIGINAL:
		{
			nDlgRet = AfxMessageBox("ɾ����ԭʼexif��Ϣ������Photoshop����ͼ����֮��༭���޸ĵ���Ϣ����ȷ��ɾ����",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//��ʼ��������
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifNotOrignalBatch,NULL);
			}
		}
		break;
	case IDC_DELETE_REMARKS:
		{
			nDlgRet = AfxMessageBox("ɾ��jpeg��Ϣ�еı�ע��ȷ��ɾ����",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//��ʼ��������
				AfxBeginThread(CMainDlg::ThreadDeleteRemarksBatch,NULL);
			}
		}
		break;
	case IDC_PUREJPG:
		{
			nDlgRet = AfxMessageBox("ɾ������jpeg�ļ��Ǳ������Ϣ��ͼƬ����ʧ�棬ȷ��ɾ����",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//��ʼ��������
				AfxBeginThread(CMainDlg::ThreadPureJpgBatch,NULL);
			}
		}
		break;
	case IDC_AUTOROTATE:
		{
			nDlgRet = AfxMessageBox("����ͼƬ��¼ˮƽ��Ϣ�Զ�ת��ͼƬ��ͼƬ����ʧ�棩��",MB_YESNO);
			if ( nDlgRet==IDYES ){
				//��ʼ��������
				AfxBeginThread(CMainDlg::ThreadDeleteImageExifBatch,NULL);
			}
		}
		break;
	case IDC_ADD_REMARKS:
		{
			CAddCommentsDlg dlg;
			nDlgRet = dlg.DoModal();
			if ( nDlgRet==IDOK ){
				//��ʼ��������
				PROCESS_PARAM_ADDCOMMENTS *pParam = new PROCESS_PARAM_ADDCOMMENTS;
				pParam->strComments = dlg.m_strAddComments;
				AfxBeginThread(CMainDlg::ThreadAddRemarksBatch,pParam);
			}
		}
		break;
	case IDC_ADD_WATERMARKER:
		{
			CString strTitle = "ѡ��ˮӡģ��";
			CFileDialog dlg(TRUE);
			dlg.m_ofn.lpstrInitialDir = g_config.m_strWatermarkTemplatePath;
			dlg.m_ofn.lpstrTitle = (LPSTR)(LPCTSTR)strTitle;
			if ( dlg.DoModal() == IDOK ){
				//��ʼ��������
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
	//ȡ���˵��ĵ�һ���Ӳ˵���ʾ
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_MAIN);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);
	OnBtnClickEditPhotos(dwID);
}
