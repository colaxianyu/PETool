module;

#include <windows.h>
#include <CommCtrl.h>

export module ListControl;

import std.compat;
import WinHandle;

namespace petools {

	export struct ColumnDefinition {
		size_t width_;
		std::wstring name_;

		ColumnDefinition(size_t width, std::wstring_view name) noexcept
			: width_(width), name_(name) {
		}

		ColumnDefinition(size_t width, std::string_view name) noexcept
			: width_(width), name_(std::wstring(name.begin(), name.end())) {
		}
	};

	export struct ItemDefinition {
		size_t sub_item_index_;
		std::wstring text_;

		ItemDefinition(size_t sub_item_index, std::wstring_view text) noexcept
			: sub_item_index_(sub_item_index), text_(text) {
		}

		ItemDefinition(size_t sub_item_index, std::string_view text) noexcept
			: sub_item_index_(sub_item_index), text_(std::wstring(text.begin(), text.end())) {
		}
	};

	export enum class ListErrorOP : unsigned char
	{
		SET_EXTENDED_STYLE,
		INSERT_COLUMN,
		DELETE_COLUMN,
		DELETE_ALL_ITEMS,
		INSERT_ITEM,
		SET_ITEM
	};

	export struct ListError
	{
		ListErrorOP op{};
		int index{ -1 };
		LRESULT result{ 0 };
		DWORD win32Err{ 0 };
	};

	export class ListCtrl
	{
	public:
		explicit ListCtrl(HWND handle) noexcept : list_hwnd_(handle) {}

		[[nodiscard]] std::expected<void, ListError> Init(UINT column_mask, UINT item_mask, DWORD extended_style = LVS_EX_FULLROWSELECT) noexcept;

		[[nodiscard]] std::expected<void, ListError> SetColumn(std::span<ColumnDefinition> columns, int fmt = LVCFMT_CENTER) noexcept;
		[[nodiscard]] std::expected<void, ListError> SetItem(std::span<ItemDefinition> items, size_t row) noexcept;

		// [[nodiscard]] std::expected<void, ListError> ClearColumns() noexcept;
		[[nodiscard]] std::expected<void, ListError> ClearItems() noexcept;

		[[nodiscard]] std::optional<int> SelectedIndex() const noexcept;

		[[nodiscard]] HWND GetListHandle() const noexcept { return list_hwnd_.get(); }

	private:
		WindowHandle list_hwnd_;
		UINT column_mask_{ 0 };
		UINT item_mask_{ 0 };

		static ListError MakeError(ListErrorOP op, int index, LRESULT result) noexcept;
	};

} //namespace petools