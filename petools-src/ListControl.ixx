module;

#include <windows.h>
#include <CommCtrl.h>
export module ListControl;

import STL;

export using widthAndName = std::pair<UINT, const TCHAR*>;
using petools::show::ItemWidthAndName;

export class ListCrl
{
public:
	ListCrl(HWND hList, std::function<void()> plantColumn, std::function<void()> plantItem);
	~ListCrl() { OutputDebugString(L"ListCrl is released\n");; };
	void Init(UINT columnMask, UINT itemMask);

	template<typename WidthType, typename NameType>
	void SetColumn(ItemWidthAndName<WidthType, NameType>& items, int col) {
		if constexpr (std::same_as<NameType, std::string>) {
			column_.pszText = const_cast<LPWSTR>(std::wstring(items.name_.begin(), items.name_.end()).c_str());
		}
		else{
			column_.pszText = const_cast<LPWSTR>(items.name_.c_str());
		}
		column_.cx = items.width_;
		column_.iSubItem = col;
	}

	void PlantColumn() { plantColumn_(); };

	void PlantItem() { plantItem_(); }

	HWND GetList() { return hList_; }

	DWORD GetColumnAddr() { return  DWORD(&column_); }
	DWORD GetItemAddr() { return  DWORD(&item_); }
private:
	HWND hList_;
	LV_COLUMN column_;
	LV_ITEM item_;

	std::function<void()> plantColumn_;
	std::function<void()> plantItem_;
};
