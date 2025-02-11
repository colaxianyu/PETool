module;

#include <windows.h>

export module PEApplication;

import STL;
import DialogEX;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



export class PEApplication
{
private:
	PEApplication() = default;
	~PEApplication() = default;

public:
	PEApplication(PEApplication&) = delete;
	PEApplication(const PEApplication&) = delete;
	PEApplication(PEApplication&&) = delete;
	PEApplication& operator=(const PEApplication&) = delete;

	static PEApplication& instance();

	void init_APP(HINSTANCE h_instance, int cmd_show) noexcept;
};
