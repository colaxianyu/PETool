module;

#include <windows.h>

export module ImportDlg;

import STL;
import Dialog;
import ListControl;

export class ImportDlg : public DialogEX
{
public:
	ImportDlg(HWND hParent = nullptr);
	virtual ~ImportDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void InitMainList();
	void PlantMainColumn();
	void PlantMainItem();

	void InitFuncList();
	void PlantFuncColumn();
	void PlantFuncItem();

	DWORD GetFirstThunkFromMainList(DWORD rowID);
private:
	static ImportDlg* thisDlg_;
	std::unique_ptr<ListCrl> mainList_ = nullptr;
	std::unique_ptr<ListCrl> funcList_ = nullptr;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK ImportProc(HWND hImport, UINT message, WPARAM wParam, LPARAM lParam);
};