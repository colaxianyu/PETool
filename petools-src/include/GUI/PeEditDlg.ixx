module;

#include <windows.h>
#include "resources/resource.h"

export module PeEditDlg;

import std.compat;
import DialogEX;
import WinHandle;
import Utility;
import PeContext;

namespace petools::gui {

	export class PeEditDlg final : public DialogEX {
	public:
		explicit PeEditDlg(HWND h_parent, std::string file_path) noexcept
			: DialogEX(IDD_DIALOG_PEEDIT, h_parent)
		{
			pe_ctx_res_.emplace(petools::pe::PeContext::Open(std::filesystem::path(file_path)));
		}

		~PeEditDlg() noexcept override = default;

		void InitDialog() noexcept override;

		void SetTitle() noexcept;
		void SetMainInfo() noexcept;

		[[nodiscard]] bool HasPe() const noexcept { return pe_ctx_res_.has_value() && pe_ctx_res_->has_value(); }
		[[nodiscard]] const petools::pe::PeContext* Pe() const noexcept { return HasPe() ? &pe_ctx_res_->value() : nullptr; }
		[[nodiscard]] const petools::pe::PeError* PeError() const noexcept { return (!pe_ctx_res_ || pe_ctx_res_->has_value()) ? nullptr : &pe_ctx_res_->error(); }
		
		//void SaveAsFile();
		//FileManage* GetFileManage() { return fileManage_.get(); }
		//std::unique_ptr<FileManager>& GetFileManager() noexcept { return file_manager_; }

	protected:
		//FileManager2 file_manager_;

	private:
		static constexpr const wchar_t* title_prefix_ = L"PE编辑器 - ";
		std::optional<std::expected<petools::pe::PeContext, petools::pe::PeError>> pe_ctx_res_;

		LRESULT HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) override;
	};

} //namespace petools
