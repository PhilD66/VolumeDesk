
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
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CVolumeDeskView construction/destruction

CVolumeDeskView::CVolumeDeskView()
{
	// TODO: add construction code here
	m_pOpenGL = NULL;
	m_pGraphics = NULL;
	m_bMouseButtonDown = FALSE;
	m_bFirstDraw = TRUE;
}

CVolumeDeskView::~CVolumeDeskView()
{
	if (m_pGraphics != NULL)
	{
		delete m_pGraphics;
	}

	if (m_pOpenGL != NULL)
	{
		delete m_pOpenGL;
	}
}

BOOL CVolumeDeskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.dwExStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// Create the OpenGL object
	m_pOpenGL = new OpenGLClass;
	if (!m_pOpenGL)
	{
		return false;
	}

	m_pGraphics = new GraphicsClass();
	if (!m_pGraphics)
	{
		return false;
	}

	return CView::PreCreateWindow(cs);
}

// CVolumeDeskView drawing

void CVolumeDeskView::OnDraw(CDC* pDC)
{
	OutputDebugStringW(L"CVolumeDeskView::OnDraw called\n");
	CVolumeDeskDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	BOOL redScreen = false;
	if (redScreen)
	{
		// TODO: add draw code for native data here
		CBrush backBrush(RGB(255, 0, 0));

		// Save old brush
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);

		CRect rect;
		pDC->GetClipBox(&rect);     // Erase the area needed

		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

		//Restore old brush
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		if (m_bFirstDraw)
		{
			for (int att = 0; att < 1; att++)
			{
				m_pGraphics->Frame();
			}
			m_bFirstDraw = FALSE;
		}
		else
		{
			m_pGraphics->Frame();
		}
	}
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


int CVolumeDeskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialise OpenGL
	WNDCLASSEX wc;
	bool result;

	// Give the application a name.
	LPCWSTR m_openGLWindowName = L"Temp OpenGL Window";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_openGLWindowName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	HWND hParentWnd = GetParent()->GetSafeHwnd();
	// Create a temporary window for the OpenGL extension setup.
	HWND m_oglWnd = ::CreateWindow(m_openGLWindowName, m_openGLWindowName, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 20, 20, hParentWnd, NULL, wc.hInstance, NULL);
	if (m_oglWnd == NULL)
	{
		return 0;
	}

	// Don't show the window.
	::ShowWindow(m_oglWnd, SW_HIDE);

	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_pOpenGL->InitializeExtensions(m_oglWnd);
	if (!result)
	{
		MessageBox(L"Could not initialize the OpenGL extensions.", L"Error", MB_OK);
		return 0;
	}

	// Release the temporary window now that the extensions have been initialized.
	::DestroyWindow(m_oglWnd);
	m_oglWnd = NULL;

	return 0;
}


void CVolumeDeskView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	// Initialize OpenGL now that the window has been created.
	if (m_pOpenGL->IsInitialised() == FALSE)
	{
		BOOL result = m_pOpenGL->InitializeOpenGL(GetDC()->GetSafeHdc(), 0, 0, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
		if (!result)
		{
			MessageBox(L"Could not initialize OpenGL, check if video card supports OpenGL 4.0.", L"Error", MB_OK);
			return;
		}

		// Initialize the graphics object.
		result = m_pGraphics->Initialize(m_pOpenGL, GetParent()->GetSafeHwnd());
		if (!result)
		{
			return;
		}
	}

	Invalidate(FALSE);
}


void CVolumeDeskView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_pGraphics->OnResizeWindow(cx, cy);
}


void CVolumeDeskView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseButtonDown = TRUE;
	m_mousePos = point;

	CView::OnLButtonDown(nFlags, point);
}


void CVolumeDeskView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseButtonDown = FALSE;

	CView::OnLButtonUp(nFlags, point);
}


void CVolumeDeskView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseButtonDown) {
		int xMove = point.x - m_mousePos.x;
		int yMove = point.y - m_mousePos.y;
		m_pGraphics->OnMouseMove(xMove, yMove);
		m_mousePos = point;
	}

	CView::OnMouseMove(nFlags, point);
	Invalidate(FALSE);
}



void CVolumeDeskView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	OutputDebugStringW(L"CVolumeDeskView::OnKeyDown called\n");
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CVolumeDeskView::SetThreshold(float threshold)
{
	if (m_pGraphics != NULL)
	{
		m_pGraphics->SetNewThreshold(threshold);
		Invalidate(FALSE);
	}
}


void CVolumeDeskView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	Invalidate(FALSE);
}

