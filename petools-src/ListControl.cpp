module;

#include <windows.h>
#include <CommCtrl.h>

module ListControl;

ListCrl::ListCrl(HWND hList, std::function<void()> plantColumn, std::function<void()> plantItem)
	: hList_(hList),
	plantColumn_(plantColumn),
	plantItem_(plantItem)

{
	memset(&column_, 0, sizeof(column_));
	memset(&item_, 0, sizeof(item_));
}

void ListCrl::Init(UINT columnMask, UINT itemMask) {
	SendMessage(hList_, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	column_.mask = columnMask;
	column_.fmt = LVCFMT_CENTER;
	item_.mask = itemMask;
}
