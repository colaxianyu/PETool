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
//   PE�ṹͷָ��
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
//	PE Tool��������
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

	// ����������
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

	// ������Դ��
	IMAGE_RESOURCE_DIRECTORY* GetResource();
	void AnalyseResource(IMAGE_RESOURCE_DIRECTORY*);

	// ���������
	IMAGE_IMPORT_DESCRIPTOR* GetImport();
	DWORD GetAllImportSize();
	DWORD GetINTSize(DWORD importIndex);
	DWORD GetIATSize(DWORD importIndex);
	DWORD GetByNameTableSize(DWORD importIndex);

	// �����ض�λ��
	bool IsHaveRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation();
	IMAGE_BASE_RELOCATION* GetRelocation(DWORD index);
	DWORD GetAllRelocationSize();

	// ���õĹ���
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
	
	// ע����ع���
	// EnlargeLastSection��VirtualSize <= SizeOfRawData�ع���
	// AddSection��VirtualSize > SizeOfRawData�ع���
	// ��������������ʵ�ֲ����Ƕ������
	void EnlargeLastSection(DWORD enlargeSize);
	void MoveImport();
	void AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]);
	bool AddSectionHeaderIfAvailable();
	void AdjustHeadrs();
	void AddSection(DWORD SectionSize, bool needAlignment);
	void AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader);
	void MoveToNewFileBuffer(const DWORD bufferSize);
	void DllInject(const TCHAR tDllName[], const TCHAR tFuncName[]);

	// �ӿ����
	void EncodeProcess(char* buffer, DWORD size);

	// �����еı��ƶ��������Ľ���
	void MoveAllTable();

	void MoveExport(DWORD& moveAddress);
	void MoveExportFAT(DWORD& moveAddress);					// �ƶ�����������ַ��
	void MoveExportFNT(DWORD& moveAddress);					// �ƶ������������Ʊ�
	void MoveExportFOT(DWORD& moveAddress);					// �ƶ�����������ű�
	void MoveExportFuncName(DWORD& moveAddress);			// �ƶ���������������

	void MoveImport(DWORD& moveAddress);					// �ƶ����е����
	void MoveINT(DWORD& moveAddress);						// �ƶ����е������Ʊ�
	void MoveIAT(DWORD& moveAddress);						// TODO���ƶ����е����ַ��
	void MoveByNameTable(DWORD& moveAddress);				// �ƶ����еĵ��뺯�����Ʊ�

	void MoveRelocation(DWORD& moveAddress);

	// Test�������ƶ��ļ�ĩβ��ǩ����Ϣ��
	bool HaveInfo();
	DWORD GetInfoBuffer(char** buffer);						// ����buffer��size

	int GetFileBufferSzie() { return fileBufferSize_; }
	PeHeaders& GetHeaders() { return headers_; };
private:
	unique_ptr<char> fileBuffer_ = nullptr;
	DWORD fileBufferSize_ = 0;
	PeHeaders headers_{ };
};
