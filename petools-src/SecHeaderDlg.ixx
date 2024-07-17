module;

#include <windows.h>

export module SecHeaderDlg;

import Dialog;
import STL;
import ListControl;


export class SecHeaderDlg : public DialogEX
{
public:
	SecHeaderDlg(HWND hParent = nullptr);
	virtual ~SecHeaderDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void InitSectionList();

	void PlantSectionColumn();
	void PlantSectionItem();
private:
	std::unique_ptr<ListCrl> sectionList_ = nullptr;

	static SecHeaderDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK SectionProc(HWND hSec, UINT message, WPARAM wParam, LPARAM lParam);
};

