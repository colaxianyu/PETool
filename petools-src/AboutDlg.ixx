module;

#include <windows.h>

export module AboutDlg;

import Dialog;

export class AboutDlg : public DialogEX
{
public:
	AboutDlg(HWND hParent = nullptr);
	virtual ~AboutDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
private:
	static AboutDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK AboutProc(HWND hAbout, UINT message, WPARAM wParam, LPARAM lParam);
};