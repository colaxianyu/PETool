#pragma once
#include "Dialog.h"
#include "ExportDlg.h"
#include "ImportDlg.h"
#include "RelocationDlg.h"

#include <memory>

class DirectoryDlg : public DialogEX
{
public:
	DirectoryDlg(HWND hParent = nullptr);
	virtual ~DirectoryDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void CreateExpertDlg();
	void CreateImportDlg();
	void CreateRelocationDlg();

	void SetDirHeaderInfo();
private:
	std::unique_ptr<ExportDlg> exportDlg_;
	std::unique_ptr<ImportDlg> importDlg_;
	std::unique_ptr<RelocationDlg> relocationDlg_;

	static DirectoryDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK DirProc(HWND hDir, UINT message, WPARAM wParam, LPARAM lParam);
};

