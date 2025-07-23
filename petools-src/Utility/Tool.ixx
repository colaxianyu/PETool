module;

#include <windows.h>
#include <format>
#include <algorithm>

export module Tool;

import STL;
import Concept;

using std::array;
using std::string;
using std::wstring;

namespace petools {

	export{

		namespace tool {

			template <typename T>
			bool is_zero_structure(const T* obj) {
				static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>, "Not trivial and standard-layout!");

				return std::all_of(
					reinterpret_cast<const char*>(obj),
					reinterpret_cast<const char*>(obj) + sizeof(T),
					[](char byte) { return byte == 0; });
			}

			void set_dlg_item_text(HWND hwnd, int item_id, DWORD value, DWORD show_length) {
				// 使用std::format直接格式化宽字符串
				std::wstring formatted_text = std::format(L"{:0{}X}", value, show_length);

				// 将格式化后的字符串传递给SetDlgItemText
				::SetDlgItemText(hwnd, item_id, formatted_text.c_str());
			}

			wstring string_to_wstring(std::string_view str, UINT code_page = CP_ACP) {
				if (str.empty()) {
					return L"";
				}

				const int length = ::MultiByteToWideChar(
					code_page, 0,
					str.data(), str.size(),
					nullptr, 0
				);

				if (length <= 0) {
					return L"";
				}

				std::wstring wstr(length, L'\0');

				::MultiByteToWideChar(
					code_page, 0,
					str.data(), str.size(),
					wstr.data(), length
				);

				return wstr;
			}

			string wstring_to_string(std::wstring_view wstr, UINT code_page = CP_ACP) {
				if (wstr.empty()) {
					return "";
				}

				const int length = ::WideCharToMultiByte(
					code_page, 0,
					wstr.data(), wstr.size(),
					nullptr, 0,
					nullptr, nullptr
				);

				if (length <= 0) {
					return "";
				}

				string str(length, '\0');

				::WideCharToMultiByte(
					code_page, 0,
					wstr.data(), wstr.size(),
					str.data(), length,
					nullptr, nullptr
				);

				return str;
			}

			std::optional<DWORD> string_to_dword(std::string_view str, uint16_t base = numeric_base::dec::value) {
				DWORD value = 0;

				auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);

				if (ec == std::errc()) {
					return value;
				}
				else {
					return std::nullopt;
				}
			}

			wstring to_wstring_hex(DWORD value, DWORD width) {
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

			template <typename T>
			struct zero_sentinel {};
			template <typename T>
			bool operator==(const T* obj, zero_sentinel<T>) {
				return tool::is_zero_structure(obj);
			}
			template <CanRanges T>
			auto make_ranges(T* obj) {
				return std::ranges::subrange(obj, zero_sentinel<T>{});
			}

		} // namespace tools

	}

} // namespace petools


module :private;

//namespace detail {
//
//	template <typename charT>
//	std::basic_string<charT> to_string_impl(auto value, int length, size_t base = petools::numeric_base::dec) {
//		constexpr if (std::same_as<charT, std::string>::value) {
//			return std::format("{:0{}X}", value, length)
//		}
//		constexpr else if (std::same_as<charT, std::wstring>::value) {
//			return std::format(L"{:0{}X}", value, length);
//		}
//	}
//
//} // namespace detail
