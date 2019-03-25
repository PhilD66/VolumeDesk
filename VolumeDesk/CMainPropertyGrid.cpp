#include "stdafx.h"
#include "CMainPropertyGrid.h"


CMainPropertyGrid::CMainPropertyGrid()
{
}


CMainPropertyGrid::~CMainPropertyGrid()
{
}


void CMainPropertyGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: Add your specialized code here and/or call the base class
	LPCTSTR pName = pProp->GetName();
	CString propName(pName);
	if (propName.Compare(L"Threshold") == 0)
	{
		float	threshold = _wtof(pProp->GetValue().bstrVal);
		pPropListener->PostMessageW(WM_HANDLE_PROP_CHANGE, 0, threshold);
	}


	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

void CMainPropertyGrid::setPropertiesListener(CWnd *pListenerWnd)
{
	pPropListener = pListenerWnd;
}