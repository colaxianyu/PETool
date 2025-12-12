module;

#include <windows.h>

module DialogEX;

using std::move;

namespace petools {

    DialogEX::DialogEX(INT template_id, HWND parent) noexcept
        : template_id_(template_id), parent_hwnd_(parent)
    {

    }

    DialogEX::DialogEX(DialogEX&& other) noexcept
        : template_id_(other.template_id_),
        parent_hwnd_(parent_hwnd_),
        current_hwnd_(move(other.current_hwnd_))
    {
        other.template_id_ = 0;
    }

    DialogEX& DialogEX::operator=(DialogEX&& other) noexcept {
        if (this != &other) {
            template_id_ = other.template_id_;
            parent_hwnd_ = other.parent_hwnd_;
            current_hwnd_ = move(other.current_hwnd_);

            other.template_id_ = 0;
        }
        return *this;
    }

    void DialogEX::configure(HINSTANCE h_instance, int cmd_show) noexcept {
        if (app_instance_.load() != nullptr) {
            return;
        }
        app_instance_.store(h_instance);
        default_cmd_show_.store(cmd_show);
    }

    INT_PTR CALLBACK DialogEX::static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        DialogEX* this_dlg = nullptr;
        if (message == WM_INITDIALOG) {
            this_dlg = reinterpret_cast<DialogEX*>(l_param);
            SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_dlg));
            this_dlg->current_hwnd_ = make_handle<HWND, decltype(&DestroyWindow)>(h_dlg);
            return TRUE;
        }

        this_dlg = reinterpret_cast<DialogEX*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
        if (!this_dlg) {
            return FALSE;
        }

        LRESULT result = this_dlg->handle_message(this_dlg->current_hwnd_, message, w_param, l_param);
        return result;
    }

} //namespace petools