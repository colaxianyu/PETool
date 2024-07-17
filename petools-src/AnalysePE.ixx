﻿module;

#include <windows.h>
#include <cstdio>
#include <fstream>
#include <memory>

export module AnalysePE;

export enum class PositionInPE : unsigned int {
	inHead = 0, inSection, outFile
};

using std::unique_ptr;
using enum PositionInPE;

//
//   PE headers
//

export struct PeHeaders {
	IMAGE_DOS_HEADER* dosHeader = nullptr;
	IMAGE_NT_HEADERS* ntHeader = nullptr;
	IMAGE_FILE_HEADER* fileHeader = nullptr;
	IMAGE_OPTIONAL_HEADER* optionalHeader = nullptr;
	IMAGE_SECTION_HEADER* sectionHeader = nullptr;

	void Reset() {
		dosHeader = nullptr;
		ntHeader = nullptr;
		fileHeader = nullptr;
		optionalHeader = nullptr;
		sectionHeader = nullptr;
	}
};

//
//	PE Analyse Tool 
//
export class AnalysePE
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

	void Init(std::fstream& file, DWORD fileSize);
	bool ReadFileToFileBuffer(std::fstream& file, DWORD fileSize);
	void UnloadPeData();							

	bool IsPEFile();
	bool SetHeaders();
	
	bool HaveTable(DWORD index);
	const DWORD GetAllTableSize();
	
	// Export Table
	IMAGE_EXPORT_DIRECTORY* GetExport();
	DWORD GetExportSize();
	//char* GetExportFAT();
	//char* GetExportFNT();
	//char* GetExportFOT();
	WORD GetOrdinalTableIndex(DWORD value);
	char* GetExportFuncName(DWORD index);
	DWORD GetFATSize();
	DWORD GetFNTSize();
	DWORD GetFOTSize();
	DWORD GetExportFuncNameSize();
	bool IsHaveExport();

	// Resource Table
	IMAGE_RESOURCE_DIRECTORY* GetResource();
	void AnalyseResource(IMAGE_RESOURCE_DIRECTORY*);
	
	// Import Table
	IMAGE_IMPORT_DESCRIPTOR* GetImport();
	DWORD GetAllImportSize();
	DWORD GetINTSize(DWORD importIndex);
	DWORD GetIATSize(DWORD importIndex);
	DWORD GetByNameTableSize(DWORD importIndex);
	bool IsHaveImport();
	
	// Relocation Table
	bool IsHaveRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation(DWORD index);
	DWORD GetAllRelocationSize();
	
	// Analyze Tool
	DWORD RVAToFOA(const DWORD RVA);
	DWORD FOAToRVA(const DWORD FOA);
	PositionInPE PositonInfoRVA(const DWORD RVA);
	PositionInPE PositonInfoFOA(const DWORD FOA);
	DWORD InWhichSectionRVA(const DWORD RVA);
	DWORD InWhichSectionFOA(const DWORD FOA);
	void GetTcharSectionName(const DWORD secIndex, TCHAR** name);
	void GetCharSectionName(const DWORD secIndex, char** name);
	DWORD GetImageSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
	DWORD GetFileSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader);
	void SetSectionCharacter(IMAGE_SECTION_HEADER& secHeader, DWORD cha);

	// 
	// EnlargeLastSection VirtualSize <= SizeOfRawData�ع���
	// AddSection VirtualSize > SizeOfRawData�ع���
	//
	void EnlargeLastSection(DWORD enlargeSize);
	void MoveImport();
	void AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]);
	bool AddSectionHeaderIfAvailable();
	void AdjustHeadrs();
	void AddSection(DWORD SectionSize, bool needAlignment);
	void AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader);
	void MoveToNewFileBuffer(const DWORD bufferSize);
	void DllInject(const TCHAR tDllName[], const TCHAR tFuncName[]);
	
	void EncodeProcess(char* buffer, DWORD size);
	
	// Move Table
	void MoveAllTable();
	void MoveExport(DWORD& moveAddress);
	void MoveExportFAT(DWORD& moveAddress);					// 
	void MoveExportFNT(DWORD& moveAddress);					// 
	void MoveExportFOT(DWORD& moveAddress);					// 
	void MoveExportFuncName(DWORD& moveAddress);			// 
	
	void MoveImport(DWORD& moveAddress);					// 
	void MoveINT(DWORD& moveAddress);						// 
	void MoveIAT(DWORD& moveAddress);						// 
	void MoveByNameTable(DWORD& moveAddress);				// 
	
	void MoveRelocation(DWORD& moveAddress);
	

	bool HaveInfo();
	DWORD GetInfoBuffer(char** buffer);						

	int GetFileBufferSzie() { return fileBufferSize_; }
	PeHeaders& GetHeaders() { return headers_; };
	char* GetFileBuffer() { return fileBuffer_.get(); }
private:
	unique_ptr<char[]> fileBuffer_ = nullptr;
	DWORD fileBufferSize_ = 0;
	PeHeaders headers_{ };
};
