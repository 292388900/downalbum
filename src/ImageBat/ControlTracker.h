#pragma once

#include "WaterMarkerCtrl.h"



//////////////////////////////////////////////////////////////////////////
//ˮӡ���ͷ���汾�ź͸���
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

	//��ˮӡ��ľ��η�Χ
	CRect m_rcBoundsRect;

	//λ������
	int m_nAlignment;		//eAlignment_custom

	//���nAlignmentΪeAlignment_custom���û��Զ���λ��ʱ������λ��ʹ���������ֵ���������һ����ֵ�����Զ����㡣
	int nPosX;
	int nPosY;

	int m_nFrameStyle;		//�߿��ʽ��0Ϊ�ޱ߿�1Ϊ��ɫ�߿�2ΪͼƬ�߿�
	COLORREF m_clrFrame;	//�߿���ɫ
	int m_nFrameTopHeight;
	int m_nFrameBottomHeight;
	int m_nFrameLeftWidth;
	int m_nFrameRightWidth;
	CString m_strFrameImageFile;//�����ͼƬ�߿�����ָʾͼƬ��·��

	//Ԥ���㹻�����Ա���
	int m_nReserved[100];
	CString m_strReserved[10];

public:
	DECLARE_SERIAL(CWaterMarkersHeader)

	// ���л����� Serialize() 
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
//��Ŀ�귶Χ�ϻ���ˮӡ�飬ˮӡ����ռ����ֻ��rcWorkarea��һС����
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

	//��ȡ����ˮӡ�ؼ���һ����ռ����С��������
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

