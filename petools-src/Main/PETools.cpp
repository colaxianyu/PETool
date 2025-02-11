#include <windows.h>
#include <iostream>
#include <variant>
#include "resource.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

import PEApplication;
import DialogEX;
import DialogManager;
import MainDlg;
//import STL;
import Logger;

int APIENTRY wWinMain(_In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE h_prev_instance,
    _In_ LPWSTR    lp_cmd_line,
    _In_ int       n_cmd_show)
{
	Logger::instance().init();
    PEApplication::instance().init_APP(h_instance, n_cmd_show);
    dialog_mgr.init();
    auto main_dlg_ptr = dialog_mgr.register_dialog<MainDlg>(nullptr).value();

    if (main_dlg_ptr) {
        dialog_mgr.change_state<MainDlg>(main_dlg_ptr.get()->id, DialogEvent::Show);
    }
   

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }
}



//template <typename Derived>
//class Dialog {
//public:
//    void set_current_dlg_handle(HWND h_dlg) { h_dlg_ = h_dlg; }
//    HWND get_handle() const { return h_dlg_; }
//
//    INT_PTR dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        return static_cast<Derived*>(this)->handle_message(h_dlg, message, w_param, l_param);
//    }
//
//protected:
//    HWND h_dlg_ = nullptr;
//
//public:
//    // 静态对话过程函数
//    static INT_PTR CALLBACK static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        Derived* dlg = nullptr;
//        if (message == WM_INITDIALOG) {
//            dlg = reinterpret_cast<Derived*>(l_param);
//            SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dlg));
//            dlg->set_current_dlg_handle(h_dlg);
//            return dlg->dialog_proc(h_dlg, message, w_param, l_param);
//        }
//        else {
//            dlg = reinterpret_cast<Derived*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
//            if (dlg) {
//                return dlg->dialog_proc(h_dlg, message, w_param, l_param);
//            }
//        }
//        return FALSE;
//    }
//};
//
//class MainDlg;
//class PEHeaderDlg;
//class AboutDlg;
//
//// 定义 DialogVariant
//using DialogVariant = std::variant<
//    std::unique_ptr<MainDlg>,
//    std::unique_ptr<PEHeaderDlg>,
//    std::unique_ptr<AboutDlg>
//>;
//
//// DialogManager 类
//class DialogManager {
//public:
//    // 获取单例实例
//    static DialogManager& get_instance() {
//        static DialogManager instance;
//        return instance;
//    }
//
//    // 禁止拷贝和赋值
//    DialogManager(const DialogManager&) = delete;
//    DialogManager& operator=(const DialogManager&) = delete;
//    DialogManager(DialogManager&&) = delete;
//    DialogManager& operator=(DialogManager&&) = delete;
//
//    // 注册对话框类型与其工厂函数
//    template <typename DialogType>
//    void register_dialog(int dialog_id) {
//        factory_map_[dialog_id] = [this](HWND parent) -> void {
//            create_dialog_instance<DialogType>(parent);
//            };
//    }
//
//    // 创建并显示指定 Dialog
//    template <typename DialogType>
//    void create_dialog(HWND parent) {
//        static_assert(std::is_base_of<Dialog<DialogType>, DialogType>::value, "DialogType must derive from Dialog<DialogType>");
//
//        // 检查是否已经存在
//        if (find_dialog<DialogType>(DialogType::IDD)) {
//            std::cout << "Dialog with ID " << DialogType::IDD << " already exists.\n";
//            return;
//        }
//
//        // 调用工厂函数
//        auto it = factory_map_.find(DialogType::IDD);
//        if (it != factory_map_.end()) {
//            it->second(parent);
//        }
//        else {
//            std::cerr << "No factory registered for dialog ID " << DialogType::IDD << "\n";
//        }
//    }
//
//    // 查找 Dialog 根据 ID
//    template <typename DialogType>
//    DialogType* find_dialog(int dialog_id) const {
//        auto it = dialog_maps_.find(dialog_id);
//        if (it != dialog_maps_.end()) {
//            if (auto ptr = std::get_if<std::unique_ptr<DialogType>>(&it->second)) {
//                return ptr->get();
//            }
//        }
//        return nullptr;
//    }
//
//private:
//    DialogManager() = default;
//    ~DialogManager() = default;
//
//    // 创建对话框实例并存储
//    template <typename DialogType>
//    void create_dialog_instance(HWND parent) {
//        auto dialog = std::make_unique<DialogType>();
//        HWND h_dlg = CreateDialogParam(
//            GetModuleHandle(nullptr),
//            MAKEINTRESOURCE(DialogType::IDD),
//            parent,
//            &DialogType::static_dialog_proc,
//            reinterpret_cast<LPARAM>(dialog.get())
//        );
//
//        if (h_dlg) {
//            dialog->set_current_dlg_handle(h_dlg);
//            dialog_maps_[DialogType::IDD] = std::move(dialog);
//            std::cout << "Created Dialog with ID: " << DialogType::IDD << "\n";
//        }
//        else {
//            // 处理错误
//            std::cerr << "Failed to create dialog with ID " << DialogType::IDD << "\n";
//        }
//    }
//
//    // 存储 ID 到对话框实例的映射，使用 std::variant
//    std::unordered_map<int, DialogVariant> dialog_maps_;
//
//    // 存储 ID 到工厂函数的映射
//    std::unordered_map<int, std::function<void(HWND)>> factory_map_;
//};
//
//// 示例对话框类 AboutDlg
//class AboutDlg : public Dialog<AboutDlg> {
//public:
//    static constexpr int IDD = 403; // 对话框资源 ID
//
//    INT_PTR handle_message(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        switch (message) {
//        case WM_INITDIALOG:
//            std::cout << "AboutDlg: WM_INITDIALOG\n";
//            return TRUE;
//        case WM_COMMAND:
//            if (LOWORD(w_param) == IDOK) { // 假设 IDOK 为按钮 ID
//                std::cout << "AboutDlg: OK Button Clicked\n";
//                // 关闭 AboutDlg
//                EndDialog(h_dlg, 0);
//            }
//            return TRUE;
//        case WM_CLOSE:
//            std::cout << "AboutDlg: WM_CLOSE\n";
//            EndDialog(h_dlg, 0);
//            return TRUE;
//        default:
//            return FALSE;
//        }
//    }
//
//    AboutDlg() {
//        // 初始化代码（如果有）
//    }
//};
//
//// 示例对话框类 PEHeaderDlg
//class PEHeaderDlg : public Dialog<PEHeaderDlg> {
//public:
//    static constexpr int IDD = 402; // 对话框资源 ID
//
//    INT_PTR handle_message(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        switch (message) {
//        case WM_INITDIALOG:
//            std::cout << "PEHeaderDlg: WM_INITDIALOG\n";
//            return TRUE;
//        case WM_COMMAND:
//            if (LOWORD(w_param) == IDOK) { // 假设 IDOK 为按钮 ID
//                std::cout << "PEHeaderDlg: OK Button Clicked\n";
//                // 打开 AboutDlg
//                DialogManager::get_instance().create_dialog<AboutDlg>(h_dlg);
//            }
//            return TRUE;
//        case WM_CLOSE:
//            std::cout << "PEHeaderDlg: WM_CLOSE\n";
//            EndDialog(h_dlg, 0);
//            return TRUE;
//        default:
//            return FALSE;
//        }
//    }
//
//    PEHeaderDlg() {
//        // 初始化代码（如果有）
//    }
//};
//
//// 示例对话框类 MainDlg
//class MainDlg : public Dialog<MainDlg> {
//public:
//    static constexpr int IDD = 401; // 对话框资源 ID
//
//    INT_PTR handle_message(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        switch (message) {
//        case WM_INITDIALOG:
//            std::cout << "MainDlg: WM_INITDIALOG\n";
//            return TRUE;
//        case WM_COMMAND:
//            if (LOWORD(w_param) == IDOK) { // 假设 IDOK 为按钮 ID
//                std::cout << "MainDlg: OK Button Clicked\n";
//                // 打开 PEHeaderDlg
//                DialogManager::get_instance().create_dialog<PEHeaderDlg>(h_dlg);
//            }
//            return TRUE;
//        case WM_CLOSE:
//            std::cout << "MainDlg: WM_CLOSE\n";
//            EndDialog(h_dlg, 0);
//            return TRUE;
//        default:
//            return FALSE;
//        }
//    }
//
//    MainDlg() {
//        // 初始化代码（如果有）
//    }
//};
//
//// 主窗口过程
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//    switch (uMsg) {
//    case WM_CREATE:
//        // 注册对话框类型
//    {
//        std::cout << "Initializing DialogManager with std::variant\n";
//        DialogManager::get_instance().register_dialog<MainDlg>(301);
//        DialogManager::get_instance().register_dialog<PEHeaderDlg>(302);
//        DialogManager::get_instance().register_dialog<AboutDlg>(303);
//
//        // 创建并显示 MainDlg
//        DialogManager::get_instance().create_dialog<MainDlg>(hwnd);
//    }
//    return 0;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    default:
//        return DefWindowProc(hwnd, uMsg, wParam, lParam);
//    }
//}
//
//// 注册窗口类并创建主窗口
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
//    // 注册窗口类
//    const wchar_t CLASS_NAME[] = L"MainWindowClass";
//
//    WNDCLASS wc = {};
//    wc.lpfnWndProc = WindowProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = (LPCWSTR)CLASS_NAME;
//
//    if (!RegisterClass(&wc)) {
//        std::cerr << "Failed to register window class.\n";
//        return 0;
//    }
//
//    // 创建主窗口
//    HWND hwnd = CreateWindowEx(
//        0,
//        (LPCWSTR)CLASS_NAME,
//        L"Main Window - std::variant-Based DialogManager",
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
//        nullptr,
//        nullptr,
//        hInstance,
//        nullptr
//    );
//
//    if (hwnd == nullptr) {
//        std::cerr << "Failed to create main window.\n";
//        return 0;
//    }
//
//    ShowWindow(hwnd, nCmdShow);
//
//    // 消息循环
//    MSG msg = {};
//    while (GetMessage(&msg, nullptr, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return 0;
//}