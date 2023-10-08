#pragma once
#include "Dialog.h"
#include "ListControl.h"
#include "AnalysePE.h"
#include <memory>

class RelocationDlg : public DialogEX
{
public:
	RelocationDlg(HWND hParent = nullptr);
	virtual ~RelocationDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;

	void SetRelocationPtr() { relocation_ = AnalysePE::GetAnalyse().GetRelocation(); }
	void InitBlockList();
	void PlantBlockColumn();
	void PlantBlockItem();

	void InitBlockItemList();
	void PlantBlockItemColumn();
	void PlantBlockItemItem();

	DWORD GetRelocationTableIndex(DWORD rowID);
private:
	static RelocationDlg* thisDlg_;
	std::unique_ptr<ListCrl> blockList_;
	std::unique_ptr<ListCrl> blockItemList_;

	IMAGE_BASE_RELOCATION* relocation_;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK RelocationProc(HWND hRelocation, UINT message, WPARAM wParam, LPARAM lParam);
};