#pragma once
#include <windows.h>
#include <memory>
#include <functional>

enum class PositionInPE : unsigned int{
	inHead = 0,  inSection, outFile
};

using std::unique_ptr;
using enum PositionInPE;

/*
*   PE�ṹͷָ��
*/

struct PeHeaders {
	IMAGE_DOS_HEADER* dosHeader;
	IMAGE_NT_HEADERS* ntHeader;
	IMAGE_FILE_HEADER* fileHeader;
	IMAGE_OPTIONAL_HEADER* optionalHeader;
	IMAGE_SECTION_HEADER* sectionHeader;

	PeHeaders() {
		dosHeader = nullptr;
		ntHeader = nullptr;
		fileHeader = nullptr;
		optionalHeader = nullptr;
		sectionHeader = nullptr;
	}

	void Reset() {
		dosHeader = nullptr;
		ntHeader = nullptr;
		fileHeader = nullptr;
		optionalHeader = nullptr;
		sectionHeader = nullptr;
	}
};

/*
*	PE Tool��������
*/

class AnalysePE
{
private:
	AnalysePE() = default;
	~AnalysePE() = default;
public:
	AnalysePE(const AnalysePE&) = delete;
	AnalysePE(AnalysePE&&) = delete;
	AnalysePE& operator=(const AnalysePE&) = delete;
	AnalysePE& operator=(AnalysePE&&) = delete;

	static AnalysePE& GetAnalyse() {
		static AnalysePE analyse;
		return analyse;
	}

	void Init(const TCHAR* tFilePath);
	void UnloadPeData();     
	void Update();										// ���ڸ���header��imageBufferSize��fileBufferSize

	bool IsPEFile();
	bool ReadFileToFileBuffer();
	bool SetHeaders();

	// ��������
	bool HaveExport();
	IMAGE_EXPORT_DIRECTORY* GetExport();
	WORD GetOrdinalTableIndex(DWORD value);
	char* GetExportFuncName(DWORD index);

	// �������
	IMAGE_IMPORT_DESCRIPTOR* GetImport();
	DWORD GetAllImportSize();

	// �ض�λ����
	bool IsHaveRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation(DWORD index);

	// ���õĹ���
	DWORD RVAToFOA(const DWORD RVA);
	DWORD FOAToRVA(const DWORD FOA);
	PositionInPE PositonInfoRVA(const DWORD RVA);
	PositionInPE PositonInfoFOA(const DWORD FOA);
	DWORD InWhichSectionRVA(const DWORD RVA);
	DWORD InWhichSectionFOA(const DWORD FOA);
	void CharToTchar(const char* in, TCHAR** out);
	void TcharToChar(const TCHAR* in, char** out);
	void TcharToDword(const TCHAR* in, DWORD* out, int base);
	void GetTcharSectionName(const DWORD secIndex, TCHAR** name);
	void GetCharSectionName(const DWORD secIndex, char** name);
	void DecodeTimeStamp(const time_t timeStamp, tm& timeFormat);
	DWORD GetImageSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
	DWORD GetFileSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
	void SetSectionCharacter(IMAGE_SECTION_HEADER& secHeader, DWORD cha);
	
	// ע����ع���
	// EnlargeLastSection��VirtualSize <= SizeOfRawData�ع���
	// AddSection��VirtualSize > SizeOfRawData�ع���
	// ��������������ʵ�ֲ����Ƕ������
	void EnlargeLastSection(DWORD enlargeSize);
	void MoveImport();
	void AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]);
	bool AddSectionHeaderIfAvailable();
	void AdjustHeadrs();
	void AddSection(DWORD SectionSize);
	void AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader);
	void MoveToNewFileBuffer(const DWORD bufferSize);
	void DllInject(const TCHAR tDllName[], const TCHAR tFuncName[]);

	void GetFilePath(TCHAR* tPath) { TCHAR* p = nullptr; CharToTchar(filePath_.get(), &p); memcpy(tPath, p, MAX_PATH); }
	int GetFileSzie() { return fileBufferSize_; }
	PeHeaders& GetHeaders() { return headers_; };
	
private:
	unique_ptr<char> filePath_ = nullptr;
	unique_ptr<char> fileBuffer_ = nullptr;
	DWORD fileBufferSize_ = 0;
	PeHeaders headers_{ };
};
