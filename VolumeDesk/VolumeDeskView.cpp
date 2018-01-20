
// VolumeDeskView.cpp : implementation of the CVolumeDeskView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VolumeDesk.h"
#endif

#include "VolumeDeskDoc.h"
#include "VolumeDeskView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVolumeDeskView

IMPLEMENT_DYNCREATE(CVolumeDeskView, CView)

BEGIN_MESSAGE_MAP(CVolumeDeskView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVolumeDeskView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CVolumeDeskView construction/destruction

CVolumeDeskView::CVolumeDeskView()
{
	// TODO: add construction code here

}

CVolumeDeskView::~CVolumeDeskView()
{
}

BOOL CVolumeDeskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVolumeDeskView drawing

void CVolumeDeskView::OnDraw(CDC* /*pDC*/)
{
	CVolumeDeskDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CVolumeDeskView printing


void CVolumeDeskView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVolumeDeskView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVolumeDeskView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVolumeDeskView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CVolumeDeskView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVolumeDeskView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVolumeDeskView diagnostics

#ifdef _DEBUG
void CVolumeDeskView::AssertValid() const
{
	CView::AssertValid();
}

void CVolumeDeskView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVolumeDeskDoc* CVolumeDeskView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVolumeDeskDoc)));
	return (CVolumeDeskDoc*)m_pDocument;
}
#endif //_DEBUG


// CVolumeDeskView message handlers
