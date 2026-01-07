module;

#include <windows.h>
#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <vector>
#include <array>
#include <stack>
#include <span>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <optional>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <source_location>

export module STL;

import std;

// Group related using declarations by category
export namespace std {
    // Smart Pointers
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::enable_shared_from_this;

    using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;

    // Containers
    using std::vector;
    using std::stack;
    using std::array;
    using std::span;
	using std::variant;
    using std::unordered_set;
    using std::unordered_map;

    // String Types
    using std::basic_string;
    using std::string;
    using std::string_view;
    using std::wstring;
    using std::wstring_view;

    // Time Types
    using std::time_t;
    using std::tm;

    // Type Traits and Utilities
    using std::same_as;
    using std::void_t;
    using std::false_type;
    using std::true_type;
    using std::derived_from;
    using std::conditional_t;
    using std::constructible_from;
    using std::integral_constant;
    using std::is_integral_v;
    using std::is_unsigned_v;
    using std::is_base_of_v;
    using std::is_invocable_r_v;


    // Function and Binding
    using std::function;
    using std::bind;
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;

    // Miscellaneous
    using std::optional;
    using std::size_t;
    using std::pair;
    using std::atomic;
    using std::atomic_int;
    using std::uint8_t;
    using std::source_location;
    using std::cout;

    // Utility Functions
    using std::declval;
    using std::find_if;
    using std::exchange;
    using std::any_of;
    using std::to_address;
    using std::forward;
    using std::move;
    using std::make_shared;
    using std::make_unique;
	using std::make_optional;
    using std::ceil;
    using std::nullopt;
    using std::visit;
	using std::byte;

    namespace chrono {
        using std::chrono::time_point;
        using std::chrono::system_clock;
    }

	namespace ranges {
		using std::ranges::subrange;
	}
}

