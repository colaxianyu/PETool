module;

#include <windows.h>
#include "resources/resource.h"

export module PeEditDlg;

import std.compat;
import DialogEX;
import WinHandle;
import FileManager;
import Utility;

namespace petools::gui {

	export class PeEditDlg final : public DialogEX {
	public:
		explicit PeEditDlg(HWND h_parent, std::string file_path) noexcept
			: DialogEX(IDD_DIALOG_PEEDIT, h_parent),
			file_manager_(file_path) {}

		~PeEditDlg() noexcept override = default;

		void InitDialog() noexcept override;

		void SetTitle() noexcept;
		void SetMainInfo() noexcept;
		
		//void SaveAsFile();
		//FileManage* GetFileManage() { return fileManage_.get(); }
		//std::unique_ptr<FileManager>& GetFileManager() noexcept { return file_manager_; }

	protected:
		FileManager file_manager_;

	private:
		static constexpr const wchar_t* title_prefix_ = L"PE编辑器 - ";

		LRESULT HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) override;
	};

} //namespace petools
