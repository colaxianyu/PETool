module;

#include <windows.h>

export module Concept;

import STL;

namespace petools {

	export namespace numeric_base {
		template <std::uint16_t V>
		using base_constant = std::integral_constant<int, V>;

		using hex = base_constant<16>;
		using dec = base_constant<10>;
		using oct = base_constant<8>;
		using bin = base_constant<2>;

		template <typename V>
		concept NumericBase = std::same_as<V, hex> || std::same_as<V, dec> || std::same_as<V, oct> || std::same_as<V, bin>;
	}

	export {

		template <typename T>
		concept CanRanges = std::same_as<T, IMAGE_IMPORT_DESCRIPTOR>;
		//|| std::same_as<T, IMAGE_EXPORT_DIRECTORY>
		//|| std::same_as<T, IMAGE_BASE_RELOCATION>
		//|| std::same_as<T, IMAGE_RESOURCE_DIRECTORY>
		//|| std::same_as<T, IMAGE_RESOURCE_DIRECTORY_ENTRY>
		//|| std::same_as<T, IMAGE_RESOURCE_DATA_ENTRY>
		//|| std::same_as<T, IMAGE_IMPORT_BY_NAME>
		//|| std::same_as<T, IMAGE_THUNK_DATA>;

		template <typename T>
		struct base_mark {};
	}

} // namespace petools