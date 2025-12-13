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
		static DialogManager& Instance() {
			static DialogManager instance;
			return instance;
		}

		template <typename Dlg, typename... Args>
		[[nodiscard]] Dlg* OpenDialog(Args&&... args) {
			static_assert(std::is_base_of_v<DialogEX, Dlg>, "Dlg must derive from DialogEX");

			auto dialog = make_unique<Dlg>(forward<Args>(args)...);
			if (!dialog) {
				return nullptr;
			}

			if (!dialogs_.empty()) {
				disable_current_dialog();
			}

			HWND hdlg = ::CreateDialogParam(
				dialog->get_instance(),
				MAKEINTRESOURCE(dialog->get_template_id()),
				dialog->get_parent_hwnd(),
				DialogEX::static_dialog_proc,
				reinterpret_cast<LPARAM>(dialog.get())
			);

			if (!hdlg) {
				::MessageBoxW(
					dialog->get_parent_hwnd(),
					L"创建对话框失败。",
					L"错误",
					MB_ICONERROR | MB_OK
				);
				return nullptr;
			}

			dialogs_.push(move(dialog));
			return static_cast<Dlg*>(dialogs_.top().get());
		}

		void close_dialog() noexcept {
			if (dialogs_.empty()) {
				return;
			}

			dialogs_.pop();
			if (!dialogs_.empty()) {
				activate_current_dialog();
			}
			else {
				::PostQuitMessage(0);
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

	//export inline auto& dialog_mgr = DialogManager::instance();
	export inline DialogManager& dialog_mgr() noexcept {
		return DialogManager::Instance();
	}

} //namespace petools