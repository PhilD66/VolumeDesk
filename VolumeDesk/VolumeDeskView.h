
// VolumeDeskView.h : interface of the CVolumeDeskView class
//

#pragma once

#include "openglclass.h"
#include "graphicsclass.h"

class CVolumeDeskView : public CView
{
protected: // create from serialization only
	CVolumeDeskView();
	DECLARE_DYNCREATE(CVolumeDeskView)

// Attributes
public:
	CVolumeDeskDoc* GetDocument() const;

protected:
	OpenGLClass	*m_pOpenGL;
	GraphicsClass *m_pGraphics;
	CPoint m_mousePos;
	BOOL m_bMouseButtonDown;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVolumeDeskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	bool m_bFirstDraw;

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetThreshold(float threshold);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in VolumeDeskView.cpp
inline CVolumeDeskDoc* CVolumeDeskView::GetDocument() const
   { return reinterpret_cast<CVolumeDeskDoc*>(m_pDocument); }
#endif

