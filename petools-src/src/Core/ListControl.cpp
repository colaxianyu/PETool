module;

#include <windows.h>
#include <CommCtrl.h>

module ListControl;

namespace petools {

	ListError ListCtrl::MakeError(ListErrorOP op, int index, LRESULT result) noexcept
	{
		return ListError{
			.op = op,
			.index = index,
			.result = result,
			.win32Err = ::GetLastError(),
		};
	}

	std::expected<void, ListError> ListCtrl::Init(UINT column_mask, UINT item_mask, DWORD extended_style) noexcept {
		if (!list_hwnd_.get() || !::IsWindow(list_hwnd_.get())) {
			return std::unexpected(MakeError(ListErrorOP::SET_EXTENDED_STYLE, -1, 0));
		}

		column_mask_ = column_mask;
		item_mask_ = item_mask;
		ListView_SetExtendedListViewStyleEx(list_hwnd_.get(), extended_style, extended_style);

		return {};
	}

	std::expected<void, ListError> ListCtrl::SetColumn(std::span<ColumnDefinition> columns, int fmt) noexcept {
		if (columns.empty()) {
			return std::unexpected(MakeError(ListErrorOP::INSERT_COLUMN, -1, 0));
		}

		if (!list_hwnd_.get() || !::IsWindow(list_hwnd_.get())) {
			return std::unexpected(MakeError(ListErrorOP::INSERT_COLUMN, -1, 0));
		}

		for (size_t i = 0; i < columns.size(); ++i) {
			LVCOLUMNW column{};
			column.mask = column_mask_;
			column.fmt = fmt;
			column.cx = static_cast<int>(columns[i].width_);
			column.iSubItem = static_cast<int>(i);
			column.pszText = const_cast<LPWSTR>(columns[i].name_.c_str());

			const LRESULT r = SendMessageW(
				list_hwnd_.get(),
				LVM_INSERTCOLUMNW,
				static_cast<WPARAM>(i),
				reinterpret_cast<LPARAM>(&column));

			if (r == -1) {
				return std::unexpected(MakeError(ListErrorOP::INSERT_COLUMN, static_cast<int>(i), r));
			}
		}

		return {};
	}

	std::expected<void, ListError> ListCtrl::SetItem(std::span<ItemDefinition> items, size_t row) noexcept {
		if (items.empty()) {
			return std::unexpected(MakeError(ListErrorOP::SET_ITEM, static_cast<int>(row), 0));
		}

		if (!list_hwnd_.get() || !::IsWindow(list_hwnd_.get())) {
			return std::unexpected(MakeError(ListErrorOP::SET_ITEM, static_cast<int>(row), 0));
		}

		const ItemDefinition* primary = nullptr;
		for (const auto& it : items) {
			if (it.sub_item_index_ == 0) {
				primary = &it;
				break;
			}
		}

		if (!primary) {
			return std::unexpected(MakeError(ListErrorOP::SET_ITEM, static_cast<int>(row), 0));
		}

		LVITEMW item{};
		item.mask = item_mask_;
		item.iItem = static_cast<int>(row);
		item.iSubItem = 0;
		item.pszText = const_cast<LPWSTR>(primary->text_.c_str());

		const LRESULT inserted = SendMessageW(
			list_hwnd_.get(),
			LVM_INSERTITEMW,
			0,
			reinterpret_cast<LPARAM>(&item));

		if (inserted == -1) {
			return std::unexpected(MakeError(ListErrorOP::SET_ITEM, static_cast<int>(row), inserted));
		}

		for (const auto& it : items) {
			if (it.sub_item_index_ == 0) {
				continue;
			}

			LVITEMW others{};
			others.mask = item_mask_;
			others.iItem = static_cast<int>(inserted);
			others.iSubItem = static_cast<int>(it.sub_item_index_);
			others.pszText = const_cast<LPWSTR>(it.text_.c_str());

			const BOOL ok = ListView_SetItem(list_hwnd_.get(), &others);
			if (!ok) {
				return std::unexpected(MakeError(ListErrorOP::SET_ITEM, static_cast<int>(it.sub_item_index_), ok));
			}
		}

		return {};
	}

	std::expected<void, ListError> ListCtrl::ClearItems() noexcept
	{
		LRESULT r = SendMessageW(list_hwnd_.get(), LVM_DELETEALLITEMS, 0, 0);
		if (r == FALSE) {
			return std::unexpected(MakeError(ListErrorOP::DELETE_ALL_ITEMS, -1, r));
		}
		return {};
	}

	std::optional<int> ListCtrl::SelectedIndex() const noexcept
	{
		int idx = ListView_GetNextItem(list_hwnd_.get(), -1, LVNI_SELECTED);
		if (idx == -1) return std::nullopt;
		return idx;
	}

} //namespace petools