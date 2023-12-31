#pragma once
#include <windows.h>

#include "Dialog.h"
#include "ListControl.h"
#include "AboutDlg.h"
#include "PeEdit.h"
#include "FileManage.h"
#include "ProtectorDlg.h"

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
	void CreateProtectorDlg();

	DWORD GetPID(int rowID);

	BOOL GetOpenFileNameEx(TCHAR* fileName);
private:
	HICON hIcon_;
	std::unique_ptr<ListCrl> processList_ = nullptr;
	std::unique_ptr<ListCrl> moduleList_ = nullptr;
	std::unique_ptr<AboutDlg> aboutDlg_ = nullptr;
	std::unique_ptr<PeEditDlg> peEditDlg_ = nullptr;
	std::unique_ptr<ProtectorDlg> protectorDlg_ = nullptr;

	static MainDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK MianProc(HWND hMain, UINT message, WPARAM wParam, LPARAM lParam);
};

