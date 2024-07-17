module;

#include <windows.h>

export module RelocationDlg;

import STL;
import Dialog;
import ListControl;

export class RelocationDlg : public DialogEX
{
public:
	RelocationDlg(HWND hParent = nullptr);
	virtual ~RelocationDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void InitBlockList();
	void PlantBlockColumn();
	void PlantBlockItem();

	void InitBlockItemList();
	void PlantBlockItemColumn();
	void PlantBlockItemItem();

	DWORD GetRelocationTableIndex(DWORD rowID);
private:
	static RelocationDlg* thisDlg_;
	std::unique_ptr<ListCrl> blockList_ = nullptr;
	std::unique_ptr<ListCrl> blockItemList_ = nullptr;

	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK RelocationProc(HWND hRelocation, UINT message, WPARAM wParam, LPARAM lParam);
};