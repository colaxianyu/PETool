module;

#include <windows.h>
#include "resources/resource.h"

export module PeEditDlg;

import STL;
import DialogEX;
import WinHandle;
import FileManager;

using std::unique_ptr;
using std::vector;
using tools::config::filename_max;

namespace petools {

	export class PeEditDlg final : public DialogEX {
	public:
		explicit PeEditDlg(HWND h_parent, std::string file_path) noexcept
			: DialogEX(IDD_DIALOG_PEEDIT, h_parent),
			file_manager_(file_path) {}

		~PeEditDlg() noexcept override = default;

		void init_dialog() noexcept override;

		void set_dialog_title() noexcept;
		void set_main_info() noexcept;
		
		//void SaveAsFile();
		//FileManage* GetFileManage() { return fileManage_.get(); }
		//std::unique_ptr<FileManager>& GetFileManager() noexcept { return file_manager_; }

	protected:
		FileManager file_manager_;

	private:
		static constexpr const wchar_t* title_prefix_ = L"PE编辑器 - ";

		LRESULT handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) override;
	};

} //namespace petools
