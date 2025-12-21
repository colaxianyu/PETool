module;

#include <windows.h>

export module DialogEX;

import std.compat;
import WinHandle;

using std::move;

namespace petools::gui {

    export class DialogEX {
    public:
        explicit DialogEX(INT template_id, HWND parent = nullptr) noexcept;

        virtual ~DialogEX() noexcept = default;

        DialogEX(const DialogEX&) = delete;
        DialogEX& operator=(const DialogEX&) = delete;

        DialogEX(DialogEX&&) noexcept;
        DialogEX& operator=(DialogEX&&) noexcept;

        [[nodiscard]] HWND GetCurrentHWND() const noexcept { return current_hwnd_.get(); }
        [[nodiscard]] HWND GetParentHWND() const noexcept { return parent_hwnd_; }

        [[nodiscard]] INT GetTemplateID() const noexcept { return template_id_; }

        static void Configure(HINSTANCE h_instance, int cmd_show) noexcept;
        [[nodiscard]] static HINSTANCE GetInstance() noexcept { return app_instance_; }
        [[nodiscard]] static int GetCmdShow() noexcept { return default_cmd_show_; }

    protected:
        WindowHandle current_hwnd_;
        HWND parent_hwnd_{};

        virtual void InitDialog() noexcept {}
        virtual void ShowDialog() noexcept {
            ::ShowWindow(current_hwnd_.get(), default_cmd_show_);
            ::UpdateWindow(current_hwnd_.get());
        }
        virtual void PreCloseDialog() noexcept {}

        virtual bool OnInitDialog() noexcept {
            InitDialog();
            ShowDialog();
            return true;
        }

        virtual bool OnCommand(WORD, WORD, HWND ) noexcept {
            return false;
        }

        virtual bool OnClose() noexcept {
            PreCloseDialog();
            return true;
        }

        virtual LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) {
            return FALSE;
        }

    private:
        friend class DialogManager;

        INT template_id_;
        inline static HINSTANCE app_instance_{ nullptr };
        inline static int default_cmd_show_{ SW_SHOWNORMAL };

        static INT_PTR CALLBACK StaticDialogProc(HWND, UINT, WPARAM, LPARAM);
    };

} //namespace petools