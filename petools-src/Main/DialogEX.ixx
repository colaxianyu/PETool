module;

#include <windows.h>
#include <vector>
#include <functional>
#include <iostream>

export module DialogEX;

import STL;

using tools::win32::show_error_message;

export enum class DialogEvent { 
	None,
	Init, 
	Show,    
	Hide,        
	Close      
};

//export class DialogEX {
//
//using event_handler = std::function<void(DialogEX*, DialogEvent)>;
//
//protected:
//	inline static HINSTANCE app_instacne_ = nullptr;
//	inline static int cmd_show_ = SW_SHOWNORMAL;
//
//	INT template_id_;
//	HWND h_current_dlg_;
//	HWND h_parent_dlg_;
//	DialogEvent current_event_ = DialogEvent::None;
//
//public:
//	explicit DialogEX(INT id_template, HWND h_parent = nullptr) noexcept;
//	virtual ~DialogEX() noexcept;
//
	//virtual void init_dialog() {};
	//virtual void show_dialog() = 0;
	//virtual void hide_dialog() {};
	//virtual void close_dialog() = 0;
//
//	void set_current_dlg_handle(HWND h_dlg) noexcept { h_current_dlg_ = h_dlg; }
//	//void set_parent_dlg_handle(HWND h_dlg) noexcept { h_parent_dlg_ = h_dlg; }
//	//void set_template_id(INT id) noexcept { template_id_ = id; }
//	void set_current_event(DialogEvent new_event) noexcept { current_event_ = new_event; }
//	
//	static void set_app_instacne_(HINSTANCE h_instacne) noexcept { app_instacne_ = h_instacne; }
//	static void set_cmd_show(int cmd_show) noexcept { cmd_show_ = cmd_show; }
//
//	[[nodiscard]] INT get_template_id() const noexcept { return template_id_; }
//	[[nodiscard]] HWND get_current_dlg_handle() const noexcept { return h_current_dlg_; }
//	[[nodiscard]] HWND get_parent_dlg_handle() const noexcept { return h_parent_dlg_; }
//	[[nodiscard]] DialogEvent get_current_event() const noexcept { return current_event_; }
//
//	virtual LRESULT dialog_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) = 0;
//};
//
//export template <typename DialogType>
//class DialogPtr {
//private:
//	std::weak_ptr<DialogType> dialog_ptr_;
//public:
//	explicit DialogPtr(std::weak_ptr<DialogType> dialog_ptr) noexcept : dialog_ptr_(dialog_ptr) {}
//
//	[[nodiscard]] DialogType* get() const noexcept { return dialog_ptr_.lock().get(); }
//
//	[[nodiscard]] DialogType* operator->() const noexcept { return get(); }
//
//	[[nodiscard]] bool is_valid() const noexcept { return !dialog_ptr_.expired(); }
//
//	explicit operator bool() const noexcept { return is_valid(); }
//
//	using dialog_type = DialogType;
//};

#define HAS_MEMBER_FUNCTION(function)                   \
template <typename T, typename = std::void_t<>>                     \
struct has_##function : std::false_type {};                        \
                                                                    \
template <typename T>                                               \
struct has_##function<T, std::void_t<decltype(std::declval<T>().function())>> : std::true_type{};   \
                                                                    \
template <typename T>                                               \
concept has_##function = has_##function<T>::value

HAS_MEMBER_FUNCTION(init_dialog_impl);
HAS_MEMBER_FUNCTION(show_dialog_impl);
HAS_MEMBER_FUNCTION(hide_dialog_impl);
HAS_MEMBER_FUNCTION(close_dialog_impl);

export template <typename Derived>
class DialogEX {
protected:
    inline static HINSTANCE app_instance_ = nullptr;
    inline static int cmd_show_ = SW_SHOWNORMAL;

    HWND h_current_dlg_;
    HWND h_parent_dlg_;
    DialogEvent current_event_ = DialogEvent::None;

public:

    explicit DialogEX(HWND h_parent = nullptr) noexcept
        : h_parent_dlg_(h_parent),
        h_current_dlg_(nullptr) {

        static_assert(has_show_dialog_impl<Derived>, "Derived must implement show_dialog_impl()");
        static_assert(has_close_dialog_impl<Derived>, "Derived must implement close_dialog_impl()");
    }

    ~DialogEX() noexcept = default;

    void init_dialog() {
        if constexpr (has_init_dialog_impl<Derived>) {
            static_cast<Derived*>(this)->init_dialog_impl();
        }
    }

    void show_dialog() {
        static_cast<Derived*>(this)->show_dialog_impl();
    }

    void hide_dialog() {
        if constexpr (has_hide_dialog_impl) {
            static_cast<Derived*>(this)->hide_dialog_impl();
        }
    }

    void close_dialog() {
        static_cast<Derived*>(this)->close_dialog_impl();
    }

    void set_current_dlg_handle(HWND h_dlg) noexcept { h_current_dlg_ = h_dlg; }
    void set_current_event(DialogEvent new_event) noexcept { current_event_ = new_event; }

    static void set_app_instance(HINSTANCE h_instance) noexcept { app_instance_ = h_instance; }
    static void set_cmd_show(int cmd_show) noexcept { cmd_show_ = cmd_show; }

    [[nodiscard]] HWND get_current_dlg_handle() const noexcept { return h_current_dlg_; }
    [[nodiscard]] HWND get_parent_dlg_handle() const noexcept { return h_parent_dlg_; }
    [[nodiscard]] DialogEvent get_current_event() const noexcept { return current_event_; }

    LRESULT dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        return static_cast<Derived*>(this)->dialog_proc_impl(h_dlg, message, w_param, l_param);
    }
};

export template <typename DialogType>
class DialogPtr {
private:
	std::weak_ptr<DialogType> dialog_ptr_;
public:
	explicit DialogPtr(std::weak_ptr<DialogType> dialog_ptr) noexcept : dialog_ptr_(dialog_ptr) {}

	[[nodiscard]] DialogType* get() const noexcept { return dialog_ptr_.lock().get(); }

	[[nodiscard]] DialogType* operator->() const noexcept { return get(); }

	[[nodiscard]] bool is_valid() const noexcept { return !dialog_ptr_.expired(); }

	explicit operator bool() const noexcept { return is_valid(); }
};
