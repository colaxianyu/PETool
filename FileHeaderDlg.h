#pragma once
#include "Dialog.h"
#include "AnalysePE.h"
#include "TimeStampDlg.h"

class FileHeaderDlg : public DialogEX
{
public:
	FileHeaderDlg(HWND hParent = nullptr);
	virtual ~FileHeaderDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetFileHeaderInfo();

	void CreateTimeDlg();
private:
	std::unique_ptr<TimeStampDlg> timeStampDlg_;

	static FileHeaderDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK FileHeaderProc(HWND hFile, UINT message, WPARAM wParam, LPARAM lParam);
};

