module;

#include <windows.h>

export module TimeStampDlg;

import Dialog;

export class TimeStampDlg : public DialogEX {
public:
	TimeStampDlg(HWND hParent = nullptr);
	virtual ~TimeStampDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetTSDlgInfo();

private:
	static TimeStampDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK TsProc(HWND hAbout, UINT message, WPARAM wParam, LPARAM lParam);
};