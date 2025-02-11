module;

export module HeteroMap;

import STL;

using std::variant;
using std::unordered_map;
using std::forward;
using std::conditional_t;

template <typename T>
struct storage_holder {
	bool is_storage = false;
	T value;

	constexpr void set(T&& r_value) {
		value = forward<T>(r_value);
		is_storage = true;
	}

	constexpr const T& get() const {
		if (!is_storage) {
			throw std::runtime_error("No value stored in storage_holder");
		}
		return value;
	}

	constexpr auto get() {
		if (!is_storage) {
			throw std::runtime_error("No value stored in storage_holder");
		}
		return value;
	}
};

template <unsigned int Key, typename... Type>
struct value_at_key;

template <unsigned int Key, typename In, typename... Res>
struct value_at_key<Key, In, Res...> {
	using type = conditional_t <
		Key == In::id,
		In,
		typename value_at_key<Key, Res...>::type
	>;
};

template <unsigned int Key, typename Last>
struct value_at_key<Key, Last> {
	static_assert(Key == Last::id, "Key not found in VarTypeDict");
	using type = Last;
};

template <unsigned int Key>
struct value_at_key<Key> {
	static_assert(Key != Key, "Key not found in VarTypeDict");
	using type = void;
};

template <unsigned int Key, typename... Type>
using value_at_key_t = typename value_at_key<Key, Type...>::type;

template <typename... Types>
struct hetero_map : storage_holder<Types> {
	template <unsigned int Key, typename TValue>
	void set(TValue&& value) {
		using T = value_at_key_t<Key, Types...>;
		storage_holder<T>::set(forward<TValue>(value));
	}

	template <unsigned int Key>
	const auto& get() const {
		using type = value_at_key_t<Key, Types...>;
		return storage_holder<type>::get();
	}

	template <unsigned int Key>
	auto& get() {
		using type = value_at_key_t<Key, Types...>;
		return storage_holder<type>::get();
	}
};