module;

#include <windows.h>

export module ExportDlg;

import STL;
import Dialog;
import ListControl;

export class ExportDlg : public DialogEX
{
public:
	ExportDlg(HWND hParent = nullptr);
	virtual ~ExportDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void SetExportInfo();
	void InitFuncList();
	void PlantFuncColumn();
	void PlantFuncItem();


private:
	static ExportDlg* thisDlg_;
	std::unique_ptr<ListCrl> funcList_ = nullptr;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK ExportProc(HWND hExport, UINT message, WPARAM wParam, LPARAM lParam);
};