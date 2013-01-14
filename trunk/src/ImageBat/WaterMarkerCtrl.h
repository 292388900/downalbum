
#pragma once

#include <gdiplus.h>
using namespace Gdiplus;



class CWaterMarkerCtrl : public CWnd
{
	//DECLARE_DYNAMIC(CWaterMarkerCtrl)

public:
	// Contructors
	CWaterMarkerCtrl( int x1, int y1, int x2, int y2 );
	CWaterMarkerCtrl();
	~CWaterMarkerCtrl();

	void Create(CWnd *pParent, BOOL bVisible = TRUE, LPCRECT pRect = NULL);
	void Destroy();

public:
	// ���л��궨��
	DECLARE_SERIAL(CWaterMarkerCtrl)

	// ���л����� Serialize() 
	virtual void Serialize(CArchive& ar);

public:
// Tracks movement rectangle for control
	void Track( CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = NULL );

// Sets bounding rectangle for control
	void SetRect( int x1, int y1, int x2, int y2 );
	void SetRect( const CRect & rect );
	CRect& GetRect();
	void OffsetWindow(int x,int y);

	//ˢ���������ڸ����ڵ�����
	void Update();

	void DrawTracker( CDC* pDC ) const;
	int HitTest(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	//��Ч�����Ƶ�ָ��dc��
	void Draw(HDC hdc, const CRect&rcRect, PVOID pParam = NULL);

public:
	//ˮӡ���ֲ���
	void SetText(const CString& strText);
	CString GetText();

	//ˮӡͼƬ����
	void SetImageFilePath(const CString& strImageFilePath);
	CString GetImageFilePath();
	void LoadImageFile();	//�����ͼƬ�ͼ���ͼƬ
	void AjustToImageSize();//�������ں�ͼƬ��Сһ��

	//�Ƿ�ѡ��
	void SetSelected(BOOL bSelected);
	BOOL GetSelected();

	//������б�ǶȲ���
	void SetGradientAngel(int nAngel);
	int GetGradientAngel();

	//��ӰЧ������
	void SetShadow(BOOL bShadow);
	BOOL GetShadow();

	//͸��Ч������
	void SetTransparent(BOOL bTransparent);
	BOOL GetTransparent();
	void SetTransparentDegree(int nTransparentDegree);
	int GetTransparentDegree();

	//��ɫ����
	void SetTextColor(COLORREF clr);
	COLORREF GetTextColor();

	//�������
	void SetTextFont(LOGFONT *pLogFont);
	LOGFONT GetTextFont();


	CWnd *GetParentWnd();
public:
	CString m_strText;	//ˮӡ����
	CString m_strImageFilePath;	//ˮӡͼƬ·��

	int  m_nGradient;	//������б��
	BOOL m_bShadow;		//������ӰЧ��
	BOOL m_bTransparent;//͸��Ч��
	int  m_nTransparentDegree;//͸��Ч��

	COLORREF m_clrText;	//������ɫ
	LOGFONT m_lffont;

	CRectTracker m_rectTracker;
	Image *m_pWaterMarkerImg;

	//��ˮӡ���е�λ��
	CPoint m_ptInBounds;
	//��ˮӡ���е�z order˳��
	int m_nZOrderInBounds;

	//Ԥ���㹻�����Ա���
	int m_nReserved[100];
	CString m_strReserved[10];

private:
// TRUE if selected
	BOOL m_bSelected;
// virtual function called to get handle masks
	UINT GetHandleMask() const;
// Sets default attributes for CRectTracker
	void Initialize();

	CWnd *m_pParentWnd;

	BOOL m_bDestroyed;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};