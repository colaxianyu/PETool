module;

#include <windows.h>

export module OptHeaderDlg;

import Dialog;

export class OptHeaderDlg : public DialogEX
{
public:
	OptHeaderDlg(HWND hParent = nullptr);
	virtual ~OptHeaderDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetOptHeaderInfo();
private:
	static OptHeaderDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK OptHeaderProc(HWND hOpt, UINT message, WPARAM wParam, LPARAM lParam);
};

