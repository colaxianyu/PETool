module;

#include <windows.h>

export module ProtectorDlg;

import FileManage;
import Dialog;
import STL;

using std::unique_ptr;

export class ProtectorDlg : public DialogEX
{
public:
	ProtectorDlg(HWND hParent = nullptr);
	virtual ~ProtectorDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void SelectProtector();
	void SelectRawProcess();

	void ReadProtectorToBuffer();
	void ReadRawProcessToBuffer();

	char* EncodeRawProcess();
	void ProtectProcess();

	void SaveFile();
private:
	unique_ptr<FileManage> protector_ = nullptr;
	unique_ptr<FileManage> rawProcess_ = nullptr;
	unique_ptr<char[]> protectorBuffer_ = nullptr;
	unique_ptr<char[]> rawProcessBuffer_ = nullptr;

	static ProtectorDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK ProtectorProc(HWND hProtector, UINT message, WPARAM wParam, LPARAM lParam);
};