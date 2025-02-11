//module;
//
//#include <windows.h>
//#include "spdlog/spdlog.h"
//
//module DialogManager;
//
//import Logger;
//
//DialogManager& DialogManager::instance() {
//	static DialogManager instance;
//	return instance;
//}
//
////DialogPtr<DialogEX> DialogManager::find_dialog(INT dialog_id) {
////	auto it = id_to_dialogs_.find(dialog_id);
////	return it != id_to_dialogs_.end() ? DialogPtr<DialogEX>(it->second) : DialogPtr<DialogEX>(std::weak_ptr<DialogEX>());
////}
////
////DialogPtr<DialogEX> DialogManager::find_dialog(HWND dialog_handle) {
////	auto it = handle_to_id_.find(dialog_handle);
////	if (it == handle_to_id_.end()) {
////		return DialogPtr<DialogEX>(std::weak_ptr<DialogEX>());
////	}
////
////	return find_dialog(it->second);
////}
//
//bool DialogManager::unregister_dialog(INT dialog_id) {
//	auto it = id_to_dialogs_.find(dialog_id);
//	if (it != id_to_dialogs_.end()) {
//		auto dialog_ptr = it->second;
//		HWND dialog_handle = dialog_ptr->get_current_dlg_handle();
//
//		info("Unregistering dialog with ID: {}", dialog_id);
//
//		handle_to_id_.erase(it->second->get_current_dlg_handle());
//		id_to_dialogs_.erase(it);
//
//		if (IsWindow(dialog_handle)) {
//			DestroyWindow(dialog_handle);
//			info("Successfully destroyed dialog window with ID: {}", dialog_id);
//		}
//
//		return true;
//	}
//	return false;
//}
//
//void DialogManager::handle_dialog_event(DialogEX<void>* dialog, DialogEvent new_event) {
//	switch (new_event) {
//	case DialogEvent::Init:
//		dialog->set_current_event(new_event);
//		dialog->init_dialog();
//		break;
//	case DialogEvent::Show:
//		dialog->set_current_event(new_event);
//		dialog->show_dialog();
//		break;
//	case DialogEvent::Hide:
//		dialog->set_current_event(new_event);
//		break;
//	case DialogEvent::Close:
//		dialog->set_current_event(new_event);
//		dialog->close_dialog();
//		break;
//	default:
//		return;
//	}
//}
//
//bool DialogManager::can_transition_to_event(DialogEX<void>* dialog, DialogEvent new_event) {
//	DialogEvent current_event = dialog->get_current_event();
//	switch (current_event) {
//	case DialogEvent::None:
//		return new_event == DialogEvent::Init;
//	case DialogEvent::Init:
//		return new_event == DialogEvent::Show || new_event == DialogEvent::Hide;
//	case DialogEvent::Show:
//		return new_event == DialogEvent::Hide || new_event == DialogEvent::Close;
//	case DialogEvent::Hide:
//		return new_event == DialogEvent::Show || new_event == DialogEvent::Close;
//	case DialogEvent::Close:
//		return false;
//	default:
//		return false;
//	}
//}
//
////void DialogManager::handle_dialog_event(DialogEX<void>* dialog, DialogEvent new_event) {
////	// 使用 CRTP 调用具体对话框类的处理逻辑
////	if (new_event == DialogEvent::Init) {
////		dialog->change_state(DialogEvent::Init);
////	}
////	else if (new_event == DialogEvent::Show) {
////		dialog->change_state(DialogEvent::Show);
////	}
////	else if (new_event == DialogEvent::Close) {
////		dialog->change_state(DialogEvent::Close);
////	}
////	else {
////		int a = 0;
////	}
////}
////
////bool DialogManager::can_transition_to_event(DialogEX<void>* dialog, DialogEvent new_event) {
////	// 静态多态方式调用特定对话框类型的状态检查方法
////	// 这里假设每个对话框类实现了类似的状态检查方法，你可以在每个具体对话框类中实现
////	// 返回是否可以转换到该事件
////	if (new_event == DialogEvent::Show) {
////		// 这里只是一个示例，你可以根据需要检查对话框当前状态
////		return true; // 假设所有对话框都能从当前状态转换到 Show
////	}
////	else if (new_event == DialogEvent::Close) {
////		return true; // 假设所有对话框都能从当前状态转换到 Close
////	}
////	return false;
////}
//
////void DialogManager::change_state(INT dialog_id, DialogEvent new_event) {
////	auto it = id_to_dialogs_.find(dialog_id);
////	if (it == id_to_dialogs_.end() || it->second->get_current_event() == new_event) {
////		return;
////	}
////
////	if (can_transition_to_event(it->second.get(), new_event)) {
////		// 使用 CRTP 调用派生类的事件处理逻辑
////		it->second->change_state(new_event);  // 这里会调用具体对话框类的 `change_state`
////	}
////}
//
//INT_PTR DialogManager::static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//	auto* dialog_ptr = reinterpret_cast<DialogEX<void>*>(l_param);
//
//	if (message == WM_INITDIALOG) {
//		SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dialog_ptr));
//		dialog_ptr->set_current_dlg_handle(h_dlg);
//		return TRUE;
//	}
//
//	dialog_ptr = reinterpret_cast<DialogEX<void>*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
//	if (!dialog_ptr) {
//		return FALSE;
//	}
//
//	return dialog_ptr->dialog_proc(h_dlg, message, w_param, l_param);
//}
//
//void DialogManager::change_state(INT dialog_id, DialogEvent new_event) {
//	auto it = id_to_dialogs_.find(dialog_id);
//
//	if (it == id_to_dialogs_.end() || it->second->get_current_event() == new_event) {
//		return;
//	}
//
//	if (can_transition_to_event(it->second.get(), new_event)) {
//		handle_dialog_event(it->second.get(), new_event);
//	}
//}
////
////INT_PTR DialogManager::static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
////	DialogEX* this_dlg = nullptr;
////
////	if (message == WM_INITDIALOG) {
////		this_dlg = reinterpret_cast<DialogEX*>(l_param);
////		SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_dlg));
////		this_dlg->set_current_dlg_handle(h_dlg);
////		return true;
////	}
////
////	this_dlg = reinterpret_cast<DialogEX*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
////
////	if (!this_dlg) {
////		return FALSE;
////	}
////
////	LRESULT result = this_dlg->dialog_proc(h_dlg, message, w_param, l_param);
////
////	return result;
////}

module;
#include <windows.h>
module DialogManager;

import STL;
import DialogEX;
import MainDlg;

DialogManager& DialogManager::instance() {
	static DialogManager instance;
	return instance;
}

void DialogManager::init() {
	set_dialog_function<MainDlg>(MainDlg::id);
}

//template <typename DialogType>
//void DialogManager::create_dialog_impl(HWND h_parent) {
//	auto dialog_ptr = std::make_shared<DialogType>(h_parent);
//	HWND hwnd = CreateDialogParam(
//		GetModuleHandle(nullptr),
//		MAKEINTRESOURCE(DialogType::id),
//		h_parent,
//		static_dialog_proc<DialogType>,
//		reinterpret_cast<LPARAM>(dialog_ptr.get())
//	);
//	if (hwnd) {
//		dialog_ptr->set_current_dlg_handle(hwnd);
//		id_to_dialog_[DialogType::id] = dialog_ptr;
//	}
//}

//template <typename DialogType>
//void DialogManager::set_dialog_function(INT dialog_id) {
//	factory_map_[dialog_id] = [this](HWND h_parent) -> void {
//		this->create_dialog_impl<DialogType>(h_parent);
//		};
//}

//template <typename DialogType>
//	requires std::is_base_of_v<DialogEX<DialogType>, DialogType>
//std::optional<DialogPtr<DialogType>> DialogManager::register_dialog(HWND h_parent) {
//	auto existing_result = find_dialog<DialogType>(DialogType::id);
//	if (existing_result.has_value()) {
//		return existing_result;
//	}
//
//	auto create_function = factory_map_[DialogType::id];
//	if (create_function == factory_map_.end()) {
//		return std::nullopt;
//	}
//
//	create_function(h_parent);
//	return find_dialog<DialogType>(DialogType::id);
//}

//template <typename DialogType>
//std::optional<DialogPtr<DialogType>> DialogManager::find_dialog(INT dialog_id) {
//	auto it = id_to_dialog_.find(dialog_id);
//	if (it != id_to_dialog_.end()) {
//		return DialogPtr<DialogType>(it->second);
//	}
//	else {
//		return std::nullopt;
//	}
//}

//template <typename DialogType>
//INT_PTR DialogManager::static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//	DialogType* this_dlg = nullptr;
//
//	if (message == WM_INITDIALOG) {
//		this_dlg = reinterpret_cast<DialogType*>(l_param);
//		SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_dlg));
//		this_dlg->set_current_dlg_handle(h_dlg);
//		return true;
//	}
//
//	this_dlg = reinterpret_cast<DialogType*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));
//
//	if (!this_dlg) {
//		return FALSE;
//	}
//
//	LRESULT result = this_dlg->dialog_proc(h_dlg, message, w_param, l_param);
//
//	return result;
//}