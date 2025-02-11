module;

#include <windows.h>
#include "resource.h"

export module MainDlg;

import STL;
import DialogEX;
import ListControl;

//import ProtectorDlg;
//import PeEditDlg;

struct IconDeleter {
	void operator()(void* h_icon) const {
		if (h_icon) {
			MessageBox(nullptr, L"IconDeleter", L"IconDeleter", MB_OK);
			DestroyIcon(static_cast<HICON>(h_icon));
		}
	}
};

//export class MainDlg : public DialogEX<MainDlg> {
//public:
//	explicit MainDlg(INT id_template, HWND h_parent = nullptr) noexcept;
//	virtual ~MainDlg() noexcept override;
//
//	void init_dialog() override;
//	void show_dialog() override;
//	void close_dialog() override;
//
	//void init_process_list();
	//void init_module_list();
//
//	void plant_process_item();
//	void plant_process_column();
//	void plant_module_column();
//	void plant_module_item();
//
//	//void create_about_dlg();
//	//void create_pe_edit_dlg();
//	//void create_protector_dlg();
//
//	std::optional<DWORD> get_pid(INT row_index) noexcept;
//private:
//	std::unique_ptr<void, IconDeleter> h_icon_{nullptr, IconDeleter()};
//
//	std::unique_ptr<ListCtrl> process_list_ = nullptr;
//	std::unique_ptr<ListCtrl> module_list_ = nullptr;
//
//	LRESULT dialog_proc(HWND h_about, UINT message, WPARAM w_param, LPARAM l_param) override;
//};

export class MainDlg : public DialogEX<MainDlg> {
public:
    static constexpr int id = IDD_DIALOG_MAIN;

    explicit MainDlg(HWND h_parent = nullptr) noexcept
        : DialogEX(h_parent)
    {
        HICON h_icon = LoadIcon(this->app_instance_, MAKEINTRESOURCE(IDI_ICON_MAINICO));
        if (h_icon) {
            h_icon_.reset(h_icon);
        }
    }

    ~MainDlg() noexcept = default;

    void init_dialog_impl();
    void show_dialog_impl();
    void close_dialog_impl();

    void init_process_list();
    void init_module_list();

    void plant_process_item();
    void plant_process_column();
    void plant_module_column();
    void plant_module_item();

    std::optional<DWORD> get_pid(INT row_index) noexcept;

    LRESULT dialog_proc_impl(HWND h_about, UINT message, WPARAM w_param, LPARAM l_param);
private:
    std::unique_ptr<ListCtrl> process_list_ = nullptr;
    std::unique_ptr<ListCtrl> module_list_ = nullptr;
    std::unique_ptr<void, IconDeleter> h_icon_{ nullptr, IconDeleter() };

};