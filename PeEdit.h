#pragma once
#include "Dialog.h"
#include "FileHeaderDlg.h"
#include "OptHeaderDlg.h"
#include "SecHeaderDlg.h"
#include "DirectoryDlg.h"
#include "CalculateDlg.h"
#include "TimeCalcDlg.h"
#include "InjectImportDlg.h"

class PeEditDlg : public DialogEX
{
	//friend class FileHeader;
public:
	PeEditDlg(HWND hParent = nullptr);
	virtual ~PeEditDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetFileName(TCHAR* fileName);
	void SetEditTitle();
	void SetPEMainInfo();
	
	void CreateFileDlg();
	void CreateOptDlg();
	void CreateSecDlg();
	void CreateDirDlg();
	void CreateCalcDlg();
	void CreateTimeCalcDlg();
	void CreateInjectImportDlg();

	void SaveFile();
private:
	TCHAR fileName_[FILENAME_MAX];

	std::unique_ptr<FileHeaderDlg> fileHeaderDlg_;
	std::unique_ptr<OptHeaderDlg> optHeaderDlg_;
	std::unique_ptr<SecHeaderDlg> secHeaderDlg_;
	std::unique_ptr<DirectoryDlg> dirHeaderDlg_;
	std::unique_ptr<CalculateDlg> calcDlg_;
	std::unique_ptr<TimeCalcDlg> timeCalcDlg_;
	std::unique_ptr<InjectImportDlg> injectImpDlg_;

	static PeEditDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK EditProc(HWND hEdit, UINT message, WPARAM wParam, LPARAM lParam);
};
