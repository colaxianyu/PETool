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

    // Containers
    using std::vector;
    using std::stack;
    using std::array;
    using std::span;
    using std::unordered_set;
    using std::unordered_map;

    // String Types
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
	using std::byte;

    namespace chrono {
        using std::chrono::time_point;
        using std::chrono::system_clock;
    }
}

export namespace numeric_base {
	using hex = std::integral_constant<int, 16>;
	using dec = std::integral_constant<int, 10>;
	using oct = std::integral_constant<int, 8>;
	using bin = std::integral_constant<int, 2>;
} // namespace numeric_base

export namespace tools {

	namespace show {
		void SetDlgItemText_t(HWND hDlg, int item_id, DWORD value, DWORD show_lenth) {
			TCHAR text[32] = { 0 };
			_snwprintf_s(text, sizeof(text) / sizeof(TCHAR), L"%0*X", show_lenth, value);
			::SetDlgItemText(hDlg, item_id, text);
		};
	}

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


namespace petools {
	export{
#define STRING_CONVERT_WIN32 1
#define STRING_CONVERT_STD 2

#ifndef STRING_CONVERT_IMPL
#ifdef _WIN32
#define STRING_CONVERT STRING_CONVERT_WIN32
#else
#define STRING_CONVERT STRING_CONVERT_STD
#endif
#endif

#if STRING_CONVERT == STRING_CONVERT_WIN32
		inline std::wstring string_to_wstring(std::string_view str) {
			std::wstring wstr;
			wstr.resize(MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), nullptr, 0));
			MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), &wstr[0], wstr.size());
			return wstr;
		}
		inline std::string wstring_to_string(std::wstring_view wstr) {
			std::string str;
			str.resize(WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), nullptr, 0, nullptr, nullptr));
			WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), &str[0], str.size(), nullptr, nullptr);
			return str;
		}
#elif STRING_CONVERT == STRING_CONVERT_STD
		inline std::wstring string_to_wstring(std::string_view str) {
			return std::wstring(str.begin(), str.end());
		}
		inline std::string wstring_to_string(std::wstring_view wstr) {
			return std::string(wstr.begin(), wstr.end());
		}
#endif

		std::optional<DWORD> string_to_dword(std::string_view str, int base = numeric_base::dec{}) {
			DWORD value = 0;

			auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);

			if (ec == std::errc()) {
				return value;
			}
			else {
				return std::nullopt;
			}
		}

		std::wstring to_wstring_hex(DWORD value, DWORD width) {
			return std::format(L"{:0{}X}", value, width);
		}

		std::optional<DWORD> wstring_to_dword(std::wstring_view wstr, int base = numeric_base::dec{}) {
			return string_to_dword(wstring_to_string(wstr), base);
		}

		std::optional<std::string> choose_file(HWND parent_handle) {
			std::wstring file_path(MAX_PATH, L'\0');
			OPENFILENAME open_file;
			memset(&open_file, 0, sizeof(open_file));

			open_file.lStructSize = sizeof(OPENFILENAME);
			open_file.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			open_file.hwndOwner = parent_handle;
			open_file.lpstrFilter = tools::config::format_filter;
			open_file.lpstrFile = &file_path[0];
			open_file.nMaxFile = MAX_PATH;

			if (GetOpenFileName(&open_file) == FALSE) {
				return std::nullopt;
			}

			return wstring_to_string(file_path);
		}
	}
}	// namespace petools