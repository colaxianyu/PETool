module;

#include <windows.h>

export module PeContext;

import std.compat;
import FileManager;

namespace petools::pe {

    export enum class PeErrorOP {
        FileOpenFailed,
        InvalidDosHeader,
        InvalidNtHeader,
        UnsupportedOptionalHeaderMagic,
        TruncatedHeaders,
        Unknown,
    };

    export struct PeError {
        PeErrorOP op{ PeErrorOP::Unknown };
        std::error_code code{};
        core::io::FileError file_error{};
        std::string message;
    };

    template <typename T>
    using pe_result = std::expected<T, PeError>;

    export struct PeHeaders32View {
        const IMAGE_DOS_HEADER* dos{ nullptr };
        const IMAGE_NT_HEADERS32* nt{ nullptr };
        const IMAGE_FILE_HEADER* file{ nullptr };
        const IMAGE_OPTIONAL_HEADER32* opt{ nullptr };
        const IMAGE_SECTION_HEADER* sec{ nullptr };
        std::uint16_t num_sections{ 0 };
    };

    export struct PeHeaders64View {
        const IMAGE_DOS_HEADER* dos{ nullptr };
        const IMAGE_NT_HEADERS64* nt{ nullptr };
        const IMAGE_FILE_HEADER* file{ nullptr };
        const IMAGE_OPTIONAL_HEADER64* opt{ nullptr };
        const IMAGE_SECTION_HEADER* sec{ nullptr };
        std::uint16_t num_sections{ 0 };
    };

    export using PeHeadersView = std::variant<PeHeaders32View, PeHeaders64View>;

    export class PeModel {
    public:
        PeModel() = default;

        PeModel(const PeModel&) = default;
        PeModel& operator=(const PeModel&) = default;

        PeModel(PeModel&&) noexcept = default;
        PeModel& operator=(PeModel&&) noexcept = default;

        ~PeModel() = default;

        static pe_result<PeModel> FromBytes(std::span<const std::byte> bytes);

        [[nodiscard]] const PeHeadersView& Headers() const noexcept {
            return headers_;
        }

        [[nodiscard]] bool IsPe32Plus() const noexcept {
            return std::holds_alternative<PeHeaders64View>(headers_);
        }

        [[nodiscard]] std::uint32_t AddressOfEntryPoint() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->AddressOfEntryPoint : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint64_t ImageBase() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint64_t {
                    if (!h.opt) {
                        return 0ull;
                    }
                    return static_cast<std::uint64_t>(h.opt->ImageBase);
                },
                headers_);
        }

        [[nodiscard]] std::uint32_t SizeOfImage() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->SizeOfImage : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t SectionAlignment() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->SectionAlignment : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t FileAlignment() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->FileAlignment : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t SizeOfHeaders() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->SizeOfHeaders : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t BaseOfCode() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->BaseOfCode : 0u; },
                headers_);
        }

        [[nodiscard]] std::optional<std::uint32_t> BaseOfData() const noexcept {
            if (auto p32 = std::get_if<PeHeaders32View>(&headers_); p32 && p32->opt) {
                return p32->opt->BaseOfData;
            }
            return std::nullopt;
        }

        [[nodiscard]] std::uint16_t Subsystem() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint16_t { return h.opt ? h.opt->Subsystem : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint16_t NumberOfSections() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint16_t { return h.num_sections; },
                headers_);
        }

        [[nodiscard]] std::uint16_t SizeOfOptionalHeader() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint16_t { return h.file ? h.file->SizeOfOptionalHeader : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint16_t Characteristics() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint16_t { return h.file ? h.file->Characteristics : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t TimeDateStamp() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.file ? h.file->TimeDateStamp : 0u; },
                headers_);
        }

        [[nodiscard]] std::uint32_t NumberOfRvaAndSizes() const noexcept {
            return std::visit(
                [](const auto& h) -> std::uint32_t { return h.opt ? h.opt->NumberOfRvaAndSizes : 0u; },
                headers_);
        }

        [[nodiscard]] std::optional<IMAGE_DATA_DIRECTORY> DataDirectory(std::size_t index) const noexcept {
            return std::visit(
                [index](const auto& h) -> std::optional<IMAGE_DATA_DIRECTORY> {
                    if (!h.opt) {
                        return std::nullopt;
                    }
                    const auto count = static_cast<std::size_t>(h.opt->NumberOfRvaAndSizes);
                    if (index >= count || index >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
                        return std::nullopt;
                    }
                    return h.opt->DataDirectory[index];
                },
                headers_);
        }

        [[nodiscard]] std::span<const std::byte> Bytes() const noexcept {
            return bytes_;
        }

        [[nodiscard]] std::span<const IMAGE_SECTION_HEADER> Sections() const noexcept {
            return std::visit(
                [](const auto& h) -> std::span<const IMAGE_SECTION_HEADER> {
                    if (!h.sec || h.num_sections == 0) {
                        return {};
                    }
                    return { h.sec, static_cast<std::size_t>(h.num_sections) };
                },
                headers_);
        }

    private:
        std::span<const std::byte> bytes_{};
        PeHeadersView headers_{ PeHeaders32View{} };
    };

    // ---------- FileManager + PeModel 打包的“打开的 PE 文档” ----------

    export class PeContext {
    public:
        PeContext() = delete;
        
        PeContext(const PeContext&) = delete;
        PeContext& operator=(const PeContext&) = delete;

        PeContext(PeContext&&) noexcept = default;
        PeContext& operator=(PeContext&&) noexcept = default;

        ~PeContext() = default;

        static pe_result<PeContext> Open(const std::filesystem::path& path);

        [[nodiscard]] const std::filesystem::path& Path() const noexcept {
            return file_.GetPath();
        }

        [[nodiscard]] std::span<const std::byte> Bytes() const noexcept {
            return file_.GetBytes();
        }

        [[nodiscard]] std::size_t Size() const noexcept {
            return file_.GetSize();
        }

        [[nodiscard]] const PeModel& Model() const noexcept {
            return model_;
        }

        [[nodiscard]] PeModel& Model() noexcept {
            return model_;
        }

    private:
        core::io::FileManager file_;
        PeModel     model_;

        PeContext(core::io::FileManager&& file, PeModel&& model) noexcept
            : file_(std::move(file))
            , model_(std::move(model)) {
        }
    };

} // namespace petools::pe