module;

#include <windows.h>

export module Dialog;

import STL;
using std::wstring;
using std::function;

using DlgProc = LRESULT(CALLBACK* )(HWND, UINT, WPARAM, LPARAM);

export class DialogEX
{
protected:
	INT idTemplate_;
	HWND hCurrentDlg_;
	HWND hParentDlg_;
public:
	DialogEX(INT idTemplate, HWND hParent = nullptr);
	virtual ~DialogEX() {};
	virtual void InitDlg() = 0;
	virtual void Plant() = 0;
	virtual void SetCurrentDlgHWND(HWND hDlg);
	virtual void CloseDlg();

	template<typename DlgType, typename DlgList, typename CheckFunction = function<bool()>>
	void CreateDlg(HWND hParent, DlgList& list, wstring err_msg = L"", CheckFunction func = []() { return true; }) {
		if (!func()) {
			MessageBox(hParent, err_msg.c_str(), L"Error", MB_OK);
			return;
		}

		auto dlg = std::make_unique<DlgType>(hParent);
		dlg->InitDlg();
		dlg->Plant();
		list.push_back(std::move(dlg));
	}
};
