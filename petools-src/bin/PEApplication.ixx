module;

#include <windows.h>

export module PEApplication;

import DialogEX;
import MainDlg;
import Logger;
import DialogManager;

namespace petools {

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

	export class PEApplication {
	public:
		PEApplication() = default;
		~PEApplication() = default;

		PEApplication(const PEApplication&) = delete;
		PEApplication& operator=(const PEApplication&) = delete;
		PEApplication(PEApplication&&) = delete;
		PEApplication& operator=(PEApplication&&) = delete;

		void RunApplication(HINSTANCE h_instance, int cmd_show) noexcept {
			using namespace petools::gui;

			Logger::instance().init();
			DialogEX::Configure(h_instance, cmd_show);
			DialogMgr().OpenDialog<MainDlg>();
			
		}
	};

} //namespace petools