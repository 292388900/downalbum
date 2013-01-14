#pragma once

#include "WaterMarkerCtrl.h"



//////////////////////////////////////////////////////////////////////////
//水印组的头：版本号和个数
class CWaterMarkersHeader : public CObject
{
public:
	CWaterMarkersHeader();
	void UpdateFrom(PVOID lpParam);//CControlTracker *
	void UpdateFrom(CWaterMarkersHeader *pHdr);

private:
	void Init();

public:
	int m_nVersion;
	int m_nCount;
	CString m_strInfos;

	//该水印组的矩形范围
	CRect m_rcBoundsRect;

	//位置属性
	int m_nAlignment;		//eAlignment_custom

	//如果nAlignment为eAlignment_custom即用户自定义位置时，坐标位置使用下面的数值，否则忽略一下数值进行自动计算。
	int nPosX;
	int nPosY;

	int m_nFrameStyle;		//边框格式：0为无边框，1为颜色边框，2为图片边框
	COLORREF m_clrFrame;	//边框颜色
	int m_nFrameTopHeight;
	int m_nFrameBottomHeight;
	int m_nFrameLeftWidth;
	int m_nFrameRightWidth;
	CString m_strFrameImageFile;//如果是图片边框，这里指示图片的路径

	//预留足够的属性备用
	int m_nReserved[100];
	CString m_strReserved[10];

public:
	DECLARE_SERIAL(CWaterMarkersHeader)

	// 串行化函数 Serialize() 
	virtual void Serialize(CArchive& ar);
};
//////////////////////////////////////////////////////////////////////////



class CControlTracker : public CRectTracker
{
public:
// Sets the cursor on all the controls
	BOOL SetCursor( UINT nHitTest, UINT message );
// Adds the Control to the Tracker
	void Add( CWaterMarkerCtrl* pObject );
//delete all
	void DeleteAllItems();
//delete selected items
	void DeleteSelectedItems();
//get count
	int GetItemsCount();
//load items from file
	BOOL Load(const CString& strFilePath,CWnd *pWndParent, BOOL bVisible = TRUE);
//save items to file
	void Save(const CString& strFilePath);

// Tells weather the specified control is selected or not
	BOOL IsSelected( CWaterMarkerCtrl* pObject ) const;
// Selects the specified control. Returns TRUE if it was initially unselected else FALSE
	BOOL Select( CWaterMarkerCtrl* pObject );
// DeSelects the specified control. Returns TRUE if it was initially selected else FALSE
	BOOL DeSelect( CWaterMarkerCtrl* pObject );
// Toggles the state of the object, TRUE: control was selected, else FALSE
	BOOL Toggle( CWaterMarkerCtrl* pObject );
// Draws all the controls
	void DrawTracker( CDC* pDC ) const;
//在目标范围上绘制水印组，水印组所占区域只是rcWorkarea的一小部分
	void Draw(HDC hdc,const CRect&rcWorkarea,PVOID pParam = NULL);

// Starts tracking the controls
	BOOL Track( const CPoint & point, UINT nFlags = 0, BOOL bTrackRubberBand = TRUE );
//select all items
	void SelectAll();
// DeSelects all the controls, returns the number of controls that were selected
	int DeSelectAll();
// Creates the Control Tracker Object
	void Create( CWnd* pParentWnd );

	CWaterMarkerCtrl *GetLastSelectedItem();

	// Gets Bounding rect for multiply selected objects
	CRect GetBoundsRect() const;

	//获取所有水印控件在一起所占的最小矩形区域
	CRect GetAllItemsBoundsRect();

public:
// Corrects the z-orders of all the controls
	void CorrectZOrders( CWnd* pWndInsertAfter );
// Selects a control unconditionally
	void SelectUnConditional( CWaterMarkerCtrl* pObject );	
// Parent window
	CWnd* m_pwndParent;
// Array of all items
	CArray< CWaterMarkerCtrl*, CWaterMarkerCtrl* > m_arrRectanglesAll;
// Array of currently selected items
	CArray< CWaterMarkerCtrl*, CWaterMarkerCtrl* > m_arrRectanglesSelected;

public:
	CControlTracker();
	virtual ~CControlTracker();

	void SetAlignment(int nAlignment);
	int GetAlignment();

public:
	CWaterMarkersHeader m_header;
};

