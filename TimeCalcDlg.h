#pragma once
#include "Dialog.h"

class TimeCalcDlg : public DialogEX {
public:
	TimeCalcDlg(HWND hParent = nullptr);
	virtual ~TimeCalcDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void InitEditInfo();
private:
	time_t timeStamp_;

	static TimeCalcDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static BOOL IsTcharHex(TCHAR tCh);

	static LRESULT CALLBACK StampProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK YMDProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK HMSProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK TimeCalcProc(HWND hTimeCalc, UINT message, WPARAM wParam, LPARAM lParam);
};