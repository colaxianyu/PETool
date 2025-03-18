module;

#include <windows.h>

export module DialogManager;

import DialogEX;
import STL;

using std::unique_ptr;
using std::stack;
using std::make_unique;
using std::forward;

namespace petools {

	export class DialogManager {
	public:
		static DialogManager& instance() {
			static DialogManager instance;
			return instance;
		}

		template <typename T, typename... Args>
		void open_dialog(Args&&... args) {
			auto dialog = make_unique<T>(forward<Args>(args)...);
			if (!dialogs_.empty()) {
				disable_current_dialog();
			}

			::CreateDialogParam(
				dialog->get_instance(),
				MAKEINTRESOURCE(dialog->get_template_id()),
				dialog->get_parent_hwnd(),
				DialogEX::static_dialog_proc,
				reinterpret_cast<LPARAM>(dialog.get())
			);

			dialog->handle_init();
			dialogs_.push(move(dialog));
			dialogs_.top()->handle_show();
		}

		void close_dialog() {
			if (dialogs_.empty()) {
				return;
			}
			dialogs_.top()->handle_close();
			dialogs_.pop();
			if (!dialogs_.empty()) {
				activate_current_dialog();
			}
		}

	private:
		DialogManager() = default;
		~DialogManager() = default;

		DialogManager(const DialogManager&) = delete;
		DialogManager& operator=(const DialogManager&) = delete;
		DialogManager(DialogManager&&) = delete;
		DialogManager& operator=(DialogManager&&) = delete;

		void disable_current_dialog() {
			if (auto hwnd = dialogs_.top()->get_current_hwnd()) {
				::EnableWindow(hwnd, FALSE);
			}
		}

		void activate_current_dialog() {
			if (auto hwnd = dialogs_.top()->get_current_hwnd()) {
				::EnableWindow(hwnd, TRUE);
				::SetForegroundWindow(hwnd);
			}
		}

		stack<unique_ptr<DialogEX>> dialogs_;
	};

	export inline auto& dialog_mgr = DialogManager::instance();

} //namespace petools