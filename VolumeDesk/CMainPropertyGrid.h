#pragma once
#include <afxpropertygridctrl.h>
class CMainPropertyGrid :
	public CMFCPropertyGridCtrl
{
public:
	CMainPropertyGrid();
	~CMainPropertyGrid();
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	void setPropertiesListener(CWnd *pListenerWnd);

protected:
	CWnd *pPropListener;
};

