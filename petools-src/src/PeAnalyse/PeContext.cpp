module;

#include <windows.h>
#include <winnt.h>

module PeContext;

import std.compat;
import STL;
import FileManager;

namespace petools::pe {

	pe_result<PeModel> PeModel::FromBytes(std::span<const std::byte> bytes) {
		PeModel model;
		model.bytes_ = bytes;

		if (bytes.size() < sizeof(IMAGE_DOS_HEADER)) {
			PeError err{
				.op = PeErrorOP::InvalidDosHeader,
				.message = "File too small to contain a valid DOS header."
			};
			return std::unexpected(err);
		}

		auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(bytes.data());
		if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
			PeError err{
				.op = PeErrorOP::InvalidDosHeader,
				.message = "Invalid DOS header signature (expected 'MZ')."
			};
			return std::unexpected(err);
		}

		if (dos->e_lfanew <= 0) {
			PeError err{
				.op = PeErrorOP::InvalidNtHeader,
				.message = "e_lfanew is non-positive; cannot locate NT headers."
			};
			return std::unexpected(err);
		}

		const std::size_t nt_offset = static_cast<std::size_t>(dos->e_lfanew);

		constexpr std::size_t nt_min_size = sizeof(std::uint32_t) + sizeof(IMAGE_FILE_HEADER);
		if (nt_offset > bytes.size() || nt_min_size > bytes.size() - nt_offset) {
			PeError err{
				.op = PeErrorOP::TruncatedHeaders,
				.message = "File too small to contain NT signature and file header."
			};
			return std::unexpected(err);
		}

		const auto nt_sig = *reinterpret_cast<const std::uint32_t*>(bytes.data() + nt_offset);
		if (nt_sig != IMAGE_NT_SIGNATURE) {
			PeError err{
				.op = PeErrorOP::InvalidNtHeader,
				.message = "Invalid NT header signature (expected 'PE\\0\\0')."
			};
			return std::unexpected(err);
		}

		const auto file_hdr = reinterpret_cast<const IMAGE_FILE_HEADER*>(bytes.data() + nt_offset + sizeof(std::uint32_t));
		const std::size_t opt_offset = nt_offset + sizeof(std::uint32_t) + sizeof(IMAGE_FILE_HEADER);
		const std::size_t opt_size = static_cast<std::size_t>(file_hdr->SizeOfOptionalHeader);
		if (opt_offset > bytes.size() || opt_size > bytes.size() - opt_offset) {
			PeError err{
				.op = PeErrorOP::TruncatedHeaders,
				.message = "Optional header truncated or out of file bounds."
			};
			return std::unexpected(err);
		}

		if (opt_size < sizeof(std::uint16_t)) {
			PeError err{
				.op = PeErrorOP::TruncatedHeaders,
				.message = "Optional header too small to contain Magic field."
			};
			return std::unexpected(err);
		}

		const auto magic = *reinterpret_cast<const std::uint16_t*>(bytes.data() + opt_offset);

		const std::uint16_t num_sections = file_hdr->NumberOfSections;
		const std::size_t section_offset = opt_offset + opt_size;
		const std::size_t section_bytes = static_cast<std::size_t>(num_sections) * sizeof(IMAGE_SECTION_HEADER);

		if (num_sections > 0) {
			if (section_offset > bytes.size() || section_bytes > bytes.size() - section_offset) {
				PeError err{
					.op = PeErrorOP::TruncatedHeaders,
					.message = "Section table truncated or out of file bounds."
				};
				return std::unexpected(err);
			}
		}

		const auto first_section = (num_sections > 0)
			? reinterpret_cast<const IMAGE_SECTION_HEADER*>(bytes.data() + section_offset)
			: nullptr;

		if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
			if (opt_size < sizeof(IMAGE_OPTIONAL_HEADER32)) {
				PeError err{
					.op = PeErrorOP::TruncatedHeaders,
					.message = "Optional header truncated for PE32."
				};
				return std::unexpected(err);
			}

			const auto nt32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(bytes.data() + nt_offset);
			const auto opt32 = reinterpret_cast<const IMAGE_OPTIONAL_HEADER32*>(bytes.data() + opt_offset);

			model.headers_ = PeHeaders32View{
				.dos = dos,
				.nt = nt32,
				.file = file_hdr,
				.opt = opt32,
				.sec = first_section,
				.num_sections = num_sections,
			};
		}
		else if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
			if (opt_size < sizeof(IMAGE_OPTIONAL_HEADER64)) {
				PeError err{
					.op = PeErrorOP::TruncatedHeaders,
					.message = "Optional header truncated for PE32+."
				};
				return std::unexpected(err);
			}

			const auto nt64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(bytes.data() + nt_offset);
			const auto opt64 = reinterpret_cast<const IMAGE_OPTIONAL_HEADER64*>(bytes.data() + opt_offset);

			model.headers_ = PeHeaders64View{
				.dos = dos,
				.nt = nt64,
				.file = file_hdr,
				.opt = opt64,
				.sec = first_section,
				.num_sections = num_sections,
			};
		}
		else {
			PeError err{
				.op = PeErrorOP::UnsupportedOptionalHeaderMagic,
				.message = "Unsupported OptionalHeader.Magic."
			};
			return std::unexpected(err);
		}

		return model;
	}

	pe_result<PeContext> PeContext::Open(const std::filesystem::path& path) {
		auto fm_res = core::io::FileManager::OpenReadOnly(path);
		if (!fm_res) {
			const core::io::FileError& ferr = fm_res.error();
			PeError err;
			err.op = PeErrorOP::FileOpenFailed;
			err.message = "Failed to open file: " + path.string();
			err.file_error = ferr;
			err.code = ferr.code;
			return std::unexpected(err);
		}

		core::io::FileManager file = std::move(fm_res).value();

		auto model_res = PeModel::FromBytes(file.GetBytes());
		if (!model_res) {
			return std::unexpected(model_res.error());
		}

		PeModel model = std::move(model_res).value();
		return PeContext(std::move(file), std::move(model));
	}

} // namespace petools::pe
