
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
	// 串行化宏定义
	DECLARE_SERIAL(CWaterMarkerCtrl)

	// 串行化函数 Serialize() 
	virtual void Serialize(CArchive& ar);

public:
// Tracks movement rectangle for control
	void Track( CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = NULL );

// Sets bounding rectangle for control
	void SetRect( int x1, int y1, int x2, int y2 );
	void SetRect( const CRect & rect );
	CRect& GetRect();
	void OffsetWindow(int x,int y);

	//刷新自身所在父窗口的区域
	void Update();

	void DrawTracker( CDC* pDC ) const;
	int HitTest(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	//将效果绘制到指定dc上
	void Draw(HDC hdc, const CRect&rcRect, PVOID pParam = NULL);

public:
	//水印文字操作
	void SetText(const CString& strText);
	CString GetText();

	//水印图片操作
	void SetImageFilePath(const CString& strImageFilePath);
	CString GetImageFilePath();
	void LoadImageFile();	//如果有图片就加载图片
	void AjustToImageSize();//调整窗口和图片大小一致

	//是否选中
	void SetSelected(BOOL bSelected);
	BOOL GetSelected();

	//文字倾斜角度操作
	void SetGradientAngel(int nAngel);
	int GetGradientAngel();

	//阴影效果操作
	void SetShadow(BOOL bShadow);
	BOOL GetShadow();

	//透明效果操作
	void SetTransparent(BOOL bTransparent);
	BOOL GetTransparent();
	void SetTransparentDegree(int nTransparentDegree);
	int GetTransparentDegree();

	//颜色操作
	void SetTextColor(COLORREF clr);
	COLORREF GetTextColor();

	//字体操作
	void SetTextFont(LOGFONT *pLogFont);
	LOGFONT GetTextFont();


	CWnd *GetParentWnd();
public:
	CString m_strText;	//水印文字
	CString m_strImageFilePath;	//水印图片路径

	int  m_nGradient;	//文字倾斜度
	BOOL m_bShadow;		//文字阴影效果
	BOOL m_bTransparent;//透明效果
	int  m_nTransparentDegree;//透明效果

	COLORREF m_clrText;	//文字颜色
	LOGFONT m_lffont;

	CRectTracker m_rectTracker;
	Image *m_pWaterMarkerImg;

	//在水印组中的位置
	CPoint m_ptInBounds;
	//在水印组中的z order顺序
	int m_nZOrderInBounds;

	//预留足够的属性备用
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