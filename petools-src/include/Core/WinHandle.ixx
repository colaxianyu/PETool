module;

#include <windows.h>
#include <assert.h>

export module WinHandle;

import STL;
import WinHandleTraits;


using std::exchange;

namespace petools {

    export template <typename HandleType, typename Deleter>
        class WinHandleRef {
        private:
            using traits = handle_traits<HandleType, Deleter>;

        public:
            explicit WinHandleRef(const HandleType& handle) noexcept : handle_(handle) {}

            ~WinHandleRef() noexcept = default;

            WinHandleRef(const WinHandleRef& other) {
                handle_ = other.handle_;
            }
            WinHandleRef& operator=(const WinHandleRef& other) {
                if (this != &other) {
                    handle_ = other.handle_;
                }
                return *this;
            }

            WinHandleRef(WinHandleRef&& other) = delete;
            WinHandleRef& operator=(WinHandleRef&& other) = delete;

            [[nodiscard]] bool is_valid() const noexcept {
                return handle_ != traits::invalid_value && traits::is_valid(handle_);
            }

            [[nodiscard]] operator HandleType() const noexcept { return handle_; }
            [[nodiscard]] operator bool() const noexcept { return is_valid(); }
            [[nodiscard]] HandleType get() const noexcept { return handle_; }

        private:
            HandleType handle_;
    };

    export template <typename HandleType, typename Deleter = typename handle_traits<HandleType, void>::deleter_type>
        class WinHandle {
        private:
            using traits = handle_traits<HandleType, Deleter>;

        public:
            using handle_type = HandleType;
            using deleter_type = Deleter;

            WinHandle() noexcept : handle_(traits::invalid_value) {}

            explicit WinHandle(HandleType handle) noexcept :handle_(handle) {
                if (handle_ && !traits::is_valid(handle_)) {
                    assert(false && "Invalid handle provided to constructor");
                    handle_ = traits::invalid_value;
                }
            }

            ~WinHandle() noexcept { reset(); }

            WinHandle(const WinHandle&) = delete;
            WinHandle& operator=(const WinHandle&) = delete;

            WinHandle(WinHandle&& other) noexcept
                : handle_(exchange(other.handle_, traits::invalid_value)) {
            }

            WinHandle& operator=(WinHandle&& other) noexcept {
                if (this != &other) {
                    handle_ = exchange(other.handle_, traits::invalid_value);
                }
                return *this;
            }

            void reset() noexcept {
                HandleType old = exchange(handle_, traits::invalid_value);
                if (old != traits::invalid_value && traits::is_valid(old)) {
                    const BOOL success = traits::deleter(old);
                    assert(success && "Handle deletion failed!");
                    (void)success;
                }
            }

            void reset(HandleType new_handle) noexcept {
                reset();
                handle_ = new_handle;
                if (handle_ != traits::invalid_value && !traits::is_valid(handle_)) {
                    assert(false && "Invalid handle provided to reset");
                    handle_ = traits::invalid_value;
                }
            }

            HandleType release() noexcept {
                return exchange(handle_, traits::invalid_value);
            }

            [[nodiscard]] bool is_valid() const noexcept {
                return traits::is_valid(handle_);
            }

            [[nodiscard]] WinHandleRef<HandleType, Deleter> borrow() const noexcept {
                return WinHandleRef<HandleType, Deleter>(handle_);
            }

            [[nodiscard]] operator HandleType() const noexcept { return handle_; }
            [[nodiscard]] operator bool() const noexcept { return is_valid(); }
            [[nodiscard]] HandleType get() const noexcept { return handle_; }

            [[nodiscard]] bool operator==(const WinHandle& other) const noexcept { return handle_ == other.handle_; }
            [[nodiscard]] bool operator==(HandleType other) const noexcept { return handle_ == other; }
            [[nodiscard]] bool operator==(std::nullptr_t) const noexcept { return handle_ == traits::invalid_value; }

        private:
            HandleType handle_;
    };

    export template<typename HandleType, typename Deleter = typename handle_traits<HandleType, void>::deleter_type>
        [[nodiscard]] auto make_handle(HandleType handle) {
        return WinHandle<HandleType, Deleter>(handle);
    }

    export using WindowHandle = WinHandle<HWND, decltype(&DestroyWindow)>;
    export using MenuHandle = WinHandle<HMENU, decltype(&DestroyMenu)>;
    export using IconHandle = WinHandle<HICON, decltype(&DestroyIcon)>;
    export using CursorHandle = WinHandle<HCURSOR, decltype(&DestroyCursor)>;
    export using FileHandle = WinHandle<HANDLE, decltype(&CloseHandle)>;
    export using ModuleHandle = WinHandle<HMODULE, decltype(&FreeLibrary)>;

    export using WindowHandleRef = WinHandleRef<HWND, decltype(&DestroyWindow)>;
    export using MenuHandleRef = WinHandleRef<HMENU, decltype(&DestroyMenu)>;
    export using IconHandleRef = WinHandleRef<HICON, decltype(&DestroyIcon)>;
    export using CursorHandleRef = WinHandleRef<HCURSOR, decltype(&DestroyCursor)>;
    export using FileHandleRef = WinHandleRef<HANDLE, decltype(&CloseHandle)>;
    export using ModuleHandleRef = WinHandleRef<HMODULE, decltype(&FreeLibrary)>;

} // namespace petools