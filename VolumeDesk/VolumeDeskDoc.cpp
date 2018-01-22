
// VolumeDeskDoc.cpp : implementation of the CVolumeDeskDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VolumeDesk.h"
#endif

#include "VolumeDeskDoc.h"
#include "VolumeDeskView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVolumeDeskDoc

IMPLEMENT_DYNCREATE(CVolumeDeskDoc, CDocument)

BEGIN_MESSAGE_MAP(CVolumeDeskDoc, CDocument)
END_MESSAGE_MAP()


// CVolumeDeskDoc construction/destruction

CVolumeDeskDoc::CVolumeDeskDoc()
{
	// TODO: add one-time construction code here

}

CVolumeDeskDoc::~CVolumeDeskDoc()
{
}

BOOL CVolumeDeskDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//	Add the axis object to the document.
	// All have this as a non-persisted object that renders the wireframe cube that encompasses the rendered object(s).
	pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->GetObjectTree()->AddDrawableObject(NULL);

	// Loop through views (but there should only be one in an SDI
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CVolumeDeskView* pView = (CVolumeDeskView *)GetNextView(pos);
		pView->UpdateWindow();
	}

	return TRUE;
}




// CVolumeDeskDoc serialization

void CVolumeDeskDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CVolumeDeskDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CVolumeDeskDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CVolumeDeskDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CVolumeDeskDoc diagnostics

#ifdef _DEBUG
void CVolumeDeskDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVolumeDeskDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVolumeDeskDoc commands
