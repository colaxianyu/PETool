#pragma once
#include "windows.h"

typedef LRESULT(CALLBACK* DlgProc)(HWND, UINT, WPARAM, LPARAM);

class DialogEX
{
protected:
	INT idTemplate_;
	HWND hCurrentDlg_;
	HWND hParentDlg_;
public:
	DialogEX(INT idTemplate, HWND hParent = nullptr);
	virtual ~DialogEX() {};
	virtual void InitDlg() = 0;
	virtual void Plant() = 0;
	virtual void SetCurrentDlgHWND(HWND hDlg);
	virtual void CloseDlg();
};

