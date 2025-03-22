module;

#include <windows.h>
#include <CommCtrl.h>

module ListControl;

import STL;

using std::function;
using std::span;
using std::size_t;
using std::move;

namespace petools {

	ListCtrl::ListCtrl(HWND handle, function<void()> plant_column, function<void()> plant_item) noexcept
		: list_hwnd_(handle),
		plant_column_(move(plant_column)),
		plant_item_(move(plant_item))

	{
		memset(&column_, 0, sizeof(column_));
		memset(&item_, 0, sizeof(item_));
	}

	void ListCtrl::init(UINT column_mask, UINT item_mask) noexcept {
		SendMessage(list_hwnd_.get(), LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		column_.mask = column_mask;
		column_.fmt = LVCFMT_CENTER;
		item_.mask = item_mask;
	}

	void ListCtrl::set_column(span<column_definition> array) noexcept {
		for (size_t i = 0; i < array.size(); i++) {
			column_.pszText = const_cast<LPWSTR>(array[i].name_.c_str());
			column_.cx = array[i].width_;
			column_.iSubItem = i;
			SendMessage(list_hwnd_.get(), LVM_INSERTCOLUMN, i, reinterpret_cast<LPARAM>(&column_));
		}
	}

	void ListCtrl::set_item(std::vector<item_definition> vector, size_t row) noexcept {
		for (size_t i = 0; i < vector.size(); i++) {
			item_.pszText = const_cast<LPWSTR>(vector[i].text_.c_str());
			item_.iSubItem = vector[i].sub_item_index_;
			item_.iItem = row;
			if (i == 0) {
				SendMessage(list_hwnd_.get(), LVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&item_));
			}
			else {
				ListView_SetItem(list_hwnd_.get(), reinterpret_cast<LPARAM>(&item_));
			}
		}
	}

} //namespace petools