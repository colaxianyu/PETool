module;

#include <windows.h>
#include <CommCtrl.h>

export module ListControl;

import STL;
import WinHandle;

using std::wstring;
using std::wstring_view;
using std::string_view;
using std::size_t;
using std::function;
using std::vector;
using std::span;

namespace petools {

	export struct column_definition {
		size_t width_;
		wstring name_;

		column_definition(size_t width, wstring_view name) noexcept
			: width_(width), name_(name) {
		}

		column_definition(size_t width, string_view name) noexcept
			: width_(width), name_(wstring(name.begin(), name.end())) {
		}
	};

	export struct item_definition {
		size_t sub_item_index_;
		wstring text_;

		item_definition(size_t sub_item_index, wstring_view text) noexcept
			: sub_item_index_(sub_item_index), text_(text) {
		}

		item_definition(size_t sub_item_index, string_view text) noexcept
			: sub_item_index_(sub_item_index), text_(wstring(text.begin(), text.end())) {
		}
	};


	export class ListCtrl
	{
	public:
		explicit ListCtrl(HWND handle, function<void()> plant_column, function<void()> plant_item) noexcept;

		void init(UINT column_mask, UINT item_mask) noexcept;

		void set_column(span<column_definition> array) noexcept;
		void set_item(vector<item_definition> vector, size_t row) noexcept;
		void plant_column() noexcept { plant_column_(); }
		void plant_item() noexcept { plant_item_(); }

		HWND get_list_handle() const noexcept { return list_hwnd_.get(); }
		auto get_column_addr() const noexcept { return &column_; }
		auto get_item_addr() const noexcept { return &item_; }

	private:
		WindowHandle list_hwnd_;
		LV_COLUMN column_;
		LV_ITEM item_;

		function<void()> plant_column_;
		function<void()> plant_item_;
	};

} //namespace petools