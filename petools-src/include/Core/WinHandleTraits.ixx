module;

#include <windows.h>

export module WinHandleTraits;

namespace petools {

	template <typename HandleType>
	struct nullptr_handle_traits_base {
		using handle_type = HandleType;
		static constexpr handle_type invalid_value = nullptr;
		static bool is_valid(handle_type handle) noexcept { return handle != invalid_value; }
	};

	template <typename HandleType>
	struct invalid_handle_traits_base {
		using handle_type = HandleType;
		static constexpr handle_type invalid_value = INVALID_HANDLE_VALUE;
		static bool is_valid(handle_type handle) noexcept { return handle != invalid_value; }
	};

	export template <typename HandleType, typename Deleter>
		struct handle_traits;

	export template <>
		struct handle_traits<HWND, decltype(&DestroyWindow)> : nullptr_handle_traits_base<HWND> {
		using deleter_type = decltype(&DestroyWindow);
		static constexpr deleter_type deleter = DestroyWindow;
	};

	export template<>
		struct handle_traits<HMENU, decltype(&DestroyMenu)> : nullptr_handle_traits_base<HMENU> {
		using deleter_type = decltype(&DestroyMenu);
		static constexpr deleter_type deleter = DestroyMenu;
	};

	export template<>
		struct handle_traits<HICON, decltype(&DestroyIcon)> : nullptr_handle_traits_base<HICON> {
		using deleter_type = decltype(&DestroyIcon);
		static constexpr deleter_type deleter = DestroyIcon;
	};

	export template<>
		struct handle_traits<HMODULE, decltype(&FreeLibrary)> : nullptr_handle_traits_base<HMODULE> {
		using deleter_type = decltype(&FreeLibrary);
		static constexpr deleter_type deleter = FreeLibrary;
	};

	export template<>
		struct handle_traits<HANDLE, decltype(&CloseHandle)> : invalid_handle_traits_base<HANDLE> {
		using deleter_type = decltype(&CloseHandle);
		static constexpr deleter_type deleter = CloseHandle;
	};

} // namespace petools