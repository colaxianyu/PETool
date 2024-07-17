module;

#include <memory>
#include <functional>
#include <vector>
#include <iostream>
#include <format>
#include <unordered_set>
#include <chrono>
#include <windows.h>
#include <string>
#include <type_traits>
#include <array>

export module STL;

export namespace std{
	using std::unique_ptr;
	using std::vector;
	using std::function;
	using std::pair;
	using std::unordered_set;
	using std::string;
	using std::wstring;
	using std::time_t;
	using std::tm;
	using std::same_as;
	using std::array;
	using std::format;
	using std::move;
	using std::make_unique;
	using std::cout;
	using std::ceil;
	using std::format;
	using std::hex;

	namespace chrono {
		using std::chrono::time_point;
		using std::chrono::system_clock;
	}
}

export namespace petools {
	namespace show {
		template<typename WidthType, typename NameType>
		requires std::same_as<NameType, std::string> || std::same_as<NameType, std::wstring>
		struct ItemWidthAndName {
			WidthType width_;
			NameType name_;
		};

		void SetDlgItemText_t(HWND hDlg, int item_id, DWORD value, DWORD show_lenth) {
			TCHAR text[32] = { 0 };
			_snwprintf_s(text, sizeof(text) / sizeof(TCHAR), L"%0*X", show_lenth, value);
			::SetDlgItemText(hDlg, item_id, text);
		};
	}

	namespace config {
		constexpr unsigned int filename_max = 260;
		constexpr TCHAR formatFilter[50] = TEXT("*.exe;*.dll;*.scr;*.drv;*.sys");
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

	void DecodeTimeStamp(const time_t timeStamp, std::string& timeYMD, std::string& timeHMS) {
		std::chrono::time_point tp = std::chrono::system_clock::from_time_t(timeStamp);

		time_t tt = std::chrono::system_clock::to_time_t(tp);
		tm time;
		errno_t err = localtime_s(&time, &tt);

		if (err != 0) {
			MessageBox(0, TEXT("Decode Time Stamp Fail"), TEXT("ERROR"), MB_OK);
			return;
		}

		timeYMD = std::format("{:04}-{:02}-{:02}", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
		timeHMS = std::format("{:02}:{:02}:{:02}", time.tm_hour, time.tm_min, time.tm_sec);
	}
}

