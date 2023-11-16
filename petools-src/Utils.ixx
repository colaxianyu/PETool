module;
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <string>
export module Utils;

//DWORD RVAToFOA(const DWORD RVA);
//DWORD FOAToRVA(const DWORD FOA);
//DWORD InWhichSectionRVA(const DWORD RVA);
//DWORD InWhichSectionFOA(const DWORD FOA);
export{
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
		WideCharToMultiByte(CP_ACP, 0, in, -1, temp, lenth, NULL, FALSE);
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
	
	void DecodeTimeStamp(const time_t timeStamp, tm& timeFormat) {
		tm time;
		errno_t err = localtime_s(&time, &timeStamp);
		if (err == 0) {
			timeFormat = time;
		}
		else {
			MessageBox(0, TEXT("Decode Time Stamp Fail"), TEXT("ERROR"), MB_OK);
		}
	}
}
//void GetTcharSectionName(const DWORD secIndex, TCHAR** name);
//void GetCharSectionName(const DWORD secIndex, char** name);

//DWORD GetImageSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
//DWORD GetFileSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
//void SetSectionCharacter(IMAGE_SECTION_HEADER& secHeader, DWORD cha);