#pragma once
#include "Dialog.h"
#include "FileHeaderDlg.h"
#include "OptHeaderDlg.h"
#include "SecHeaderDlg.h"
#include "DirectoryDlg.h"
#include "CalculateDlg.h"
#include "InjectImportDlg.h"
#include "FileManage.h"

class PeEditDlg : public DialogEX
{
public:
	PeEditDlg(HWND hParent = nullptr);
	virtual ~PeEditDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetFileName(TCHAR* fileName);
	void OpenFile(const TCHAR* filePath, const char* mdoe = "rb");
	void SetEditTitle();
	void SetPEMainInfo();
	
	void CreateFileDlg();
	void CreateOptDlg();
	void CreateSecDlg();
	void CreateDirDlg();
	void CreateCalcDlg();
	void CreateInjectImportDlg();

	void SaveFile();
	FileManage* const GetFileManage() { return fileManage_.get(); }
private:
	TCHAR fileName_[FILENAME_MAX];
	unique_ptr<FileManage> fileManage_ = nullptr;

	std::unique_ptr<FileHeaderDlg> fileHeaderDlg_ = nullptr;
	std::unique_ptr<OptHeaderDlg> optHeaderDlg_ = nullptr;
	std::unique_ptr<SecHeaderDlg> secHeaderDlg_ = nullptr;
	std::unique_ptr<DirectoryDlg> dirHeaderDlg_ = nullptr;
	std::unique_ptr<CalculateDlg> calcDlg_ = nullptr;
	std::unique_ptr<InjectImportDlg> injectImpDlg_ = nullptr;

	static PeEditDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK EditProc(HWND hEdit, UINT message, WPARAM wParam, LPARAM lParam);
};
