#pragma once
#include "Dialog.h"

/*
*	用于计算一个地址在PE文件中的位置
*	可通过FOA、VA与RVA三类地址计算，且验证其合法性
*/

class CalculateDlg : public DialogEX
{
public:
	CalculateDlg(HWND hParent = nullptr);
	virtual ~CalculateDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void InitRadioButton();
	void OnClickVA();
	void OnClickRVA();
	void OnClickFOA();
	void OnClickCalc();

	void FromVACalc(DWORD VA);
	void FromRVACalc(DWORD RVA);
	void FromFOACalc(DWORD FOA);

private:
	static CalculateDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK CalcProc(HWND hCalc, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK RvaNewProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK VaNewProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK FoaNewProc(HWND, UINT, WPARAM, LPARAM);

	static BOOL IsTcharHex(TCHAR tCh);
};

