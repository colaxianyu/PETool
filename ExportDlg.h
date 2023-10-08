#pragma once
#include "Dialog.h"
#include "ListControl.h"
#include "AnalysePE.h"
#include <memory>

class ExportDlg : public DialogEX
{
public:
	ExportDlg(HWND hParent = nullptr);
	virtual ~ExportDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetExportPtr() { export_ = AnalysePE::GetAnalyse().GetExport(); };
	void SetExportInfo();
	void InitFuncList();
	void PlantFuncColumn();
	void PlantFuncItem();

	
private:
	static ExportDlg* thisDlg_;
	std::unique_ptr<ListCrl> funcList_;
	IMAGE_EXPORT_DIRECTORY* export_;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK ExportProc(HWND hExport, UINT message, WPARAM wParam, LPARAM lParam);
};