module;

#include <windows.h>

export module DialogManager;

import std.compat;
import DialogEX;

namespace petools::gui {

	export class DialogManager {
	public:
		static DialogManager& Instance() noexcept {
			static DialogManager instance;
			return instance;
		}

		template <typename Dlg, typename... Args>
		[[nodiscard]] Dlg* OpenDialog(Args&&... args) {
			static_assert(std::is_base_of_v<DialogEX, Dlg>, "Dlg must derive from DialogEX");

			auto dialog = std::make_unique<Dlg>(std::forward<Args>(args)...);
			if (!dialog) {
				return nullptr;
			}

			if (!dialogs_.empty()) {
				DisableCurrentDialog();
			}

			HWND hdlg = ::CreateDialogParam(
				dialog->GetInstance(),
				MAKEINTRESOURCE(dialog->GetTemplateID()),
				dialog->GetParentHWND(),
				DialogEX::StaticDialogProc,
				reinterpret_cast<LPARAM>(dialog.get())
			);

			if (!hdlg) {
				::MessageBoxW(
					dialog->GetParentHWND(),
					L"创建对话框失败。",
					L"错误",
					MB_ICONERROR | MB_OK
				);
				return nullptr;
			}

			dialogs_.push(std::move(dialog));
			return static_cast<Dlg*>(dialogs_.top().get());
		}

		void CloseDialog() noexcept {
			if (dialogs_.empty()) {
				return;
			}

			dialogs_.pop();

			if (!dialogs_.empty()) {
				ActivateCurrentDialog();
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

		void DisableCurrentDialog() noexcept {
			if (auto hwnd = dialogs_.top()->GetCurrentHWND()) {
				::EnableWindow(hwnd, FALSE);
			}
		}

		void ActivateCurrentDialog() noexcept {
			if (auto hwnd = dialogs_.top()->GetCurrentHWND()) {
				::EnableWindow(hwnd, TRUE);
				::SetForegroundWindow(hwnd);
			}
		}

		std::stack<std::unique_ptr<DialogEX>> dialogs_;
	};

	export inline DialogManager& DialogMgr() noexcept {
		return DialogManager::Instance();
	}

} //namespace petools::gui