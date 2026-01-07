module;

#include <windows.h>
#include "resources/resource.h"

export module RelocationDlg;

import STL;
import DialogEX;
import WinHandle;
import ListControl;

namespace petools::gui {

	export class RelocationDlg final : public DialogEX {
	public:
		explicit RelocationDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_RELOCATION, parent) {}
		~RelocationDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void ShowDialog() noexcept override;

		void init_block_list() noexcept;
		void plant_block_column() noexcept;
		void plant_block_item() noexcept;

		void init_block_detail_list() noexcept;
		void plant_block_detail_column() noexcept;
		void plant_block_detail_item() noexcept;

		//void InitBlockList();
		//void plantBlockColumn();
		//void plantBlockItem();

		//void InitBlockItemList();
		//void plantBlockItemColumn();
		//void plantBlockItemItem();

		//DWORD GetRelocationTableIndex(DWORD rowID);

	private:
		std::unique_ptr<ListCtrl> block_list_ = nullptr;
		std::unique_ptr<ListCtrl> block_detail_list_ = nullptr;

		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools