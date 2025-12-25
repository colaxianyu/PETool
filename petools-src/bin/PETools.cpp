//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
#include <windows.h>
//#include "fast_io/fast_io.h"
//#include "fast_io/fast_io_device.h"
//#include "fast_io/fast_io_legacy.h"

import PEApplication;
import MainDlg;
//using namespace fast_io;

//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

int APIENTRY wWinMain(_In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE h_prev_instance,
    _In_ LPWSTR    lp_cmd_line,
    _In_ int       n_cmd_show)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(1497);
    petools::PEApplication app;
    app.RunApplication(h_instance, n_cmd_show);
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }
    
    //_CrtDumpMemoryLeaks();
}