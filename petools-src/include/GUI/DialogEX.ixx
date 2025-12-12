module;

#include <windows.h>

export module DialogEX;

import std.compat;
import WinHandle;

using std::atomic;
using std::atomic_int;
using std::move;

namespace petools {

    export class DialogEX {
    public:
        explicit DialogEX(INT template_id, HWND parent = nullptr) noexcept;

        virtual ~DialogEX() noexcept = default;

        DialogEX(const DialogEX&) = delete;
        DialogEX& operator=(const DialogEX&) = delete;

        DialogEX(DialogEX&&) noexcept;
        DialogEX& operator=(DialogEX&&) noexcept;

        [[nodiscard]] HWND get_current_hwnd() const noexcept { return current_hwnd_.get(); }
        [[nodiscard]] HWND get_parent_hwnd() const noexcept { return parent_hwnd_; }

        [[nodiscard]] INT get_template_id() const noexcept { return template_id_; }

        static void configure(HINSTANCE h_instance, int cmd_show) noexcept;
        [[nodiscard]] static HINSTANCE get_instance() noexcept { return app_instance_.load(); }
        [[nodiscard]] static int get_cmd_show() noexcept { return default_cmd_show_.load(); }

    protected:
        WindowHandle current_hwnd_;
        HWND parent_hwnd_{};

        // Lifecycle hooks for derived dialogs
        virtual void init_dialog() noexcept {}
        virtual void show_dialog() noexcept {
            ShowWindow(current_hwnd_.get(), default_cmd_show_);
            UpdateWindow(current_hwnd_.get());
        }
        virtual void hide_dialog() noexcept {}
        virtual void close_dialog() noexcept {
            EndDialog(current_hwnd_.get(), 0);
        }

        // High-level message handlers. Return true if the message was handled.
        // Default implementations provide reasonable behavior and can be
        // overridden in derived dialogs.
        virtual bool on_init_dialog() noexcept {
            init_dialog();
            return true;
        }

        virtual bool on_command(WORD /*id*/, WORD /*code*/, HWND /*ctrl*/) noexcept {
            return false;
        }

        virtual bool on_close() noexcept {
            close_dialog();
            return true;
        }

        virtual LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) = 0;

    private:
        friend class DialogManager;

        INT template_id_;
        inline static atomic<HINSTANCE> app_instance_{ nullptr };
        inline static atomic_int default_cmd_show_{ SW_SHOWNORMAL };

        // Lifecycle entry points used by DialogManager
        void handle_init() noexcept { init_dialog(); }
        void handle_show() noexcept { show_dialog(); }
        void handle_hide() noexcept { hide_dialog(); }
        void handle_close() noexcept { close_dialog(); }

        static INT_PTR CALLBACK static_dialog_proc(HWND, UINT, WPARAM, LPARAM);
    };

} //namespace petools