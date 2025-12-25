module;

#include <windows.h>
#include <Tlhelp32.h>
#include <format>
#include <algorithm>

export module Utility;

import STL;
import std.compat;
import Concept;

using std::array;
using std::string;
using std::wstring;

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

export namespace petools::utility {

	template <typename T>
	bool is_zero_structure(const T* obj) {
		static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>, "Not trivial and standard-layout!");

		return std::all_of(
			reinterpret_cast<const char*>(obj),
			reinterpret_cast<const char*>(obj) + sizeof(T),
			[](char byte) { return byte == 0; });
	}

	void set_dlg_item_text(HWND hwnd, int item_id, DWORD value, DWORD show_length) {
		
		std::wstring formatted_text = std::format(L"{:0{}X}", value, show_length);

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
		return is_zero_structure(obj);
	}
	template <CanRanges T>
	auto make_ranges(T* obj) {
		return std::ranges::subrange(obj, zero_sentinel<T>{});
	}


	struct ProcessInfo {
		DWORD       pid{};
		std::wstring exe_name;
		DWORD       image_base{};
		DWORD       image_size{};
	};

	[[nodiscard]] inline std::vector<ProcessInfo> EnumProcesses() noexcept {
		std::vector<ProcessInfo> result;

		PROCESSENTRY32 process{};
		process.dwSize = sizeof(process);

		HANDLE process_snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (process_snap == INVALID_HANDLE_VALUE) {
			return result;
		}

		MODULEENTRY32 module{};
		module.dwSize = sizeof(module);

		if (::Process32First(process_snap, &process)) {
			do {
				HANDLE module_snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);
				if (module_snap == INVALID_HANDLE_VALUE) {
					continue;
				}

				if (::Module32First(module_snap, &module)) {
					ProcessInfo info{};
					info.pid = process.th32ProcessID;
					info.exe_name = process.szExeFile;
					info.image_base = reinterpret_cast<DWORD>(module.modBaseAddr);
					info.image_size = module.modBaseSize;

					result.push_back(info);
				}

				::CloseHandle(module_snap);
			} while (::Process32Next(process_snap, &process));
		}

		::CloseHandle(process_snap);
		return result;                                        
	}

	struct ModuleInfo {
		std::wstring path;
		DWORD        size;
	};

	[[nodiscard]] inline std::vector<ModuleInfo> EnumModules(DWORD pid) {
		std::vector<ModuleInfo> result;

		HANDLE h_module = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if (h_module == INVALID_HANDLE_VALUE) {
			return result;
		}

		MODULEENTRY32 module{};
		module.dwSize = sizeof(module);

		if (::Module32First(h_module, &module)) {
			do {
				ModuleInfo info;
				info.path = module.szExePath;
				info.size = module.modBaseSize; 

				result.push_back(info);
			} while (::Module32Next(h_module, &module));
		}

		::CloseHandle(h_module);
		return result;
	}

	enum class PidParseError {
		invalid_format
	};

	inline std::expected<DWORD, PidParseError>
		ParsePidHex(std::wstring_view pid_text) noexcept {
		auto value_opt = wstring_to_dword(pid_text, numeric_base::hex{});
		if (!value_opt.has_value()) {
			return std::unexpected(PidParseError::invalid_format);
		}
		return *value_opt;
	}

} // namespace petools::utility

