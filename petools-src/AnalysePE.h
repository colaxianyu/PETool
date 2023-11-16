#pragma once
#include <windows.h>
#include <memory>
#include <functional>
#include "FileManage.h"

enum class PositionInPE : unsigned int{
	inHead = 0,  inSection, outFile
};

using std::unique_ptr;
using enum PositionInPE;

//
//   PE结构头指针
//

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

//
//	PE Tool功能主体
//
class FileManage;
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

	void Init(FileManage* const fileManage);
	void UnloadPeData();     
	void Update();			// TODO								

	bool IsPEFile();
	bool ReadFileToFileBuffer(FileManage* const fileManage);
	bool SetHeaders();

	bool HaveTable(DWORD index);
	const DWORD GetAllTableSize();

	// 读出导出表
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

	// 读出资源表
	IMAGE_RESOURCE_DIRECTORY* GetResource();
	void AnalyseResource(IMAGE_RESOURCE_DIRECTORY*);

	// 读出输入表
	IMAGE_IMPORT_DESCRIPTOR* GetImport();
	DWORD GetAllImportSize();
	DWORD GetINTSize(DWORD importIndex);
	DWORD GetIATSize(DWORD importIndex);
	DWORD GetByNameTableSize(DWORD importIndex);

	// 读出重定位表
	bool IsHaveRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation(DWORD index);
	DWORD GetAllRelocationSize();

	// 常用的工具
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
	
	// 注入相关工具
	// EnlargeLastSection是VirtualSize <= SizeOfRawData特供版
	// AddSection是VirtualSize > SizeOfRawData特供版
	// 故这两个函数的实现不考虑多于情况
	void EnlargeLastSection(DWORD enlargeSize);
	void MoveImport();
	void AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]);
	bool AddSectionHeaderIfAvailable();
	void AdjustHeadrs();
	void AddSection(DWORD SectionSize, bool needAlignment);
	void AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader);
	void MoveToNewFileBuffer(const DWORD bufferSize);
	void DllInject(const TCHAR tDllName[], const TCHAR tFuncName[]);

	// 加壳相关
	void EncodeProcess(char* buffer, DWORD size);

	// 将所有的表移动到新增的节中
	void MoveAllTable();

	void MoveExport(DWORD& moveAddress);
	void MoveExportFAT(DWORD& moveAddress);					// 移动导出函数地址表
	void MoveExportFNT(DWORD& moveAddress);					// 移动导出函数名称表
	void MoveExportFOT(DWORD& moveAddress);					// 移动导出函数序号表
	void MoveExportFuncName(DWORD& moveAddress);			// 移动导出函数的名称

	void MoveImport(DWORD& moveAddress);					// 移动所有导入表
	void MoveINT(DWORD& moveAddress);						// 移动所有导入名称表
	void MoveIAT(DWORD& moveAddress);						// TODO：移动所有导入地址表
	void MoveByNameTable(DWORD& moveAddress);				// 移动所有的导入函数名称表

	void MoveRelocation(DWORD& moveAddress);

	// Test：用于移动文件末尾的签名信息等
	bool HaveInfo();
	DWORD GetInfoBuffer(char** buffer);						// 返回buffer的size

	int GetFileBufferSzie() { return fileBufferSize_; }
	PeHeaders& GetHeaders() { return headers_; };
private:
	unique_ptr<char> fileBuffer_ = nullptr;
	DWORD fileBufferSize_ = 0;
	PeHeaders headers_{ };
};
