module;

#include <windows.h>

module DialogEX;

import DialogManager;

namespace petools::gui {

    DialogEX::DialogEX(INT template_id, HWND parent) noexcept
        : template_id_(template_id), parent_hwnd_(parent)
    {

    }

    DialogEX::DialogEX(DialogEX&& other) noexcept
        : template_id_(other.template_id_),
        parent_hwnd_(other.parent_hwnd_),
        current_hwnd_(std::move(other.current_hwnd_))
    {
        other.template_id_ = 0;
    }

    DialogEX& DialogEX::operator=(DialogEX&& other) noexcept {
        if (this != &other) {
            template_id_ = other.template_id_;
            parent_hwnd_ = other.parent_hwnd_;
            current_hwnd_ = std::move(other.current_hwnd_);

            other.template_id_ = 0;
        }
        return *this;
    }

    void DialogEX::Configure(HINSTANCE h_instance, int cmd_show) noexcept {
        if (app_instance_ != nullptr) {
            return;
        }
        app_instance_ = h_instance;
        default_cmd_show_ = cmd_show;
    }

    INT_PTR CALLBACK DialogEX::StaticDialogProc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        DialogEX* this_dlg = nullptr;
        if (message == WM_INITDIALOG) {
            this_dlg = reinterpret_cast<DialogEX*>(l_param);
            SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_dlg));
            this_dlg->current_hwnd_ = make_handle<HWND, decltype(&DestroyWindow)>(h_dlg);
            return this_dlg->OnInitDialog() ? TRUE : FALSE;
        }

        this_dlg = reinterpret_cast<DialogEX*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
        if (!this_dlg) {
            return FALSE;
        }
        try {
            switch (message) {
            case WM_COMMAND: {
                WORD id = LOWORD(w_param);
                WORD code = HIWORD(w_param);
                HWND ctrl = reinterpret_cast<HWND>(l_param);
                if (this_dlg->OnCommand(id, code, ctrl)) {
                    return TRUE;
                }
                break;
            }
            case WM_CLOSE:
                if (this_dlg->OnPreClose()) {
                    DialogMgr().CloseDialog();
                    return TRUE;
                }
                break;
            default:
                break;
            }

            LRESULT result = this_dlg->OnOtherMessage(message, w_param, l_param);
            return result;
        }
        catch (...) {
			//Todo: log the exception
            return FALSE;
        }
    }

} //namespace petools::gui