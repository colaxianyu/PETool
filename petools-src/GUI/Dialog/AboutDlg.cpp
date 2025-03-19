module;

#include <windows.h>
#include "..\GUI\resource.h"

module AboutDlg;

import DialogManager;

namespace petools {

    LRESULT AboutDlg::handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message) {
        case WM_CLOSE:
            dialog_mgr.close_dialog();
            break;
        default:
            return FALSE;
        }
		return FALSE;
    }

} //namespace petools