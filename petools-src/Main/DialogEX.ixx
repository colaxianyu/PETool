module;

#include <windows.h>

export module DialogEX;

import STL;
import DialogStateMachine;
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
        [[nodiscard]] HWND get_parent_hwnd() const noexcept { return parent_hwnd_.get(); }

        [[nodiscard]] INT get_template_id() const noexcept { return template_id_; }
        [[nodiscard]] DialogStateMachine& get_state_machine() noexcept { return state_machine_; }

        static void configure(HINSTANCE h_instance, int cmd_show) noexcept;
        [[nodiscard]] static HINSTANCE get_instance() noexcept { return app_instance_.load(); }
        [[nodiscard]] static int get_cmd_show() noexcept { return default_cmd_show_.load(); }

    protected:
        WindowHandle current_hwnd_;
        WindowHandleRef parent_hwnd_;

        virtual void init_dialog() {}
        virtual void show_dialog() {
            ShowWindow(current_hwnd_.get(), default_cmd_show_);
            UpdateWindow(current_hwnd_.get());
        }
        virtual void hide_dialog() {}
        virtual void close_dialog() {
            EndDialog(current_hwnd_.get(), 0);
        }

        [[nodiscard]] DialogState get_current_state() const noexcept { return state_machine_.get_current_state(); }
        [[nodiscard]] DialogState get_previous_state() const noexcept { return state_machine_.get_previous_state(); }

        virtual LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) = 0;
    private:
        friend class DialogManager;

        INT template_id_;
        DialogStateMachine state_machine_;
        inline static atomic<HINSTANCE> app_instance_{ nullptr };
        inline static atomic_int default_cmd_show_{ SW_SHOWNORMAL };

        void handle_init() { state_machine_.transition_to(DialogState::Initializing); }
        void handle_show() { state_machine_.transition_to(DialogState::Active); }
        void handle_hide() { state_machine_.transition_to(DialogState::Suspended); }
        void handle_close() { state_machine_.transition_to(DialogState::Closing); }

        void SetTransitions();

        static INT_PTR CALLBACK static_dialog_proc(HWND, UINT, WPARAM, LPARAM);
    };

} //namespace petools