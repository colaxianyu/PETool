#pragma once
#include "Dialog.h"
#include "ListControl.h"
#include "AnalysePE.h"
#include <memory>

class ImportDlg : public DialogEX
{
public:
	ImportDlg(HWND hParent = nullptr);
	virtual ~ImportDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetImportPtr();

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

	IMAGE_IMPORT_DESCRIPTOR* import_ = nullptr;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK ImportProc(HWND hImport, UINT message, WPARAM wParam, LPARAM lParam);
};