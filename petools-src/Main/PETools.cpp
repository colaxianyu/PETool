#include <windows.h>
#include "fast_io/fast_io.h"
#include "fast_io/fast_io_device.h"
#include "fast_io/fast_io_legacy.h"

import PEApplication;
import MainDlg;
import STL;
using namespace fast_io;

int APIENTRY wWinMain(_In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE h_prev_instance,
    _In_ LPWSTR    lp_cmd_line,
    _In_ int       n_cmd_show)
{
    //fast_io::native_file_loader file(fast_io::mnp::os_c_str(file_path.data()));
 //   std::size_t size = file.size();
 //   std::size_t size2 = file.address_end - file.address_begin;
 //   //fast_io::io::scan(file, str);
 //   fast_io::io::print(size);
 //   fast_io::io::print(size2);
    petools::PEApplication app;
    app.run_application(h_instance, n_cmd_show);
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }
}