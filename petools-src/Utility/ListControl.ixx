module;

#include <windows.h>
#include <CommCtrl.h>
#include <string>

export module ListControl;

import STL;

export struct ColumnDefinition {
	std::size_t width_;
	std::wstring name_;

	constexpr ColumnDefinition(std::size_t width, std::wstring_view name)
		: width_(width), name_(name) {}

	constexpr ColumnDefinition(std::size_t width, std::string_view name)
		: width_(width), name_(std::wstring(name.begin(), name.end())) {}
};

export struct ItemDefinition {
	std::size_t sub_item_index_;
	std::wstring text_;

	constexpr ItemDefinition(std::size_t sub_item_index, std::wstring_view text)
		: sub_item_index_(sub_item_index), text_(text){}

	constexpr ItemDefinition(std::size_t sub_item_index, std::string_view text)
		: sub_item_index_(sub_item_index), text_(std::wstring(text.begin(), text.end())) {}
};

export class ListCtrl
{
public:
	explicit ListCtrl(HWND h_list, std::function<void()> plant_column, std::function<void()> plant_item) noexcept;

	~ListCtrl() = default;

	void init(UINT column_mask, UINT item_mask) noexcept;

	void set_column(std::span<ColumnDefinition> array) noexcept;

	void set_item(std::vector<ItemDefinition> vector, std::size_t row) noexcept;

	void plant_column() noexcept { plant_column_(); }

	void plant_item() noexcept { plant_item_(); }

	HWND get_list_handle() const noexcept { return h_list_; }
	auto get_column_addr() const noexcept { return  &column_; }
	auto get_item_addr() const noexcept { return  &item_; }

private:
	HWND h_list_;
	LV_COLUMN column_;
	LV_ITEM item_;

	std::function<void()> plant_column_;
	std::function<void()> plant_item_;
};
