#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include <memory>
#include <vector>
#include <functional>

typedef std::pair<UINT, const TCHAR*> widthAndName;;

class ListCrl
{
public:
	ListCrl(HWND hList, std::function<void()> plantColumn, std::function<void()> plantItem);
	void Init(UINT columnMask, UINT itemMask);

	void SetColumn(widthAndName& items, int col);

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

