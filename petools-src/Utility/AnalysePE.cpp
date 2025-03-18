//module;
//
//#include <windows.h>
//#include <string>
//#include <memory>
//
//module AnalysePE;
//
//import STL;
//
//using std::string;
//using std::cout;
//
//using petools::TcharToChar;
//using petools::CharToTchar;
//
//void AnalysePE::init(std::fstream& file, DWORD fileSize){
//	bool isReadToBuffer = ReadFileToFileBuffer(file, fileSize);
//	if (!isReadToBuffer) {
//		return;
//	}
//	SetHeaders();
//}
//
//void AnalysePE::UnloadPeData() {
//	fileBuffer_.reset();
//	fileBufferSize_ = 0;
//	headers_.Reset();
//}
//
//bool AnalysePE::ReadFileToFileBuffer(std::fstream& file, DWORD fileSize) {
//	fileBufferSize_ = fileSize;
//	fileBuffer_ = std::make_unique<char[]>(fileSize);
//	file.read(fileBuffer_.get(), fileSize);
//
//	if (file.fail()) {
//		return false;
//	}
//	return true;
//}
//
//bool AnalysePE::IsPEFile() {
//	if (*(WORD*)fileBuffer_.get() != IMAGE_DOS_SIGNATURE) {
//		return false;
//	}
//	IMAGE_DOS_HEADER* tempDosHeader = (IMAGE_DOS_HEADER*)fileBuffer_.get();
//	if (*(DWORD*)((DWORD)tempDosHeader + tempDosHeader->e_lfanew) != IMAGE_NT_SIGNATURE) {
//		return false;
//	}
//	return true;
//}
//
//bool AnalysePE::SetHeaders() {
//	if (!IsPEFile()) {
//		MessageBox(0, TEXT("不是有效的PE文件!"), TEXT("ERROR"), MB_OK);
//		return false;
//	}
//
//	headers_.dosHeader = (IMAGE_DOS_HEADER*)fileBuffer_.get();
//	headers_.ntHeader = (IMAGE_NT_HEADERS*)((DWORD)headers_.dosHeader + headers_.dosHeader->e_lfanew);
//	headers_.fileHeader = (IMAGE_FILE_HEADER*)((DWORD)headers_.ntHeader + 4);
//	headers_.optionalHeader = (IMAGE_OPTIONAL_HEADER*)((DWORD)headers_.fileHeader + IMAGE_SIZEOF_FILE_HEADER);
//	headers_.sectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)headers_.optionalHeader + headers_.fileHeader->SizeOfOptionalHeader);
//
//	return true;
//}
//
//bool AnalysePE::HaveTable(DWORD index) {
//	if (index < 0 || index > 15) {
//		return false;
//	}
//	if (headers_.optionalHeader->DataDirectory[index].Size != 0
//		&& headers_.optionalHeader->DataDirectory[index].VirtualAddress != 0) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//const DWORD AnalysePE::GetAllTableSize() {
//	DWORD exportSize = GetExportSize() + GetFATSize() + GetFNTSize() + GetFOTSize() + GetExportFuncNameSize();
//	DWORD importSize = 0;
//	DWORD importTable = GetAllImportSize() / sizeof(IMAGE_IMPORT_DESCRIPTOR);
//	for (DWORD i = 0; i < (GetAllImportSize() / sizeof(IMAGE_IMPORT_DESCRIPTOR)); i++) {
//		importSize += GetIATSize(i) + GetINTSize(i) + GetByNameTableSize(i);
//	}
//	importSize += GetAllImportSize();
//	DWORD relocationSize = GetAllRelocationSize();
//	DWORD allSize = headers_.optionalHeader->SectionAlignment
//		* ceil(static_cast<float>(importSize + exportSize + relocationSize) / static_cast<float>(headers_.optionalHeader->SectionAlignment));
//
//	return allSize;
//}
//
//IMAGE_EXPORT_DIRECTORY* AnalysePE::GetExport() {
//	return (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
//}
//
//DWORD AnalysePE::GetExportSize() {
//	return sizeof(IMAGE_EXPORT_DIRECTORY);
//}
//
//WORD AnalysePE::GetOrdinalTableIndex(DWORD value) {
//	IMAGE_EXPORT_DIRECTORY* exportPtr = (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
//	WORD* ordinalTable = (WORD*)((DWORD)headers_.dosHeader
//		+ RVAToFOA((DWORD)exportPtr->AddressOfNameOrdinals));
//
//	for (WORD i = 0; i < exportPtr->NumberOfNames; i++) {
//		if (*(ordinalTable + i) == value) {
//			return i;
//		}
//	}
//	return -1;
//}
//
//char* AnalysePE::GetExportFuncName(DWORD index) {
//	IMAGE_EXPORT_DIRECTORY* exportPtr = (IMAGE_EXPORT_DIRECTORY*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[0].VirtualAddress));
//	DWORD* nameOffset = (DWORD*)((DWORD)headers_.dosHeader
//		+ RVAToFOA((DWORD)exportPtr->AddressOfNames));
//
//	char* name = (char*)((DWORD)headers_.dosHeader + RVAToFOA(*(nameOffset + index)));
//	return name;
//}
//
//DWORD AnalysePE::GetFATSize() {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	//DWORD* FAT = (DWORD*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfFunctions));
//	return exportTable->NumberOfFunctions * sizeof(DWORD);
//}
//
//DWORD AnalysePE::GetFNTSize() {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	return exportTable->NumberOfNames * sizeof(DWORD);
//}
////
//DWORD AnalysePE::GetFOTSize() {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	return exportTable->NumberOfNames * sizeof(WORD);
//}
//
//DWORD AnalysePE::GetExportFuncNameSize() {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	DWORD* FNT = (DWORD*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfNames));
//	char* funcName = (char*)((DWORD)headers_.dosHeader + RVAToFOA(*FNT));
//	DWORD funcNameSize = 0;
//	for (DWORD i = 0; i < exportTable->NumberOfNames; i++) {
//		funcNameSize += (strlen(funcName) + 1);
//		funcName = (char*)((DWORD)headers_.dosHeader + RVAToFOA(*(++FNT)));
//	}
//	return funcNameSize;
//}
//
//bool AnalysePE::IsHaveExport() {
//	if (headers_.optionalHeader->DataDirectory[0].Size != 0
//		&& headers_.optionalHeader->DataDirectory[0].VirtualAddress != 0) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//IMAGE_RESOURCE_DIRECTORY* AnalysePE::GetResource() {
//	return (IMAGE_RESOURCE_DIRECTORY*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[2].VirtualAddress));
//}
//
//void DisplayResource(IMAGE_RESOURCE_DIRECTORY_ENTRY* resourceEntry) {
//	IMAGE_RESOURCE_DIRECTORY* resourceTable = AnalysePE::GetAnalyse().GetResource();
//	IMAGE_RESOURCE_DATA_ENTRY* data = (IMAGE_RESOURCE_DATA_ENTRY*)((DWORD)resourceTable + resourceEntry->OffsetToDirectory);
//	//cout << "RVA is: " << std::hex <<data->OffsetToData << "   " << "Size is: " << std::hex << data->Size;
//	cout << format("RVA is: {:x}, Size is {:x}", data->OffsetToData, data->Size);
//}
//
//void GetAppName() {
//	IMAGE_RESOURCE_DIRECTORY* resourceTable = AnalysePE::GetAnalyse().GetResource();
//	IMAGE_RESOURCE_DIRECTORY_ENTRY* stringTableEntry = nullptr;
//	for (int i = 1; i <= (resourceTable->NumberOfIdEntries + resourceTable->NumberOfNamedEntries); i++) {
//		stringTableEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((DWORD)resourceTable + sizeof(IMAGE_RESOURCE_DIRECTORY) * i);
//		if (stringTableEntry->Id == 6) {
//			break;
//		}
//	}
//	IMAGE_RESOURCE_DIRECTORY* firstDir = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)resourceTable + stringTableEntry->OffsetToDirectory);
//	IMAGE_RESOURCE_DIRECTORY_ENTRY* firstEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((DWORD)firstDir + sizeof(IMAGE_RESOURCE_DIRECTORY));
//
//	IMAGE_RESOURCE_DIRECTORY* secondDir = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)resourceTable + firstEntry->OffsetToDirectory);
//	IMAGE_RESOURCE_DIRECTORY_ENTRY* secondEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((DWORD)secondDir + sizeof(IMAGE_RESOURCE_DIRECTORY));
//
//	IMAGE_RESOURCE_DATA_ENTRY* stringData = (IMAGE_RESOURCE_DATA_ENTRY*)((DWORD)resourceTable + secondEntry->OffsetToDirectory);
//	char* index = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
//		+ AnalysePE::GetAnalyse().RVAToFOA(stringData->OffsetToData));
//	for (index; *index == 0; index++) {
//
//	}
//	DWORD nameAddr = (DWORD)index;
//	WORD* name = (WORD*)index;
//	string s;
//	for (name; *name != 0; name++) {
//		s.push_back(*name);
//	}
//	cout << s << "\n";
//	TCHAR other2[] = L"cloud23456";
//	string other = "cloud23456";
//	memcpy((char*)nameAddr, other2, 20);
//}
//
//void AnalysePE::AnalyseResource(IMAGE_RESOURCE_DIRECTORY* resourceDir) {
//	HANDLE h = GetModuleHandleA(nullptr);
//	IMAGE_DOS_HEADER* dosh = (IMAGE_DOS_HEADER*)h;
//	GetAppName();
//	IMAGE_RESOURCE_DIRECTORY* resourceTable = AnalysePE::GetAnalyse().GetResource();
//	IMAGE_RESOURCE_DIRECTORY_ENTRY* resourceEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((DWORD)resourceDir + sizeof(IMAGE_RESOURCE_DIRECTORY));
//	if (!resourceEntry->DataIsDirectory) {
//		DisplayResource(resourceEntry);
//		return;
//	}
//	else {
//		cout << "-------------------------------\n";
//		IMAGE_RESOURCE_DIRECTORY_ENTRY* resourceEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((DWORD)resourceDir + sizeof(IMAGE_RESOURCE_DIRECTORY));
//		DWORD number = resourceDir->NumberOfIdEntries + resourceDir->NumberOfNamedEntries;
//		for (int i = 1; i <= number; i++) {
//			cout << "ID: " << (resourceEntry->NameIsString ? NULL : resourceEntry->Id) << "\n";
//			AnalyseResource((IMAGE_RESOURCE_DIRECTORY*)((DWORD)resourceTable + resourceEntry->OffsetToDirectory));
//			resourceEntry++;
//		}
//		cout << "-------------------------------\n";
//		return;
//	}
//}
//
//IMAGE_IMPORT_DESCRIPTOR* AnalysePE::GetImport() {
//	return (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
//}
//
//DWORD AnalysePE::GetAllImportSize() {
//	DWORD importNum = 0;
//	IMAGE_IMPORT_DESCRIPTOR* tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader +
//		RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
//	for (; tempImport->OriginalFirstThunk != 0 && tempImport->FirstThunk != 0;tempImport++) {
//		importNum++;
//	}
//	// 通过目录表得到的size比实际size大，因为目录表中的计算方式是通过起始位置计算的，那样会计算到用0填充的部分
//	DWORD allImportSize = importNum * sizeof(IMAGE_IMPORT_DESCRIPTOR);
//	return allImportSize;
//}
//
//DWORD AnalysePE::GetINTSize(DWORD importIndex) {
//	if (importIndex < 0 || importIndex >= headers_.optionalHeader->DataDirectory[1].Size) {
//		return 0;
//	}
//	IMAGE_IMPORT_DESCRIPTOR* import = GetImport() + importIndex;
//
//	DWORD size = 0;
//
//	IMAGE_THUNK_DATA * first = (IMAGE_THUNK_DATA*)((DWORD)headers_.dosHeader + RVAToFOA(import->OriginalFirstThunk));
//	while (first->u1.Ordinal != 0) {
//		size++;
//		first++;
//	}
//	import++;
//
//	return size * sizeof(IMAGE_THUNK_DATA);
//}
//DWORD AnalysePE::GetIATSize(DWORD importIndex) {
//	return GetINTSize(importIndex);
//}
//
//DWORD AnalysePE::GetByNameTableSize(DWORD importIndex) {
//	if (importIndex < 0 || importIndex >= headers_.optionalHeader->DataDirectory[1].Size) {
//		return 0;
//	}
//
//	IMAGE_IMPORT_DESCRIPTOR* import = GetImport() + importIndex;
//	IMAGE_THUNK_DATA* INT = (IMAGE_THUNK_DATA*)((DWORD)headers_.dosHeader + RVAToFOA(import->OriginalFirstThunk));
//	IMAGE_IMPORT_BY_NAME* first = (IMAGE_IMPORT_BY_NAME*)((DWORD)headers_.dosHeader + RVAToFOA(INT->u1.Function));
//	DWORD byNameTableSize = 0;
//	for (const DWORD* tempINT = (DWORD*)INT; *tempINT != 0; tempINT++) {
//		if ((*tempINT & 0x10000000) == 0) {
//			byNameTableSize = byNameTableSize + sizeof(WORD) + strlen(first->Name) + 1;
//		}
//	}
//	return byNameTableSize;
//}
//
//bool AnalysePE::IsHaveImport() {
//	if (headers_.optionalHeader->DataDirectory[1].Size != 0
//		&& headers_.optionalHeader->DataDirectory[1].VirtualAddress != 0) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//bool AnalysePE::IsHaveRelocation() {
//	if (headers_.optionalHeader->DataDirectory[5].Size != 0
//		&& headers_.optionalHeader->DataDirectory[5].VirtualAddress != 0) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//IMAGE_BASE_RELOCATION* AnalysePE::GetRelocation() {
//	return (IMAGE_BASE_RELOCATION*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[5].VirtualAddress));
//}
//
//IMAGE_BASE_RELOCATION* AnalysePE::GetRelocation(DWORD index) {
//	IMAGE_BASE_RELOCATION* tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[5].VirtualAddress));
//	for (DWORD i = 0; i < index; i++) {
//		tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)tempRelocation
//			+ tempRelocation->SizeOfBlock);
//	}
//	return tempRelocation;
//}
//
//DWORD AnalysePE::GetAllRelocationSize() {
//	if (!HaveTable(5)) {
//		return 0;
//	}
//	DWORD size= 0;
//	IMAGE_BASE_RELOCATION* temp = GetRelocation();
//	for (DWORD i = 0; temp->VirtualAddress != 0 && temp->SizeOfBlock != 0; i++) {
//		size += temp->SizeOfBlock;
//		temp = (IMAGE_BASE_RELOCATION*)((DWORD)temp + temp->SizeOfBlock);
//	}
//	return size;
//}
//
//DWORD AnalysePE::RVAToFOA(const DWORD RVA){
//	DWORD FOA = 0;
//	PositionInPE pos = PositonInfoRVA(RVA);
//	switch (pos) {
//	case PositionInPE::inHead:
//	{
//		FOA = RVA;
//		return FOA;
//	}
//	case PositionInPE::inSection:
//	{
//		DWORD sectionIndex = InWhichSectionRVA(RVA);
//		IMAGE_SECTION_HEADER a = headers_.sectionHeader[sectionIndex];
//		FOA = RVA - headers_.sectionHeader[sectionIndex].VirtualAddress
//			+ headers_.sectionHeader[sectionIndex].PointerToRawData;;
//		return FOA;
//	}
//	default:
//		return FOA;
//	}
//}
//
//DWORD AnalysePE::FOAToRVA(const DWORD FOA) {
//	DWORD RVA = 0;
//	PositionInPE pos = PositonInfoFOA(FOA);
//
//	switch (pos) {
//	case PositionInPE::inHead:
//	{
//		RVA = FOA;
//		return RVA;
//	}
//	case PositionInPE::inSection:
//	{
//		DWORD sectionIndex = InWhichSectionFOA(FOA);
//		RVA = FOA - headers_.sectionHeader[sectionIndex].PointerToRawData
//			+ headers_.sectionHeader[sectionIndex].VirtualAddress;
//		return RVA;
//	}
//	default:
//		return RVA;
//	}
//}
//
//PositionInPE AnalysePE::PositonInfoRVA(const DWORD RVA) {
//	PositionInPE Pos;
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	if (RVA < headers_.sectionHeader->VirtualAddress) {
//		Pos = PositionInPE::inHead;
//	}
//	// TODO：SizeOfImage的值并不一定准确，例如SizeOfImage = lastSection->RVA + lastSection->VSize，此时未对齐
//	// 而lastSection->RVA + lastSection->VSize之后可能还有内容，考虑边界的是否应该改为max(SizeOfImage, lastSection->RVA + 内存对齐(VSize))
//	else if (RVA >= max(headers_.optionalHeader->SizeOfImage, lastSectionHeader->VirtualAddress + GetImageSectionSizeAlignment(*lastSectionHeader))) {
//		Pos = PositionInPE::outFile;
//	}
//	/*else if (RVA >= headers_.optionalHeader->SizeOfImage) {
//		Pos = outFile;
//	}*/
//	else {
//		Pos = PositionInPE::inSection;
//	}
//	return Pos;
//}
//
//PositionInPE AnalysePE::PositonInfoFOA(const DWORD FOA) {
//	PositionInPE Pos;
//
//	if (FOA < headers_.optionalHeader->SizeOfHeaders) {
//		Pos = PositionInPE::inHead;
//	}
//	else if (DWORD lastSectionIndex = headers_.fileHeader->NumberOfSections - 1; 
//		FOA >= headers_.sectionHeader[lastSectionIndex].PointerToRawData
//		+ headers_.sectionHeader[lastSectionIndex].SizeOfRawData) {
//		Pos = PositionInPE::outFile;
//	}
//	else {
//		Pos = PositionInPE::inSection;
//	}
//	return Pos;
//}
//
//DWORD AnalysePE::InWhichSectionRVA(const DWORD RVA) {
//	DWORD sectionIndex = headers_.fileHeader->NumberOfSections - 1;
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	DWORD sectionEdge = headers_.optionalHeader->SizeOfImage;
//
//	while (sectionIndex >= 0) {
//		if (RVA >= headers_.sectionHeader[sectionIndex].VirtualAddress
//			&& RVA <= sectionEdge) {
//			return sectionIndex;
//		}
//		sectionEdge = headers_.sectionHeader[sectionIndex].VirtualAddress;
//		sectionIndex--;		
//	}
//}
//
//DWORD AnalysePE::InWhichSectionFOA(const DWORD FOA) {
//	DWORD sectionIndex = headers_.fileHeader->NumberOfSections - 1;
//
//	while (sectionIndex >= 0) {
//		if (FOA >= headers_.sectionHeader[sectionIndex].PointerToRawData
//			&& FOA < headers_.sectionHeader[sectionIndex].PointerToRawData
//			+ headers_.sectionHeader[sectionIndex].SizeOfRawData) {
//
//			return sectionIndex;
//		}
//		sectionIndex--;
//	}
//}
//
//void AnalysePE::GetTcharSectionName(const DWORD secIndex, TCHAR** name) {
//	char* cTemp = new char[sizeof(char) * 8];
//	for (int i = 0; i < 8; i++) {
//		cTemp[i] = headers_.sectionHeader[secIndex].Name[i];
//	}
//	TCHAR* tTemp = nullptr;
//	CharToTchar(cTemp, &tTemp);
//	*name = tTemp;
//}
//
//void AnalysePE::GetCharSectionName(const DWORD secIndex, char** name) {
//	char* cTemp = new char[sizeof(char) * 8];
//	for (int i = 0; i < 8; i++) {
//		cTemp[i] = headers_.sectionHeader[secIndex].Name[i];
//	}
//	*name = cTemp;
//}
//
//// 内存映像中section的实际大小，取二者中最大的，然后进行内存对齐
//DWORD AnalysePE::GetImageSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader) {
//	DWORD imageSectionSize = headers_.optionalHeader->SectionAlignment
//		* ceil(static_cast<float>(max(sectionHeader.Misc.VirtualSize, sectionHeader.SizeOfRawData)) / static_cast<float>(headers_.optionalHeader->SectionAlignment));
//	return imageSectionSize;
//}
//
//// 磁盘文件中section的实际大小，取二者中最小的，然后进行磁盘对齐
//DWORD AnalysePE::GetFileSectionSizeAlignment(const IMAGE_SECTION_HEADER& sectionHeader) {
//	DWORD fileSectionSize = headers_.optionalHeader->FileAlignment
//		* ceil(static_cast<float>(min(sectionHeader.Misc.VirtualSize, sectionHeader.SizeOfRawData)) / static_cast<float>(headers_.optionalHeader->FileAlignment));
//	return fileSectionSize;
//}
//
//void AnalysePE::SetSectionCharacter(IMAGE_SECTION_HEADER& secHeader, DWORD cha) {
//	secHeader.Characteristics = cha;
//}
//
//void AnalysePE::EnlargeLastSection(DWORD enlargeSize) {
//	if (enlargeSize != 0) {
//		return;
//	}
//	DWORD oldSectionSize = (headers_.sectionHeader
//		+ headers_.fileHeader->NumberOfSections - 1)->SizeOfRawData;
//	char* tempBuffer = nullptr;
//	DWORD alignmentSize = ceil(static_cast<float>(enlargeSize) / static_cast<float>(headers_.optionalHeader->FileAlignment))
//		* headers_.optionalHeader->FileAlignment;
//	tempBuffer = new char[fileBufferSize_ + alignmentSize];
//
//	if (tempBuffer == nullptr) {
//		MessageBox(0, TEXT("Enlarge失败!"), TEXT("ERROR"), MB_OK);
//		return;
//	}
//	memset(tempBuffer, 0, fileBufferSize_ + alignmentSize);
//	memcpy(tempBuffer, fileBuffer_.get(), fileBufferSize_);
//	fileBufferSize_ += alignmentSize;
//	fileBuffer_.reset(tempBuffer);
//
//	SetHeaders();
//	IMAGE_SECTION_HEADER* newLastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1)->SizeOfRawData = alignmentSize + oldSectionSize;
//
//	headers_.optionalHeader->SizeOfImage += GetImageSectionSizeAlignment(*(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1));
//}
//
//void AnalysePE::MoveImport() {
//	DWORD allImportSize = GetAllImportSize();
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	//DWORD oldLastSectionSize = lastSectionHeader->SizeOfRawData;
//	// 考虑两种情况，1.VirtualSize <= SizeOfRawData，那么VirtualSize反映了数据真实的大小，空闲空间的计算正确
//	// 2.VirtualSize > SizeOfRawData(此时freeSpace为负)，此时区块内有大量未初始化的数据，无法写入
//	/*DWORD freeSpace = fileBufferSize_ - (lastSectionHeader->PointerToRawData + min(lastSectionHeader->Misc.VirtualSize, lastSectionHeader->SizeOfRawData));
//	if(freeSpace <= allImportSize) {
//		EnlargeLastSection(allImportSize - freeSpace);
//	}*/
//	DWORD newImportAddr = (DWORD)headers_.dosHeader + (headers_.sectionHeader
//		+ headers_.fileHeader->NumberOfSections - 1)->PointerToRawData
//		+ lastSectionHeader->Misc.VirtualSize;
//	
//	DWORD oldImportAddr = (DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress);
//	memcpy((char*)newImportAddr, (char*)oldImportAddr, allImportSize);
//
//
//	headers_.optionalHeader->DataDirectory[1].VirtualAddress = FOAToRVA(newImportAddr - (DWORD)headers_.dosHeader);
//}
//
//void AnalysePE::AddImport(const TCHAR tDllName[], const TCHAR tFuncName[]) {
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//
//	DWORD allImportSize = GetAllImportSize();
//	DWORD freeSpace = fileBufferSize_ - (headers_.sectionHeader
//		+ headers_.fileHeader->NumberOfSections - 1)->PointerToRawData;
//
//	IMAGE_IMPORT_DESCRIPTOR* tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
//	while (tempImport->OriginalFirstThunk != 0 && tempImport->FirstThunk != 0) {
//		tempImport++;
//	}
//
//	//创建新Import
//	IMAGE_IMPORT_DESCRIPTOR* newImport = new IMAGE_IMPORT_DESCRIPTOR();
//	memset(newImport, 0, 20);
//	memcpy((char*)tempImport, (char*)newImport, 20);
//	delete newImport;
//	newImport = tempImport;
//	memset((char*)(tempImport+1), 0, 20);
//	tempImport += 3;
//
//	//创建新的INT
//	//tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_IMPORT_BY_NAME*)tempImport + 2);
//	IMAGE_THUNK_DATA* newINT = new IMAGE_THUNK_DATA();
//	memset(newINT, 0, sizeof(IMAGE_THUNK_DATA));
//	memcpy((char*)tempImport, newINT, sizeof(IMAGE_THUNK_DATA));
//	delete newINT;
//	newINT = (IMAGE_THUNK_DATA*)tempImport;
//	memset((char*)(newINT + 1), 0, sizeof(IMAGE_THUNK_DATA));
//	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_THUNK_DATA*)tempImport + 3);
//
//	//创建新的IAT表
//	IMAGE_THUNK_DATA* newIAT = new IMAGE_THUNK_DATA();
//	memset(newIAT, 0, sizeof(IMAGE_THUNK_DATA));
//	memcpy((char*)tempImport, newIAT, sizeof(IMAGE_THUNK_DATA));
//	delete newIAT;
//	newIAT = (IMAGE_THUNK_DATA*)tempImport;
//	memset((char*)(newIAT + 1), 0, sizeof(IMAGE_THUNK_DATA));
//	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_THUNK_DATA*)tempImport + 3);
//
//	// 为新建的Import表赋值
//	char* cDllname = new char();
//	TcharToChar(tDllName, &cDllname);
//	std::string s;
//	s += cDllname;
//	delete cDllname;
//	cDllname = (char*)tempImport;
//	memcpy((char*)cDllname, s.c_str(), s.size());
//
//	newImport->OriginalFirstThunk = FOAToRVA((DWORD)newINT - (DWORD)headers_.dosHeader);
//	newImport->FirstThunk = FOAToRVA((DWORD)newIAT - (DWORD)headers_.dosHeader);
//	newImport->Name = FOAToRVA((DWORD)cDllname - (DWORD)headers_.dosHeader);
//
//	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((char*)tempImport + s.size() + 1);
//
//	//创建新的ByName表
//	IMAGE_IMPORT_BY_NAME* newByname = new IMAGE_IMPORT_BY_NAME;
//	memset((char*)newByname, 0, sizeof(IMAGE_IMPORT_BY_NAME));
//	memcpy((char*)tempImport, newByname, sizeof(IMAGE_IMPORT_BY_NAME));
//	delete newByname;
//	newByname = (IMAGE_IMPORT_BY_NAME*)tempImport;
//	tempImport = (IMAGE_IMPORT_DESCRIPTOR*)((IMAGE_IMPORT_BY_NAME*)tempImport + 2);
//
//
//
//	// 为新建的INT与IAT表赋值
//	newINT->u1.AddressOfData = FOAToRVA((DWORD)newByname - (DWORD)headers_.dosHeader);
//	newIAT->u1.AddressOfData = FOAToRVA((DWORD)newByname - (DWORD)headers_.dosHeader);
//
//
//	// 为新建的ByName表赋值
//
//	newByname->Hint = 0;
//	char* funcName = nullptr;
//	TcharToChar(tFuncName, &funcName);
//	char* name = newByname->Name;
//	int i = 0;
//	for (i; *(funcName + i) != '\0'; i++) {
//		*(name + i) = *(funcName + i);
//	}
//	/**(name + i) = '\0';*/
//}
//
//// 判断最后一个区块表与以一个区块之间的的空间是否可以添加新区块表
//// 因为PointerToRawData在特定情况下可能为0，所以实际的空闲空间并不能靠该值计算
//bool AnalysePE::AddSectionHeaderIfAvailable() {
//	DWORD freeSpaceSize = headers_.optionalHeader->SizeOfHeaders - ((DWORD)headers_.sectionHeader - (DWORD)headers_.dosHeader)
//		- headers_.fileHeader->NumberOfSections * IMAGE_SIZEOF_SECTION_HEADER;
//
//	// 40字节给新的sectionHeader结构、40字节全0做结束标志
//	if (freeSpaceSize < IMAGE_SIZEOF_SECTION_HEADER * 2) {
//		return false;
//	}
//	return true;
//}
//
//void AnalysePE::AdjustHeadrs() {
//	headers_.dosHeader->e_lfanew = sizeof(*headers_.dosHeader);
//
//	DWORD moveHeadersSize = sizeof(headers_.ntHeader->Signature) + IMAGE_SIZEOF_FILE_HEADER
//		+ headers_.fileHeader->SizeOfOptionalHeader
//		+ IMAGE_SIZEOF_SECTION_HEADER * headers_.fileHeader->NumberOfSections;
//	memcpy((void*)((DWORD)headers_.dosHeader + headers_.dosHeader->e_lfanew), headers_.ntHeader, moveHeadersSize);
//
//	SetHeaders();
//	DWORD freeSpaceSize = headers_.optionalHeader->SizeOfHeaders - ((DWORD)headers_.sectionHeader - (DWORD)headers_.dosHeader)
//		- headers_.fileHeader->NumberOfSections * sizeof(*headers_.sectionHeader);
//
//	memset(headers_.sectionHeader + headers_.fileHeader->NumberOfSections, 0, freeSpaceSize);
//}
//
//void AnalysePE::AddSection(DWORD SectionSize, bool needAlignment) {
//	// 判断最后一个区块后是否还有信息
//	bool haveInfo = HaveInfo();
//	DWORD infoSize = 0;
//	char* buffer = nullptr;
//	if (haveInfo) {
//		infoSize = GetInfoBuffer(&buffer);
//	}
//
//	IMAGE_SECTION_HEADER mySectionHeader{0};
//
//	// 初始化自定义的区块表，其中PointerToRawData需要实际添加后才可确定值，故由AddSectionHeader()函数完成
//	std::string name = ".mySec";
//	for (int i = 0; name[i] != 0 && i < IMAGE_SIZEOF_SHORT_NAME; i++) {
//		mySectionHeader.Name[i] = name[i];
//	}
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	mySectionHeader.VirtualAddress = lastSectionHeader->VirtualAddress + GetImageSectionSizeAlignment(*lastSectionHeader); 	
//	DWORD fileSectionSize = 0;
//	if (needAlignment) {
//		fileSectionSize = headers_.optionalHeader->FileAlignment
//			* ceil(static_cast<float>(SectionSize) / static_cast<float>(headers_.optionalHeader->FileAlignment));
//	}
//	else {
//		fileSectionSize = SectionSize;
//	}
//	mySectionHeader.Misc.VirtualSize = fileSectionSize;
//	mySectionHeader.SizeOfRawData = fileSectionSize;
//	mySectionHeader.Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;
//
//	// 确定自定义Section在文件中的位置，即PointerToRawData，为了防止最后一个Section是一个完全未初始化的Section(即SizeOfRawData和PointerToRawData都为0)
//	// 故需要从LastSection向前遍历，找到第一个PointerToRawData不为0的Section，将自定义Section复制到该节之后
//	for (int i = headers_.fileHeader->NumberOfSections - 1; i >= 0; i--) {
//		if ((headers_.sectionHeader + i)->PointerToRawData != 0 && (headers_.sectionHeader + i)->SizeOfRawData != 0) {
//			mySectionHeader.PointerToRawData = (headers_.sectionHeader + i)->PointerToRawData + GetFileSectionSizeAlignment(*(headers_.sectionHeader + i));
//			break;
//		}
//	}
//
//	char* newFileBuffer = new char[fileBufferSize_ + mySectionHeader.SizeOfRawData];
//	memset(newFileBuffer, 0, fileBufferSize_ + mySectionHeader.SizeOfRawData);
//	memcpy(newFileBuffer, fileBuffer_.get(), haveInfo ? fileBufferSize_ - infoSize : fileBufferSize_);
//
//	fileBuffer_.reset(newFileBuffer);
//	fileBufferSize_ += mySectionHeader.SizeOfRawData;
//	SetHeaders();
//
//	DWORD infoAddress = (DWORD)headers_.dosHeader + mySectionHeader.PointerToRawData + mySectionHeader.SizeOfRawData;
//	if (haveInfo) {
//		memcpy((char*)infoAddress, buffer, infoSize);
//	}
//
//	AddSectionHeader(&mySectionHeader);
//	headers_.fileHeader->NumberOfSections++;
//	headers_.optionalHeader->SizeOfImage = mySectionHeader.VirtualAddress + GetImageSectionSizeAlignment(mySectionHeader);
//}
//
//// 为新的区块添加区块表，判断Header中的空闲空间是否可以加入区块表，如果不可以，则讲dosHeader之下的所有header抬高（占用dos stub的空间）
//// 若仍然不够，则开辟新空间
//void AnalysePE::AddSectionHeader(IMAGE_SECTION_HEADER* mySectionHeader) {
//	bool isAvailableSpace = AddSectionHeaderIfAvailable();	
//	DWORD newBufferSize = 0;
//
//	if (!isAvailableSpace) {
//		AdjustHeadrs();
//
//		bool isAvailableAdjust = AddSectionHeaderIfAvailable();
//		if (!isAvailableAdjust) {
//			DWORD headerSizeNoAlignment = (DWORD)(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1)
//				+ IMAGE_SIZEOF_SECTION_HEADER - (DWORD)headers_.dosHeader;
//
//			// newBufferSize = 原BufferSize - 原header大小 + 新的对其后的header大小
//			DWORD newHeaderSize = headers_.optionalHeader->FileAlignment 
//				* ceil(static_cast<float>(headerSizeNoAlignment + IMAGE_SIZEOF_SECTION_HEADER) / static_cast<float>(headers_.optionalHeader->FileAlignment));
//			newBufferSize = fileBufferSize_ - headers_.optionalHeader->SizeOfHeaders + newHeaderSize;
//			char* newFileBuffer = new char[newBufferSize];
//			memcpy(newFileBuffer, fileBuffer_.get(), newHeaderSize);
//			memcpy(newFileBuffer + newHeaderSize, fileBuffer_.get() + headers_.optionalHeader->SizeOfHeaders
//				, fileBufferSize_ - headers_.optionalHeader->SizeOfHeaders);
//			fileBuffer_.reset(newFileBuffer);
//			SetHeaders();
//			headers_.optionalHeader->SizeOfHeaders = newHeaderSize;
//			headers_.optionalHeader->SizeOfImage += headerSizeNoAlignment;
//		}		
//	}
//	IMAGE_SECTION_HEADER* temp = (IMAGE_SECTION_HEADER*)headers_.sectionHeader + headers_.fileHeader->NumberOfSections;
//	memcpy(temp, mySectionHeader, IMAGE_SIZEOF_SECTION_HEADER);
//}
//
//void AnalysePE::MoveToNewFileBuffer(const DWORD newBufferSize) {
//	char* newFileBuffer = new char[newBufferSize];
//	if (newFileBuffer == nullptr) {
//		return;
//	}
//	memset(newFileBuffer, 0, newBufferSize);
//	memcpy_s(newFileBuffer, newBufferSize, fileBuffer_.get(), fileBufferSize_);
//	fileBufferSize_ = newBufferSize;
//	fileBuffer_.reset(newFileBuffer);
//	SetHeaders();
//}
//
//void AnalysePE::dll_inject(const std::wstring ws_dll_name, const std::wstring ws_function_name) {
//
//	std::string dll_name(ws_dll_name.begin(), ws_dll_name.end());
//	std::string function_name(ws_function_name.begin(), ws_function_name.end());
//	//char* cDllName = nullptr;
//	//TcharToChar(tDllName, &cDllName);
//	//char* cFuncName = nullptr;
//	//TcharToChar(tFuncName, &cFuncName);
//
//	// injectSpaceSize的最小值 = （导入表 + INT表 + IAT表）* 2 + byName表中HINT的大小(2) + byName表大小 + len(tDllName) + len(tFuncNAme) + allImportSize + 2
//	// 为了方便计算，采用（导入表 + INT表 + IAT表 + byName表）* 2 + len(tDllName) + len(tFuncNAme) + allImportSize + 2(两个\0)，只比上式多了2字节
//	DWORD injectSpaceSize = (sizeof(IMAGE_IMPORT_DESCRIPTOR) + sizeof(IMAGE_THUNK_DATA) * 2 + sizeof(IMAGE_IMPORT_BY_NAME)) * 2
//		+ strlen(dll_name.c_str()) + strlen(function_name.c_str()) + 2 + GetAllImportSize();
//
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//
//	std::function<void()> SetFreeSpace = nullptr;
//	DWORD needSize = 0;
//	if (lastSectionHeader->Misc.VirtualSize > lastSectionHeader->SizeOfRawData) {
//		needSize = injectSpaceSize;
//		SetFreeSpace = [&]() {AddSection(needSize, false); };
//	}
//	else {
//		DWORD freeSpaceSize = lastSectionHeader->SizeOfRawData - lastSectionHeader->Misc.VirtualSize;
//		// 错误的计算方式，会覆盖文件的数字签名、调试信息等内容
//		//DWORD freeSpaceSize = fileBufferSize_ - (lastSectionHeader->PointerToRawData + lastSectionHeader->Misc.VirtualSize);
//		if (freeSpaceSize < injectSpaceSize) {
//			needSize = injectSpaceSize - freeSpaceSize;
//		}
//		SetFreeSpace = [&]() {EnlargeLastSection(needSize); };
//		SetSectionCharacter(*lastSectionHeader, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
//	}
//	SetFreeSpace();
//	MoveImport();
//	AddImport(ws_dll_name.c_str(), ws_function_name.c_str());
//}
//
//void AnalysePE::EncodeProcess(char* buffer, DWORD size) {
//	char* index = buffer;
//	for (int i = 0; i < size; i++) {
//		*index = ~*index;
//		index++;
//	}
//}
//
//void AnalysePE::MoveAllTable() {
//	DWORD newSectionSize = GetAllTableSize();
//	AddSection(newSectionSize, false); 
//	
//	DWORD newSectionAddr =	(DWORD)headers_.dosHeader + 
//		(headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1)->PointerToRawData;
//	
//	
//	//MoveExport(newSectionAddr);
//	//char* buffer = new char[fileBufferSize_];
//	//memcpy(buffer2, fileBuffer_.get(), fileBufferSize_);
//	//SetHeaders();
//	//fileBufferSize_ += size;
//	//IMAGE_SECTION_HEADER* last = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	//memset((char*)((DWORD)headers_.dosHeader + last->PointerToRawData), 0, size);
//	
//	//char* p = (char*)((DWORD)headers_.dosHeader + last->PointerToRawData + last->SizeOfRawData);
//	//memcpy(p, buffer, size);
//
//	MoveImport(newSectionAddr);
//	MoveRelocation(newSectionAddr);
//}
//
//void AnalysePE::MoveExport(DWORD& moveAddress) {
//	if (!HaveTable(0)) {
//		return;
//	}
//	IMAGE_EXPORT_DIRECTORY* oldExportTable = GetExport();
//	DWORD exportSize = GetExportSize();
//	memcpy((char*)moveAddress, oldExportTable, exportSize);
//	DWORD exportRAV = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	headers_.optionalHeader->DataDirectory[0].VirtualAddress = exportRAV;
//	moveAddress += exportSize;
//
//	IMAGE_EXPORT_DIRECTORY* newExportTable = GetExport();	
//	char* oldExportName = (char*)((DWORD)headers_.dosHeader + RVAToFOA(newExportTable->Name));
//	memcpy((char*)moveAddress, oldExportName, strlen(oldExportName));
//	newExportTable->Name = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	moveAddress += (strlen(oldExportName) + 1);
//
//	MoveExportFAT(moveAddress);
//	MoveExportFNT(moveAddress);
//	MoveExportFOT(moveAddress);
//	MoveExportFuncName(moveAddress);
//}
//
//void AnalysePE::MoveExportFAT(DWORD& moveAddress) {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	char* oldFAT = (char*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfFunctions));
//	DWORD FATSize = GetFATSize();
//	memcpy((char*)moveAddress, oldFAT, FATSize);
//	exportTable->AddressOfFunctions = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	moveAddress += FATSize;
//}
//
//void AnalysePE::MoveExportFNT(DWORD& moveAddress) {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	char* oldFNT = (char*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfNames));
//	DWORD FNTSize = GetFNTSize();
//	memcpy((char*)moveAddress, oldFNT, FNTSize);
//	exportTable->AddressOfNames = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	moveAddress += FNTSize;
//}
//
//void AnalysePE::MoveExportFOT(DWORD& moveAddress) {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	char* oldFOT = (char*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfNameOrdinals));
//	DWORD FOTSize = GetFOTSize();
//	memcpy((char*)moveAddress, oldFOT, FOTSize);
//	exportTable->AddressOfNameOrdinals = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	moveAddress += FOTSize;
//}
//
//void AnalysePE::MoveExportFuncName(DWORD& moveAddress) {
//	IMAGE_EXPORT_DIRECTORY* exportTable = GetExport();
//	DWORD* FNT = (DWORD*)((DWORD)headers_.dosHeader + RVAToFOA(exportTable->AddressOfNames));
//	char* oldFuncName = (char*)((DWORD)headers_.dosHeader + RVAToFOA(*FNT));
//	DWORD funcNameSize = GetExportFuncNameSize();
//	memcpy((char*)moveAddress, oldFuncName, funcNameSize);
//	for (int i = 0; i < exportTable->NumberOfNames; i++) {
//		*FNT = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//		FNT++;
//		moveAddress = moveAddress + strlen((char*)moveAddress) + 1;
//	}
//}
//
//void AnalysePE::MoveImport(DWORD& MoveAddress) {
//	if (!HaveTable(1)) {
//		return;
//	}
//	IMAGE_IMPORT_DESCRIPTOR* firstImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)headers_.dosHeader
//		+ RVAToFOA(headers_.optionalHeader->DataDirectory[1].VirtualAddress));
//
//	DWORD importSize = headers_.optionalHeader->DataDirectory[1].Size;
//	memcpy((char*)MoveAddress, firstImport, importSize);
//	DWORD importRVA = FOAToRVA(MoveAddress - (DWORD)headers_.dosHeader);
//	headers_.optionalHeader->DataDirectory[1].VirtualAddress = importRVA;
//	IMAGE_IMPORT_DESCRIPTOR* firstnewImport = (IMAGE_IMPORT_DESCRIPTOR*)MoveAddress;
//	MoveAddress += importSize;
//
//	for (IMAGE_IMPORT_DESCRIPTOR* temp = firstnewImport; temp->FirstThunk != 0 && temp->OriginalFirstThunk != 0; temp++) {
//		char* name = (char*)((DWORD)headers_.dosHeader + RVAToFOA(temp->Name));
//		DWORD nameLen = strlen(name) + 1;
//		memcpy((char*)MoveAddress, name, nameLen);
//		temp->Name = FOAToRVA(MoveAddress - (DWORD)headers_.dosHeader);
//		MoveAddress += nameLen;
//	}
//	MoveINT(MoveAddress);
//	MoveByNameTable(MoveAddress);
//	//MoveIAT(MoveAddress);
//}
//
//void AnalysePE::MoveINT(DWORD& moveAddress) {
//	IMAGE_IMPORT_DESCRIPTOR* firstImport = GetImport();
//	int index = 0;
//	for (IMAGE_IMPORT_DESCRIPTOR* temp = firstImport; temp->FirstThunk != 0 && temp->OriginalFirstThunk != 0; temp++, index++) {
//		DWORD INTSize = GetINTSize(index);
//		IMAGE_THUNK_DATA* INT = (IMAGE_THUNK_DATA*)((DWORD)headers_.dosHeader + RVAToFOA(temp->OriginalFirstThunk));
//		memcpy((char*)moveAddress, INT, INTSize);
//		temp->OriginalFirstThunk = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//		moveAddress += (INTSize + sizeof(IMAGE_THUNK_DATA));
//	}
//}
//
//void AnalysePE::MoveIAT(DWORD& moveAddress) {
//	// 因为PE中间接调用写的是IAT表中的内容，移动IAT表需要修改程序中所有的间接调用的位置，过于麻烦
//	// 暂时没想到一个好的方法解决该问题
//}
//
//void AnalysePE::MoveByNameTable(DWORD& moveAddress) {
//	IMAGE_IMPORT_DESCRIPTOR* firstImport = GetImport();
//	int index = 0;
//	for (IMAGE_IMPORT_DESCRIPTOR* temp = firstImport; temp->FirstThunk != 0 && temp->OriginalFirstThunk != 0; temp++, index++) {
//		DWORD byNameTableSize = GetByNameTableSize(index);
//		IMAGE_THUNK_DATA* INT = (IMAGE_THUNK_DATA*)((DWORD)headers_.dosHeader + RVAToFOA(temp->OriginalFirstThunk));
//		IMAGE_THUNK_DATA* IAT = (IMAGE_THUNK_DATA*)((DWORD)headers_.dosHeader + RVAToFOA(temp->FirstThunk));
//		IMAGE_IMPORT_BY_NAME* firstByNameTable = (IMAGE_IMPORT_BY_NAME*)((DWORD)headers_.dosHeader + RVAToFOA(*(DWORD*)INT));
//		memcpy((char*)moveAddress, firstByNameTable, byNameTableSize);
//		INT->u1.AddressOfData = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//		IAT->u1.AddressOfData = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//		moveAddress += (byNameTableSize + 1);
//	}
//}
//
//void AnalysePE::MoveRelocation(DWORD& moveAddress) {
//	if (!HaveTable(5)) {
//		return;
//	}
//
//	DWORD allRelocationSize = GetAllRelocationSize();
//	IMAGE_BASE_RELOCATION* oldRelocation = GetRelocation();
//	memcpy((char*)moveAddress, oldRelocation, allRelocationSize);
//	headers_.optionalHeader->DataDirectory[5].VirtualAddress = FOAToRVA(moveAddress - (DWORD)headers_.dosHeader);
//	moveAddress += allRelocationSize;
//}
//
//
//bool AnalysePE::HaveInfo() {
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	if (fileBufferSize_ > (lastSectionHeader->PointerToRawData + lastSectionHeader->SizeOfRawData)) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//DWORD AnalysePE::GetInfoBuffer(char** buffer) {
//	IMAGE_SECTION_HEADER* lastSectionHeader = headers_.sectionHeader + headers_.fileHeader->NumberOfSections - 1;
//	DWORD size = fileBufferSize_ - (lastSectionHeader->PointerToRawData + lastSectionHeader->SizeOfRawData);
//	char* tempBuffer = new char[size];
//	char* infoAddress = (char*)((DWORD)headers_.dosHeader + lastSectionHeader->PointerToRawData + lastSectionHeader->SizeOfRawData);
//
//	memcpy(tempBuffer, infoAddress, size);
//	*buffer = tempBuffer;
//	tempBuffer = nullptr;
//	return size;
//
//}