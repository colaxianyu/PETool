#pragma once
#include "Dialog.h"

class InjectImportDlg : public DialogEX
{
public:
	InjectImportDlg(HWND hParent = nullptr);
	virtual ~InjectImportDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SelectDll();

private:
	static InjectImportDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK InjectImportProc(HWND hInject, UINT message, WPARAM wParam, LPARAM lParam);
};