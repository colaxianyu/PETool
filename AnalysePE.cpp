#include "AnalysePE.h"
#include <time.h>
#include <iostream>
#include <cassert>

void AnalysePE::Init(const TCHAR* tFilePath) {
	char* cPath = nullptr;
	TcharToChar(tFilePath, &cPath);
	filePath_ = unique_ptr<char>(cPath); 
	cPath = nullptr;

	bool isReadToBuffer = ReadFileToFileBuffer();
	if (!isReadToBuffer) {
		return;
	}
	SetHeaders();
}

void AnalysePE::UnloadPeData() {
	fileBuffer_.reset();
	filePath_.reset();
	fileBufferSize_ = 0;
	headers_.Reset();
}

void AnalysePE::Update() {

}

bool AnalysePE::ReadFileToFileBuffer() {
	FILE* file = nullptr;
	fopen_s(&file,filePath_.get(), "rb");

	//assert(file != nullptr && "文件打开失败!");
	if (file == nullptr) {
		MessageBox(0, TEXT("文件打开失败!"), TEXT("ERROR"), MB_OK);
		return false;
	}

	// 计算文件大小，并分配Buffer
	DWORD fileSize = 0;
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	fileBuffer_ = unique_ptr<char>(new char[fileSize]);

	assert(fileBuffer_ != nullptr && "FileBuffer申请失败!");

	fileBufferSize_ = fileSize;
	// 将文件读入buffer中
	DWORD readSize = fread(fileBuffer_.get(), fileSize, 1, file);

	assert(readSize == 0 && "文件读入Buffer失败!");

	fclose(file);
	file = nullptr;
	return true;
}

bool AnalysePE::IsPEFile() {
	if (*(WORD*)fileBuffer_.get() != IMAGE_DOS_SIGNATURE) {
		return false;
	}
	IMAGE_DOS_HEADER* tempDosHeader = (IMAGE_DOS_HEADER*)fileBuffer_.get();
	if (*(DWORD*)((DWORD)tempDosHeader + tempDosHeader->e_lfanew) != IMAGE_NT_SIGNATURE) {
		return false;
	}
	return true;
}

bool AnalysePE::SetHeaders() {
	if (!IsPEFile()) {
		MessageBox(0, TEXT("不是有效的PE文件!"), TEXT("ERROR"), MB_OK);
		return false;
	}

	headers_.dosHeader = (IMAGE_DOS_HEADER*)fileBuffer_.get();
	headers_.ntHeader = (IMAGE_NT_HEADERS*)((DWORD)headers_.dosHeader + headers_.dosHeader->e_lfanew);
	headers_.fileHeader = (IMAGE_FILE_HEADER*)((DWORD)headers_.ntHeader + 4);
	headers_.optionalHeader = (IMAGE_OPTIONAL_HEADER*)((DWORD)headers_.fileHeader + IMAGE_SIZEOF_FILE_HEADER);
	headers_.sectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)headers_.optionalHeader + headers_.fileHeader->SizeOfOptionalHeader);

	return true;
}

bool AnalysePE::HaveExport() {
	if (headers_.optionalHeader->DataDirectory[0].Size != 0
		&& headers_.optionalHeader->DataDirectory[0].VirtualAddress != 0) {
		return true;
	}
	else {
		return false;
	}
}

IMAGE_EXPORT_DIRECTORY* AnalysePE::GetExport() {
	return (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
}

WORD AnalysePE::GetOrdinalTableIndex(DWORD value) {
	IMAGE_EXPORT_DIRECTORY* exportPtr = (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
	WORD* ordinalTable = (WORD*)((DWORD)headers_.dosHeader
		+ RVAToFOA((DWORD)exportPtr->AddressOfNameOrdinals));

	for (int i = 0; i < exportPtr->NumberOfNames; i++) {
		if (*(ordinalTable + i) == value) {
			return i;
		}
	}
	return -1;
}

char* AnalysePE::GetExportFuncName(DWORD index) {
	IMAGE_EXPORT_DIRECTORY* exportPtr = (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
	DWORD* nameOffset = (DWORD*)((DWORD)headers_.dosHeader
		+ RVAToFOA((DWORD)exportPtr->AddressOfNames));

	char* name = (char*)((DWORD)headers_.dosHeader + RVAToFOA(*(nameOffset + index)));
	return name;
}

IMAGE_IMPORT_DESCRIPTOR* AnalysePE::GetImport() {
	return (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
}

DWORD AnalysePE::GetAllImportSize() {
	DWORD importNum = 0;
	IMAGE_IMPORT_DESCRIPTOR* tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader +
		RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
	for (int i = 0; tempImport->OriginalFirstThunk != 0 && tempImport->FirstThunk != 0; i++, tempImport++) {
		importNum++;
	}
	DWORD allImportSize = importNum * sizeof(IMAGE_IMPORT_DESCRIPTOR);
	return allImportSize;
}

bool AnalysePE::IsHaveRelocation() {
	if (headers_.optionalHeader->DataDirectory[5].Size != 0
		&& headers_.optionalHeader->DataDirectory[5].VirtualAddress != 0) {
		return true;
	}
	else {
		return false;
	}
}

IMAGE_BASE_RELOCATION* AnalysePE::GetRelocation() {
	return (IMAGE_BASE_RELOCATION*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[5].VirtualAddress));
}

IMAGE_BASE_RELOCATION* AnalysePE::GetRelocation(DWORD index) {
	IMAGE_BASE_RELOCATION* tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[5].VirtualAddress));
	for (int i = 0; i < index; i++) {
		tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)tempRelocation
			+ tempRelocation->SizeOfBlock);
	}
	return tempRelocation;
}

void AnalysePE::CharToTchar(const char* in, TCHAR** out) {
	DWORD lenth = MultiByteToWideChar(CP_ACP, 0, in, strlen(in) + 1, nullptr, 0);
	TCHAR* temp = new TCHAR[sizeof(TCHAR) * lenth];
	MultiByteToWideChar(CP_ACP, 0, in, strlen(in) + 1, temp, lenth);
	*out = temp;
	temp = nullptr;
}

void AnalysePE::TcharToChar(const TCHAR* in, char** out) {
	DWORD lenth = WideCharToMultiByte(CP_ACP, 0, in, -1, nullptr, 0, nullptr, FALSE);
	char* temp = new char[sizeof(char) * lenth];
	WideCharToMultiByte(CP_ACP, 0, in, -1, temp, lenth, NULL, FALSE);
	*out = temp;
	temp = nullptr;
}

void AnalysePE::TcharToDword(const TCHAR* in, DWORD* out, int base) {
	TCHAR* end;
	*out = wcstol(in, &end, base);
}

DWORD AnalysePE::RVAToFOA(const DWORD RVA){
	DWORD FOA = 0;
	PositionInPE pos = PositonInfoRVA(RVA);
	switch (pos) {
	case inHead:
	{
		FOA = RVA;
		return FOA;
	}
	case inSection:
	{
		DWORD sectionIndex = InWhichSectionRVA(RVA);
		IMAGE_SECTION_HEADER a = headers_.sectionHeader[sectionIndex];
		FOA = RVA - headers_.sectionHeader[sectionIndex].VirtualAddress
			+ headers_.sectionHeader[sectionIndex].PointerToRawData;;
		return FOA;
	}
	default:
		return FOA;
	}
}

DWORD AnalysePE::FOAToRVA(const DWORD FOA) {
	DWORD RVA = 0;
	PositionInPE pos = PositonInfoFOA(FOA);

	switch (pos) {
	case inHead:
	{
		RVA = FOA;
		return RVA;
	}
	case inSection:
	{
		DWORD sectionIndex = InWhichSectionFOA(FOA);
		RVA = FOA - headers_.sectionHeader[sectionIndex].PointerToRawData
			+ headers_.sectionHeader[sectionIndex].VirtualAddress;
		return RVA;
	}
	default:
		return RVA;
	}
}

PositionInPE AnalysePE::PositonInfoRVA(const DWORD RVA) {
	PositionInPE Pos;
	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;

	if (RVA < headers_.sectionHeader->VirtualAddress) {
		Pos = inHead;
	}
	else if (RVA >= headers_.optionalHeader->SizeOfImage) {
		Pos = outFile;
	}
	else {
		Pos = inSection;
	}
	return Pos;
}

PositionInPE AnalysePE::PositonInfoFOA(const DWORD FOA) {
	PositionInPE Pos;

	if (FOA < headers_.optionalHeader->SizeOfHeaders) {
		Pos = inHead;
	}
	else if (DWORD lastSectionIndex = headers_.fileHeader->NumberOfSections - 1; 
		FOA >= headers_.sectionHeader[lastSectionIndex].PointerToRawData
		+ headers_.sectionHeader[lastSectionIndex].SizeOfRawData) {
		Pos = outFile;
	}
	else {
		Pos = inSection;
	}
	return Pos;
}

DWORD AnalysePE::InWhichSectionRVA(const DWORD RVA) {
	DWORD sectionIndex = headers_.fileHeader->NumberOfSections - 1;
	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
	DWORD sectionEdge = headers_.optionalHeader->SizeOfImage;

	while (sectionIndex >= 0) {
		if (RVA >= headers_.sectionHeader[sectionIndex].VirtualAddress
			&& RVA <= sectionEdge) {
			return sectionIndex;
		}
		sectionEdge = headers_.sectionHeader[sectionIndex].VirtualAddress;
		sectionIndex--;		
	}
}

DWORD AnalysePE::InWhichSectionFOA(const DWORD FOA) {
	DWORD sectionIndex = headers_.fileHeader->NumberOfSections - 1;

	while (sectionIndex >= 0) {
		if (FOA >= headers_.sectionHeader[sectionIndex].PointerToRawData
			&& FOA < headers_.sectionHeader[sectionIndex].PointerToRawData
			+ headers_.sectionHeader[sectionIndex].SizeOfRawData) {

			return sectionIndex;
		}
		sectionIndex--;
	}
}

void AnalysePE::GetTcharSectionName(const DWORD secIndex, TCHAR** name) {
	char* cTemp = new char[sizeof(char) * 8];
	for (int i = 0; i < 8; i++) {
		cTemp[i] = headers_.sectionHeader[secIndex].Name[i];
	}
	TCHAR* tTemp = nullptr;
	CharToTchar(cTemp, &tTemp);
	*name = tTemp;
}

void AnalysePE::GetCharSectionName(const DWORD secIndex, char** name) {
	char* cTemp = new char[sizeof(char) * 8];
	for (int i = 0; i < 8; i++) {
		cTemp[i] = headers_.sectionHeader[secIndex].Name[i];
	}
	*name = cTemp;
}

void AnalysePE::DecodeTimeStamp(const time_t timeStamp, tm& timeFormat) {
	tm time;
	errno_t err = localtime_s(&time, &timeStamp);
	if (err == 0) {
		timeFormat = time;
	}
	else {
		MessageBox(0, TEXT("Decode Time Stamp Fail"), TEXT("ERROR"), MB_OK);
	}
}

// 内存映像中section的实际大小，取二者中最大的，然后进行内存对齐
DWORD AnalysePE::GetImageSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader) {
	DWORD imageSectionSize = headers_.optionalHeader->SectionAlignment
		* ceil(static_cast<float>(max(sectionHeader.Misc.VirtualSize, sectionHeader.SizeOfRawData)) / static_cast<float>(headers_.optionalHeader->SectionAlignment));
	return imageSectionSize;
}

// 磁盘文件中section的实际大小，取二者中最小的，然后进行磁盘对齐
DWORD AnalysePE::GetFileSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader) {
	DWORD fileSectionSize = headers_.optionalHeader->FileAlignment
		* ceil(static_cast<float>(min(sectionHeader.Misc.VirtualSize, sectionHeader.SizeOfRawData)) / static_cast<float>(headers_.optionalHeader->FileAlignment));
	return fileSectionSize;
}

void AnalysePE::SetSectionCharacter(IMAGE_SECTION_HEADER& secHeader, DWORD cha) {
	secHeader.Characteristics = cha;
}

void AnalysePE::EnlargeLastSection(DWORD enlargeSize) {
	if (enlargeSize == 0) {
		return;
	}
	DWORD oldSectionSize = (headers_.sectionHeader
		+ headers_.fileHeader->NumberOfSections - 1)->SizeOfRawData;
	char* tempBuffer = nullptr;
	DWORD alignmentSize = ceil(static_cast<float>(enlargeSize) / static_cast<float>(headers_.optionalHeader->FileAlignment))
		* headers_.optionalHeader->FileAlignment;
	tempBuffer = new char[fileBufferSize_ + alignmentSize];

	if (tempBuffer == nullptr) {
		MessageBox(0, TEXT("Enlarge失败!"), TEXT("ERROR"), MB_OK);
		return;
	}
	memset(tempBuffer, 0, fileBufferSize_ + alignmentSize);
	memcpy(tempBuffer, fileBuffer_.get(), fileBufferSize_);
	fileBufferSize_ += alignmentSize;
	fileBuffer_.reset();
	fileBuffer_ = unique_ptr<char>(tempBuffer);

	SetHeaders();
	IMAGE_SECTION_HEADER* newLastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
	(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1)->SizeOfRawData = alignmentSize + oldSectionSize;

	headers_.optionalHeader->SizeOfImage += GetImageSectionSizeAlignment(*(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1));
}

void AnalysePE::MoveImport() {
	DWORD allImportSize = GetAllImportSize();
	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
	//DWORD oldLastSectionSize = lastSectionHeader->SizeOfRawData;
	// 考虑两种情况，1.VirtualSize <= SizeOfRawData，那么VirtualSize反映了数据真实的大小，空闲空间的计算正确
	// 2.VirtualSize > SizeOfRawData(此时freeSpace为负)，此时区块内有大量未初始化的数据，无法写入
	/*DWORD freeSpace = fileBufferSize_ - (lastSectionHeader->PointerToRawData + min(lastSectionHeader->Misc.VirtualSize, lastSectionHeader->SizeOfRawData));
	if(freeSpace <= allImportSize) {
		EnlargeLastSection(allImportSize - freeSpace);
	}*/
	DWORD newImportAddr = (DWORD)headers_.dosHeader + (headers_.sectionHeader
		+ headers_.fileHeader->NumberOfSections - 1)->PointerToRawData
		+ lastSectionHeader->Misc.VirtualSize;
	
	DWORD oldImportAddr = (DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress);
	memcpy((char*)newImportAddr, (char*)oldImportAddr, allImportSize);


	headers_.optionalHeader->DataDirectory[1].VirtualAddress = FOAToRVA(newImportAddr - (DWORD)headers_.dosHeader);
}

void AnalysePE::AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]) {
	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;

	//MoveImport();
	DWORD allImportSize = GetAllImportSize();
	DWORD freeSpace = fileBufferSize_ - (headers_.sectionHeader
		+ headers_.fileHeader->NumberOfSections - 1)->PointerToRawData;
	if (freeSpace < 62) {
		EnlargeLastSection(400);
	}
	IMAGE_IMPORT_DESCRIPTOR* tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader
		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
	while (tempImport->OriginalFirstThunk != 0 && tempImport->FirstThunk != 0) {
		tempImport++;
	}

	//创建新Import表
	IMAGE_IMPORT_DESCRIPTOR* newImport = new IMAGE_IMPORT_DESCRIPTOR();
	memset(newImport, 0, 20);
	memcpy((char*)tempImport, (char*)newImport, 20);
	delete newImport;
	newImport = tempImport;
	memset((char*)(tempImport+1), 0, 20);
	tempImport += 3;

	//创建新的INT表
	//tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_IMPORT_BY_NAME*)tempImport + 2);
	IMAGE_THUNK_DATA* newINT = new IMAGE_THUNK_DATA();
	memset(newINT, 0, sizeof(IMAGE_THUNK_DATA));
	memcpy((char*)tempImport, newINT, sizeof(IMAGE_THUNK_DATA));
	delete newINT;
	newINT = (IMAGE_THUNK_DATA*)tempImport;
	memset((char*)(newINT + 1), 0, sizeof(IMAGE_THUNK_DATA));
	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_THUNK_DATA*)tempImport + 3);

	//创建新的IAT表
	IMAGE_THUNK_DATA* newIAT = new IMAGE_THUNK_DATA();
	memset(newIAT, 0, sizeof(IMAGE_THUNK_DATA));
	memcpy((char*)tempImport, newIAT, sizeof(IMAGE_THUNK_DATA));
	delete newIAT;
	newIAT = (IMAGE_THUNK_DATA*)tempImport;
	memset((char*)(newIAT + 1), 0, sizeof(IMAGE_THUNK_DATA));
	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_THUNK_DATA*)tempImport + 3);

	// 为新建的Import表赋值
	char* cDllname = new char();
	TcharToChar(tDllName, &cDllname);
	std::string s;
	s += cDllname;
	delete cDllname;
	cDllname = (char*)tempImport;
	memcpy((char*)cDllname, s.c_str(), s.size());

	newImport->OriginalFirstThunk = FOAToRVA((DWORD)newINT - (DWORD)headers_.dosHeader);
	newImport->FirstThunk = FOAToRVA((DWORD)newIAT - (DWORD)headers_.dosHeader);
	newImport->Name = FOAToRVA((DWORD)cDllname - (DWORD)headers_.dosHeader);

	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((char*)tempImport + s.size() + 1);

	//创建新的ByName表
	IMAGE_IMPORT_BY_NAME* newByname = new IMAGE_IMPORT_BY_NAME;
	memset((char*)newByname, 0, sizeof(IMAGE_IMPORT_BY_NAME));
	memcpy((char*)tempImport, newByname, sizeof(IMAGE_IMPORT_BY_NAME));
	delete newByname;
	newByname = (IMAGE_IMPORT_BY_NAME*)tempImport;
	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_IMPORT_BY_NAME*)tempImport + 2);



	// 为新建的INT与IAT表赋值
	newINT->u1.AddressOfData = FOAToRVA((DWORD)newByname - (DWORD)headers_.dosHeader);
	newIAT->u1.AddressOfData = FOAToRVA((DWORD)newByname - (DWORD)headers_.dosHeader);


	// 为新建的ByName表赋值

	newByname->Hint = 0;
	char* funcName = nullptr;
	TcharToChar(tFuncName, &funcName);
	char* name = newByname->Name;
	int i = 0;
	for (i; *(funcName + i) != '\0'; i++) {
		*(name + i) = *(funcName + i);
	}
	/**(name + i) = '\0';*/
}

// 判断最后一个区块表与以一个区块之间的的空间是否可以添加新区块表
// 因为PointerToRawData在特定情况下可能为0，所以实际的空闲空间并不能靠该值计算
bool AnalysePE::AddSectionHeaderIfAvailable() {
	DWORD freeSpaceSize = headers_.optionalHeader->SizeOfHeaders - ((DWORD)headers_.sectionHeader - (DWORD)headers_.dosHeader)
		- headers_.fileHeader->NumberOfSections * IMAGE_SIZEOF_SECTION_HEADER;

	// 40字节给新的sectionHeader结构、40字节全0做结束标志
	if (freeSpaceSize < IMAGE_SIZEOF_SECTION_HEADER * 2) {
		return false;
	}
	return true;
}

void AnalysePE::AnalysePE::AdjustHeadrs() {
	headers_.dosHeader->e_lfanew = sizeof(*headers_.dosHeader);

	DWORD moveHeadersSize = sizeof(headers_.ntHeader->Signature) + IMAGE_SIZEOF_FILE_HEADER
		+ headers_.fileHeader->SizeOfOptionalHeader
		+ IMAGE_SIZEOF_SECTION_HEADER * headers_.fileHeader->NumberOfSections;
	memcpy((void*)((DWORD)headers_.dosHeader + headers_.dosHeader->e_lfanew), headers_.ntHeader, moveHeadersSize);

	SetHeaders();
	DWORD freeSpaceSize = headers_.optionalHeader->SizeOfHeaders - ((DWORD)headers_.sectionHeader - (DWORD)headers_.dosHeader)
		- headers_.fileHeader->NumberOfSections * sizeof(*headers_.sectionHeader);

	memset(headers_.sectionHeader + headers_.fileHeader->NumberOfSections, 0, freeSpaceSize);
}

void AnalysePE::AddSection(DWORD SectionSize) {
	IMAGE_SECTION_HEADER mySectionHeader{0};
	
	// 初始化自定义的区块表，其中PointerToRawData需要实际添加后才可确定值，故由AddSectionHeader()函数完成
	std::string name = ".mySec";
	for (int i = 0; name[i] != 0 && i < IMAGE_SIZEOF_SHORT_NAME; i++) {
		mySectionHeader.Name[i] = name[i];
	}
	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
	mySectionHeader.VirtualAddress = lastSectionHeader->VirtualAddress + GetImageSectionSizeAlignment(*lastSectionHeader);
	mySectionHeader.Misc.VirtualSize = 0;															
	DWORD fileAlignmentSectionSize = headers_.optionalHeader->FileAlignment
		* ceil(static_cast<float>(SectionSize) / static_cast<float>(headers_.optionalHeader->FileAlignment));
	mySectionHeader.SizeOfRawData = fileAlignmentSectionSize;
	mySectionHeader.Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;

	// 确定自定义Section在文件中的位置，即PointerToRawData，为了防止最后一个Section是一个完全未初始化的Section(即SizeOfRawData和PointerToRawData都为0)
	// 故需要从LastSection向前遍历，找到第一个PointerToRawData不为0的Section，将自定义Section复制到该节之后
	for (int i = headers_.fileHeader->NumberOfSections - 1; i >=0; i--) {
		if ((headers_.sectionHeader + i)->PointerToRawData != 0 && (headers_.sectionHeader + i)->SizeOfRawData != 0) {
			mySectionHeader.PointerToRawData = (headers_.sectionHeader + i)->PointerToRawData + GetFileSectionSizeAlignment(*(headers_.sectionHeader + i));
			break;
		}
	}
	char* newFileBuffer = new char[fileBufferSize_ + mySectionHeader.SizeOfRawData];
	memset(newFileBuffer, 0, fileBufferSize_ + mySectionHeader.SizeOfRawData);
	memcpy(newFileBuffer, fileBuffer_.get(), fileBufferSize_);
	fileBuffer_.reset();
	fileBuffer_ = unique_ptr<char>(newFileBuffer);
	fileBufferSize_ += mySectionHeader.SizeOfRawData;
	SetHeaders();

	AddSectionHeader(&mySectionHeader);
	headers_.fileHeader->NumberOfSections++;
	headers_.optionalHeader->SizeOfImage += GetImageSectionSizeAlignment(mySectionHeader);
}

// 为新的区块添加区块表，判断Header中的空闲空间是否可以加入区块表，如果不可以，则讲dosHeader之下的所有header抬高（占用dos stub的空间）
// 若仍然不够，则开辟新空间
void AnalysePE::AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader) {
	bool isAvailableSpace = AddSectionHeaderIfAvailable();	
	DWORD newBufferSize = 0;

	if (!isAvailableSpace) {
		AdjustHeadrs();

		bool isAvailableAdjust = AddSectionHeaderIfAvailable();
		if (!isAvailableAdjust) {
			DWORD headerSizeNoAlignment = (DWORD)(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1)
				+ IMAGE_SIZEOF_SECTION_HEADER - (DWORD)headers_.dosHeader;

			// newBufferSize = 原BufferSize - 原header大小 + 新的对其后的header大小
			DWORD newHeaderSize = headers_.optionalHeader->FileAlignment 
				* ceil(static_cast<float>(headerSizeNoAlignment + IMAGE_SIZEOF_SECTION_HEADER) / static_cast<float>(headers_.optionalHeader->FileAlignment));
			newBufferSize = fileBufferSize_ - headers_.optionalHeader->SizeOfHeaders + newHeaderSize;
			char* newFileBuffer = new char[newBufferSize];
			memcpy(newFileBuffer, fileBuffer_.get(), newHeaderSize);
			memcpy(newFileBuffer + newHeaderSize, fileBuffer_.get() + headers_.optionalHeader->SizeOfHeaders
				, fileBufferSize_ - headers_.optionalHeader->SizeOfHeaders);
			fileBuffer_.reset();
			fileBuffer_ = unique_ptr<char>(newFileBuffer);
			SetHeaders();
			headers_.optionalHeader->SizeOfHeaders = newHeaderSize;
		}		
	}
	IMAGE_SECTION_HEADER* temp = (IMAGE_SECTION_HEADER*)headers_.sectionHeader + headers_.fileHeader->NumberOfSections;
	memcpy(temp, mySectionHeader, IMAGE_SIZEOF_SECTION_HEADER);
	
}

void AnalysePE::MoveToNewFileBuffer(const DWORD newBufferSize) {
	char* newFileBuffer = new char[newBufferSize];
	if (newFileBuffer == nullptr) {
		return;
	}
	memset(newFileBuffer, 0, newBufferSize);
	memcpy_s(newFileBuffer, newBufferSize, fileBuffer_.get(), fileBufferSize_);
	fileBufferSize_ = newBufferSize;
	fileBuffer_.reset();	
	fileBuffer_ = unique_ptr<char>((char*)newFileBuffer);
	SetHeaders();
}

void AnalysePE::DllInject(const TCHAR tDllName[], const TCHAR tFuncName[]) {
	char* cDllName = nullptr;
	TcharToChar(tDllName, &cDllName);
	char* cFuncName = nullptr;
	TcharToChar(tFuncName, &cFuncName); 

	// injectSpaceSize的最小值 = （导入表 + INT表 + IAT表）* 2 + byName表中HINT的大小(2) + byName表大小 + len(tDllName) + len(tFuncNAme) + allImportSize + 2
	// 为了方便计算，采用（导入表 + INT表 + IAT表 + byName表）* 2 + len(tDllName) + len(tFuncNAme) + allImportSize + 2(两个\0)，只比上式多了2字节
	DWORD injectSpaceSize = (sizeof(IMAGE_IMPORT_DESCRIPTOR) + sizeof(IMAGE_THUNK_DATA) * 2 + sizeof(IMAGE_IMPORT_BY_NAME)) * 2
		+ strlen(cDllName) + strlen(cFuncName) + 2 + GetAllImportSize();

	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;

	std::function<void()> SetFreeSpace = nullptr;
	DWORD needSize = 0;
	if (lastSectionHeader->Misc.VirtualSize > lastSectionHeader->SizeOfRawData) {
		needSize = injectSpaceSize;
		SetFreeSpace = [&]() {AddSection(needSize); };
	}
	else{
		DWORD freeSpaceSize = lastSectionHeader->SizeOfRawData - lastSectionHeader->Misc.VirtualSize;
		// 错误的计算方式，会覆盖文件的数字签名、调试信息等内容
		//DWORD freeSpaceSize = fileBufferSize_ - (lastSectionHeader->PointerToRawData + lastSectionHeader->Misc.VirtualSize);
		if (freeSpaceSize < injectSpaceSize) {
			needSize = injectSpaceSize - freeSpaceSize;
		}
		SetFreeSpace = [&]() {EnlargeLastSection(needSize); };
		SetSectionCharacter(*lastSectionHeader, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	}
	SetFreeSpace();
	MoveImport();
	AddImport(tDllName, tFuncName);
}
