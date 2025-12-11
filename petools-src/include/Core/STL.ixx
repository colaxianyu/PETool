module;

#include <windows.h>
#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <vector>
#include <array>
#include <stack>
#include <span>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <optional>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <source_location>

export module STL;

import std;

// Group related using declarations by category
export namespace std {
    // Smart Pointers
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::enable_shared_from_this;

    using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;

    // Containers
    using std::vector;
    using std::stack;
    using std::array;
    using std::span;
	using std::variant;
    using std::unordered_set;
    using std::unordered_map;

    // String Types
    using std::basic_string;
    using std::string;
    using std::string_view;
    using std::wstring;
    using std::wstring_view;

    // Time Types
    using std::time_t;
    using std::tm;

    // Type Traits and Utilities
    using std::same_as;
    using std::void_t;
    using std::false_type;
    using std::true_type;
    using std::derived_from;
    using std::conditional_t;
    using std::constructible_from;
    using std::integral_constant;
    using std::is_integral_v;
    using std::is_unsigned_v;
    using std::is_base_of_v;
    using std::is_invocable_r_v;


    // Function and Binding
    using std::function;
    using std::bind;
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;

    // Miscellaneous
    using std::optional;
    using std::size_t;
    using std::pair;
    using std::atomic;
    using std::atomic_int;
    using std::uint8_t;
    using std::source_location;
    using std::cout;

    // Utility Functions
    using std::declval;
    using std::find_if;
    using std::exchange;
    using std::any_of;
    using std::to_address;
    using std::forward;
    using std::move;
    using std::make_shared;
    using std::make_unique;
	using std::make_optional;
    using std::ceil;
    using std::nullopt;
    using std::visit;
	using std::byte;

    namespace chrono {
        using std::chrono::time_point;
        using std::chrono::system_clock;
    }

	namespace ranges {
		using std::ranges::subrange;
	}
}

export namespace tools {

	namespace config {
		constexpr unsigned int filename_max = 260;
		constexpr TCHAR format_filter[50] = TEXT("*.exe;*.dll;*.scr;*.drv;*.sys");
	}

	void CharToTchar(const char* in, TCHAR** out) {
		DWORD lenth = MultiByteToWideChar(CP_ACP, 0, in, strlen(in) + 1, nullptr, 0);
		TCHAR* temp = new TCHAR[sizeof(TCHAR) * lenth];
		MultiByteToWideChar(CP_ACP, 0, in, strlen(in) + 1, temp, lenth);
		*out = temp;
		temp = nullptr;
	}

	void TcharToChar(const TCHAR* in, char** out) {
		DWORD lenth = WideCharToMultiByte(CP_ACP, 0, in, -1, nullptr, 0, nullptr, FALSE);
		char* temp = new char[sizeof(char) * lenth];
		WideCharToMultiByte(CP_ACP, 0, in, -1, temp, lenth, nullptr, FALSE);
		*out = temp;
		temp = nullptr;
	}

	const std::string TcharToString(const TCHAR* in) {
		char* tempChar = nullptr;
		TcharToChar(in, &tempChar);
		std::string s = tempChar;
		return s;
	}

	void TcharToDword(const TCHAR* in, DWORD* out, int base) {
		TCHAR* end;
		*out = wcstol(in, &end, base);
	}

	namespace win32 {
		void show_error_message(HWND h_WND, std::wstring error_message) {
			MessageBox(h_WND, error_message.c_str(), L"ERROR", MB_OK);
		}

		[[nodiscard]] std::wstring get_dlg_item_text_as_wstring(HWND h_dlg, int id_dlg_item) {
			int text_length = GetWindowTextLength(GetDlgItem(h_dlg, id_dlg_item));
			if (text_length == 0) {
				return L"";
			}

			std::wstring text;
			text.resize(text_length + 1);

			GetDlgItemText(h_dlg, id_dlg_item, &text[0], text_length + 1);

			text.resize(text_length);
			return text;
		}

		[[nodiscard]] bool set_dlg_item_text_from_wstring(HWND h_dlg, int id_dlg_item, const std::wstring& text) {
			return SetDlgItemText(h_dlg, id_dlg_item, text.c_str());
		}

		void set_IDC_EDIT_read_only(HWND h_dlg, int edit_id) {
			constexpr WPARAM read_only_mode = 1;
			SendDlgItemMessage(h_dlg, edit_id, EM_SETREADONLY, read_only_mode, 0);
		}

		void set_IDC_EDIT_text_limit(HWND h_dlg, int edit_id, int limit) {
			SendDlgItemMessage(h_dlg, edit_id, EM_LIMITTEXT, limit, 0);
		}
	}
}