module;

#include <windows.h>
//#include <assert.h>
//#include <map>
//#include <memory>	
//#include "spdlog/spdlog.h"
//
export module DialogManager;

import DialogEX;
import STL;
import MainDlg;
//import Logger;
//
//
//export class DialogManager {
//private:
//	DialogManager() = default;
//	~DialogManager() = default;
//
//	DialogManager(const DialogManager&) = delete;
//	DialogManager& operator=(const DialogManager&) = delete;
//	DialogManager(DialogManager&& other) = delete;
//	DialogManager& operator=(DialogManager&& other) = delete;
//
//public:
//	static DialogManager& instance();
//
//	template <typename DialogType, typename... Args>
//	[[nodiscard]] DialogPtr<DialogType> register_dialog(Args&&... args) {
//		auto dialog = std::make_shared<DialogType>(std::forward<Args>(args)...);
//		INT dialog_id = dialog->get_template_id();
//
//		info("Registering dialog with ID: {} and type: {}", dialog_id, typeid(DialogType).name());
//		create_and_show_dialog(dialog_id, dialog);
//
//		id_to_dialogs_[dialog_id] = dialog;
//		HWND dialog_handle = dialog->get_current_dlg_handle();
//		handle_to_id_[dialog_handle] = dialog_id;
//
//		change_state(dialog->get_template_id(), DialogEvent::Init);
//
//		info("Dialog with ID: {} registered successfully", dialog_id);
//		return DialogPtr<DialogType>(dialog);
//	}
//
//	//[[nodiscard]] DialogPtr<DialogEX> find_dialog(INT dialog_id);
//
//	//[[nodiscard]] DialogPtr<DialogEX> find_dialog(HWND dialog_handle);
//
//	[[nodiscard]] bool unregister_dialog(INT dialog_id);
//
//	void change_state(INT dialog_id, DialogEvent new_event);
//
//private:
//	std::unordered_map<INT, std::shared_ptr<DialogEX>> id_to_dialogs_;
//	std::unordered_map<HWND, INT> handle_to_id_;
//
//	void handle_dialog_evnet(DialogEX* dialog, DialogEvent new_event);
//	[[nodiscard]] bool can_transition_to_event(DialogEX* dialog, DialogEvent new_event);
//
//	static INT_PTR CALLBACK static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param);
//
//	template <typename DialogType>
//		requires std::derived_from<DialogType, DialogEX>
//	void create_and_show_dialog(const INT dialog_id, std::shared_ptr<DialogType> dialog_ptr) {
//		HWND hwnd = CreateDialogParam(
//			GetModuleHandle(nullptr),
//			MAKEINTRESOURCE(dialog_id),
//			dialog_ptr->get_parent_dlg_handle(),
//			static_dialog_proc,
//			reinterpret_cast<LPARAM>(dialog_ptr.get())
//		);
//	}
//};
//
//export inline auto& dialog_mgr = DialogManager::instance();
//
//export class DialogManager {
//private:
//	DialogManager() = default;
//	~DialogManager() = default;
//
//	DialogManager(const DialogManager&) = delete;
//	DialogManager& operator=(const DialogManager&) = delete;
//	DialogManager(DialogManager&& other) = delete;
//	DialogManager& operator=(DialogManager&& other) = delete;
//
//public:
//	static DialogManager& instance();
//
//	template <typename DialogType, typename... Args>
//	[[nodiscard]] DialogPtr<DialogType> register_dialog(Args&&... args) {
//		auto dialog = std::make_shared<DialogType>(std::forward<Args>(args)...);
//		INT dialog_id = dialog->get_template_id();
//
//		info("Registering dialog with ID: {} and type: {}", dialog_id, typeid(DialogType).name());
//		create_and_show_dialog(dialog_id, dialog);
//
//		id_to_dialogs_[dialog_id] = std::static_pointer_cast<DialogEX<void>>(dialog);
//		HWND dialog_handle = dialog->get_current_dlg_handle();
//		handle_to_id_[dialog_handle] = dialog_id;
//
//		change_state(dialog->get_template_id(), DialogEvent::Init);
//
//		info("Dialog with ID: {} registered successfully", dialog_id);
//		return DialogPtr<DialogType>(dialog);
//	}
//
//	//[[nodiscard]] DialogPtr<DialogEX> find_dialog(INT dialog_id);
//
//	//[[nodiscard]] DialogPtr<DialogEX> find_dialog(HWND dialog_handle);
//
//	[[nodiscard]] bool unregister_dialog(INT dialog_id);
//
//	void change_state(INT dialog_id, DialogEvent new_event);
//
//private:
//	std::unordered_map<INT, std::shared_ptr<DialogEX<void>>> id_to_dialogs_;
//	std::unordered_map<HWND, INT> handle_to_id_;
//
//	void handle_dialog_event(DialogEX<void>* dialog, DialogEvent new_event);
//
//	bool can_transition_to_event(DialogEX<void>* dialog, DialogEvent new_event);
//
//	static INT_PTR CALLBACK static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param);
//
//	template <typename DialogType>
//	void create_and_show_dialog(const INT dialog_id, std::shared_ptr<DialogType> dialog_ptr) {
//		HWND hwnd = CreateDialogParam(
//			GetModuleHandle(nullptr),
//			MAKEINTRESOURCE(dialog_id),
//			dialog_ptr->get_parent_dlg_handle(),
//			static_dialog_proc,
//			reinterpret_cast<LPARAM>(dialog_ptr.get())
//		);
//	}
//};
//
//export inline auto& dialog_mgr = DialogManager::instance();

//using DialogVariant = std::variant <
//	std::shared_ptr<MainDlg>
//>;

export class DialogManager {
private:
	DialogManager() = default;
	~DialogManager() = default;

	DialogManager(const DialogManager&) = delete;
	DialogManager& operator=(const DialogManager&) = delete;
	DialogManager(DialogManager&& other) = delete;
	DialogManager& operator=(DialogManager&& other) = delete;

	template <typename DialogType>
	std::optional<std::shared_ptr<DialogType>> create_dialog_impl(HWND h_parent) {
		auto dialog_ptr = std::make_shared<DialogType>(h_parent);
		HWND hwnd = CreateDialogParam(
			GetModuleHandle(nullptr),
			MAKEINTRESOURCE(DialogType::id),
			h_parent,
			static_dialog_proc<DialogType>,
			reinterpret_cast<LPARAM>(dialog_ptr.get())
		);
		if (hwnd) {
			dialog_ptr->set_current_dlg_handle(hwnd);
			return dialog_ptr;
		}
		else {
			return std::nullopt;
		}
	}

	template <typename DialogType>
	static INT_PTR CALLBACK static_dialog_proc(HWND h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
		DialogType* this_dlg = nullptr;

		if (message == WM_INITDIALOG) {
			this_dlg = reinterpret_cast<DialogType*>(l_param);
			SetWindowLongPtr(h_dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_dlg));
			this_dlg->set_current_dlg_handle(h_dlg);
			return true;
		}

		this_dlg = reinterpret_cast<DialogType*>(GetWindowLongPtr(h_dlg, GWLP_USERDATA));

		if (!this_dlg) {
			return FALSE;
		}

		LRESULT result = this_dlg->dialog_proc(h_dlg, message, w_param, l_param);

		return result;
	}

public:
	static DialogManager& instance();

	void init();

	template <typename DialogType>
	void set_dialog_function(INT dialog_id) {
		factory_map_[dialog_id] = [this](HWND h_parent) -> void {
			this->create_dialog_impl<DialogType>(h_parent);
			};
	}

	template <typename DialogType>
		requires std::is_base_of_v<DialogEX<DialogType>, DialogType>
	[[nodiscard]] std::optional<DialogPtr<DialogType>> register_dialog(HWND h_parent) {
		auto existing_result = find_dialog<DialogType>(DialogType::id);
		if (existing_result.has_value()) {
			return existing_result;
		}

		auto it = factory_map_.find(DialogType::id);
		if (it == factory_map_.end()) {
			return std::nullopt;
		}
		it->second(h_parent);

		//auto dialog = find_dialog<DialogType>(DialogType::id);
		//change_state(dialog.value(), DialogEvent::Init);
		change_state(DialogType::id, DialogEvent::Init);
		return find_dialog<DialogType>(DialogType::id);
	}

	template <typename DialogType>
	[[nodiscard]] std::optional<DialogPtr<DialogType>> find_dialog(INT dialog_id) {
		auto ptr = std::get_if<std::shared_ptr<DialogType>>(&dialogs_);
		if (ptr) {
			return DialogPtr<DialogType>(*ptr);
		}
		else {
			return std::nullopt;
		}
	}

	template <typename DialogType>
	void handle_dialog_evnet(DialogPtr<DialogType> dialog, DialogEvent new_event) {
		switch (new_event) {
		case DialogEvent::Init:
			dialog->set_current_event(new_event);
			dialog->init_dialog();
			break;
		case DialogEvent::Show:
			dialog->set_current_event(new_event);
			dialog->show_dialog();
			break;
		case DialogEvent::Hide:
			dialog->set_current_event(new_event);
			break;
		case DialogEvent::Close:
			dialog->set_current_event(new_event);
			dialog->close_dialog();
			break;
		default:
			return;
		}
	}

	template <typename DialogType>
	[[nodiscard]] bool can_transition_to_event(DialogPtr<DialogType> dialog, DialogEvent new_event) {
		DialogEvent current_event = dialog->get_current_event();
		switch (current_event) {
		case DialogEvent::None:
			return new_event == DialogEvent::Init;
		case DialogEvent::Init:
			return new_event == DialogEvent::Show || new_event == DialogEvent::Hide;
		case DialogEvent::Show:
			return new_event == DialogEvent::Hide || new_event == DialogEvent::Close;
		case DialogEvent::Hide:
			return new_event == DialogEvent::Show || new_event == DialogEvent::Close;
		case DialogEvent::Close:
			return false;
		default:
			return false;
		}
	}

	//template <typename DialogType>
	//void change_state(DialogPtr<DialogType> dialog, DialogEvent new_event) {
	//	if (dialog->get_current_event() == new_event) {
	//		return;
	//	}

	//	if (can_transition_to_event(dialog, new_event)) {
	//		handle_dialog_evnet(dialog, new_event);
	//	}
	//}

	template <typename DialogType>
	void change_state(int dialog_id, DialogEvent new_event) {
		auto dialog_ptr = std::get_if<std::shared_ptr<DialogType>>(&dialogs_);

		if (dialog_ptr->get_current_event() == new_event) {
			return;
		}

		if (can_transition_to_event(dialog_ptr, new_event)) {
			handle_dialog_evnet(dialog_ptr, new_event);
		}
	}

private:

	std::variant<
		std::shared_ptr<MainDlg>
	> dialogs_;



	std::unordered_map<INT, std::function<void(HWND)>> factory_map_;
};

export inline auto& dialog_mgr = DialogManager::instance();