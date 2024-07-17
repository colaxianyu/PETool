module;

#include <windows.h>

export module FileHeaderDlg;

import STL;
import Dialog;
import TimeStampDlg;

export class FileHeaderDlg : public DialogEX
{
public:
	FileHeaderDlg(HWND hParent = nullptr);
	virtual ~FileHeaderDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void SetFileHeaderInfo();

	void CreateTimeDlg();
private:
	std::unique_ptr<TimeStampDlg> timeStampDlg_ = nullptr;

	static FileHeaderDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK FileHeaderProc(HWND hFile, UINT message, WPARAM wParam, LPARAM lParam);
};