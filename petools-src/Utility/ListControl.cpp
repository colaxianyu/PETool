module;

#include <windows.h>
#include <CommCtrl.h>

module ListControl;

ListCtrl::ListCtrl(HWND h_list, std::function<void()> plant_column, std::function<void()> plant_item) noexcept
	: h_list_(h_list),
	plant_column_(std::move(plant_column)),
	plant_item_(std::move(plant_item))

{
	memset(&column_, 0, sizeof(column_));
	memset(&item_, 0, sizeof(item_));
}

void ListCtrl::init(UINT column_mask, UINT item_mask) noexcept {
	SendMessage(h_list_, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	column_.mask = column_mask;
	column_.fmt = LVCFMT_CENTER;
	item_.mask = item_mask;
}

void ListCtrl::set_column(std::span<ColumnDefinition> array) noexcept {
	for (std::size_t i = 0; i < array.size(); i++) {
		column_.pszText = const_cast<LPWSTR>(array[i].name_.c_str());
		column_.cx = array[i].width_;
		column_.iSubItem = i;
		SendMessage(h_list_, LVM_INSERTCOLUMN, i, reinterpret_cast<LPARAM>(&column_));
	}
}

void ListCtrl::set_item(std::vector<ItemDefinition> vector, std::size_t row) noexcept {
	for (std::size_t i = 0; i < vector.size(); i++) {
		item_.pszText = const_cast<LPWSTR>(vector[i].text_.c_str());
		item_.iSubItem = vector[i].sub_item_index_;
		item_.iItem = row;
		if (i == 0) {
			SendMessage(h_list_, LVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&item_));
		}
		else {
			ListView_SetItem(h_list_, reinterpret_cast<LPARAM>(&item_));
		}
		
	}
}