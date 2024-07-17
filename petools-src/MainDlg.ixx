module;

#include <windows.h>

export module MainDlg;

import STL;
import Dialog;
import ListControl;
import AboutDlg;
import ProtectorDlg;
import PeEditDlg;

using std::unique_ptr;
using std::vector;

export class MainDlg : public DialogEX
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

	vector<unique_ptr<DialogEX>> dlg_list_;
	unique_ptr<ListCrl> processList_ = nullptr;
	unique_ptr<ListCrl> moduleList_ = nullptr;

	static MainDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK MianProc(HWND hMain, UINT message, WPARAM wParam, LPARAM lParam);
};
