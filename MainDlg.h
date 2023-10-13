#pragma once
#include <windows.h>

#include "Dialog.h"
#include "ListControl.h"
#include "AboutDlg.h"
#include "PeEdit.h"
#include "FileManage.h"

class MainDlg : public DialogEX
{
public:
	MainDlg(HWND hParent = nullptr);
	virtual ~MainDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	HWND GetMainWhnd() { return hCurrentDlg_; }

	void InitProcessList();
	void InitModuleList();

	void PlantProcessItem();
	void PlantProcessColumn();
	void PlantModuleColumn();
	void PlantModuleItem();

	void CreateAboutDlg();
	void CreatePeEditDlg();

	DWORD GetPID(int rowID);

	BOOL GetOpenFileNameEx(TCHAR* fileName);
private:
	HICON hIcon_;
	std::unique_ptr<ListCrl> processList_;
	std::unique_ptr<ListCrl> moduleList_;
	std::unique_ptr<AboutDlg> aboutDlg_;
	std::unique_ptr<PeEditDlg> peEditDlg_;

	static MainDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK MianProc(HWND hMain, UINT message, WPARAM wParam, LPARAM lParam);
};

